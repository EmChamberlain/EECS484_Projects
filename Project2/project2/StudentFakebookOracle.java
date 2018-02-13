package project2;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;

/*
    The StudentFakebookOracle class is derived from the FakebookOracle class and implements
    the abstract query functions that investigate the database provided via the <connection>
    parameter of the constructor to discover specific information.
*/
public final class StudentFakebookOracle extends FakebookOracle {
    // [Constructor]
    // REQUIRES: <connection> is a valid JDBC connection
    public StudentFakebookOracle(Connection connection) {
        oracle = connection;
    }
    
    @Override
    // Query 0
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the total number of users for which a birth month is listed
    //        (B) Find the birth month in which the most users were born
    //        (C) Find the birth month in which the fewest users (at least one) were born
    //        (D) Find the IDs, first names, and last names of users born in the month
    //            identified in (B)
    //        (E) Find the IDs, first names, and last name of users born in the month
    //            identified in (C)
    //
    // This query is provided to you completed for reference. Below you will find the appropriate
    // mechanisms for opening up a statement, executing a query, walking through results, extracting
    // data, and more things that you will need to do for the remaining nine queries
    public BirthMonthInfo findMonthOfBirthInfo() throws SQLException {
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            // Step 1
            // ------------
            // * Find the total number of users with birth month info
            // * Find the month in which the most users were born
            // * Find the month in which the fewest (but at least 1) users were born
            ResultSet rst = stmt.executeQuery(
                "SELECT COUNT(*) AS Birthed, Month_of_Birth " +         // select birth months and number of uses with that birth month
                "FROM " + UsersTable + " " +                            // from all users
                "WHERE Month_of_Birth IS NOT NULL " +                   // for which a birth month is available
                "GROUP BY Month_of_Birth " +                            // group into buckets by birth month
                "ORDER BY Birthed DESC, Month_of_Birth ASC");           // sort by users born in that month, descending; break ties by birth month
            
            long mostMonth = 0;
            long leastMonth = 0;
            long total = 0;
            while (rst.next()) {                       // step through result rows/records one by one
                if (rst.isFirst()) {                   // if first record
                    mostMonth = rst.getLong(2);         //   it is the month with the most
                }
                if (rst.isLast()) {                    // if last record
                    leastMonth = rst.getLong(2);        //   it is the month with the least
                }
                total += rst.getLong(1);                // get the first field's value as an integer
            }
            BirthMonthInfo info = new BirthMonthInfo(total, (int)(mostMonth), (int)(leastMonth));
            
            // Step 2
            // ------------
            // * Get the names of users born in the most popular birth month
            rst = stmt.executeQuery(
                "SELECT User_ID, First_Name, Last_Name " +                // select ID, first name, and last name
                "FROM " + UsersTable + " " +                              // from all users
                "WHERE Month_of_Birth = " + mostMonth + " " +             // born in the most popular birth month
                "ORDER BY User_ID");                                      // sort smaller IDs first
                
            while (rst.next()) {
                info.addMostPopularBirthMonthUser(new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3)));
            }

            // Step 3
            // ------------
            // * Get the names of users born in the least popular birth month
            rst = stmt.executeQuery(
                "SELECT User_ID, First_Name, Last_Name " +                // select ID, first name, and last name
                "FROM " + UsersTable + " " +                              // from all users
                "WHERE Month_of_Birth = " + leastMonth + " " +            // born in the least popular birth month
                "ORDER BY User_ID");                                      // sort smaller IDs first
                
            while (rst.next()) {
                info.addLeastPopularBirthMonthUser(new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3)));
            }

            // Step 4
            // ------------
            // * Close resources being used
            rst.close();
            stmt.close();                            // if you close the statement first, the result set gets closed automatically

            return info;

        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
            return new BirthMonthInfo(-1, -1, -1);
        }
    }
    
    @Override
    // Query 1
    // -----------------------------------------------------------------------------------
    // GOALS: (A) The first name(s) with the most letters
    //        (B) The first name(s) with the fewest letters
    //        (C) The first name held by the most users
    //        (D) The number of users whose first name is that identified in (C)
    public FirstNameInfo findNameInfo() throws SQLException {
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                FirstNameInfo info = new FirstNameInfo();
                info.addLongName("Aristophanes");
                info.addLongName("Michelangelo");
                info.addLongName("Peisistratos");
                info.addShortName("Bob");
                info.addShortName("Sue");
                info.addCommonName("Harold");
                info.addCommonName("Jessica");
                info.setCommonNameCount(42);
                return info;
            */
			
			ResultSet rst = stmt.executeQuery(
                "SELECT FIRST_NAME, COUNT(FIRST_NAME) " +   // select first names
                "FROM " + UsersTable + " " +
                "GROUP BY FIRST_NAME " +                            // group into buckets by birth month
                "ORDER BY LENGTH(FIRST_NAME) ASC, FIRST_NAME ASC");           // sort by users born in that month, descending; break ties by birth month

            rst.first();
			FirstNameInfo info = new FirstNameInfo();
			int shortest = rst.getString(1).length();
			
			while(rst.getString(1).length() == shortest) {
				info.addShortName(rst.getString(1));
				rst.next();
			}

            rst = stmt.executeQuery(
                    "SELECT FIRST_NAME, COUNT(FIRST_NAME) " +   // select first names
                            "FROM " + UsersTable + " " +
                            "GROUP BY FIRST_NAME " +                            // group into buckets by birth month
                            "ORDER BY LENGTH(FIRST_NAME) DESC, FIRST_NAME ASC");           // sort by users born in that month, descending; break ties by birth month
            rst.first();
            int longest = rst.getString(1).length();

            while(rst.getString(1).length() == longest) {
                info.addLongName(rst.getString(1));
                rst.next();
            }
			
			rst = stmt.executeQuery(
                "SELECT FIRST_NAME, COUNT(FIRST_NAME) AS NAMECOUNT " +   // select first names
                "FROM " + UsersTable + " " +
                "GROUP BY FIRST_NAME " +                            // group into buckets by birth month
                "ORDER BY NAMECOUNT DESC, FIRST_NAME ASC"); 
            rst.first();
			long maxFreq = rst.getLong(2);
			info.setCommonNameCount(maxFreq);
			info.addCommonName(rst.getString(1));

			
            return info;                // placeholder for compilation
        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
            return new FirstNameInfo();
        }
    }
    
    @Override
    // Query 2
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the IDs, first names, and last names of users without any friends
    //
    // Be careful! Remember that if two users are friends, the Friends table only contains
    // the one entry (U1, U2) where U1 < U2.
    public FakebookArrayList<UserInfo> lonelyUsers() throws SQLException {
        FakebookArrayList<UserInfo> results = new FakebookArrayList<UserInfo>(", ");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo u1 = new UserInfo(15, "Abraham", "Lincoln");
                UserInfo u2 = new UserInfo(39, "Margaret", "Thatcher");
                results.add(u1);
                results.add(u2);
            */
			ResultSet rst = stmt.executeQuery(
                "SELECT USER_ID, FIRST_NAME, LAST_NAME " +
                "FROM " + UsersTable + " " +
				"WHERE USER_ID NOT IN (SELECT USER1_ID FROM " + FriendsTable + ") AND USER_ID NOT IN (SELECT USER2_ID FROM " + FriendsTable + ") " +
                "ORDER BY USER_ID ASC"); 
				
			while(rst.next()) {
                results.add(new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3)));
            }
        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    @Override
    // Query 3
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the IDs, first names, and last names of users who no longer live
    //            in their hometown (i.e. their current city and their hometown are different)
    public FakebookArrayList<UserInfo> liveAwayFromHome() throws SQLException {
        FakebookArrayList<UserInfo> results = new FakebookArrayList<UserInfo>(", ");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo u1 = new UserInfo(9, "Meryl", "Streep");
                UserInfo u2 = new UserInfo(104, "Tom", "Hanks");
                results.add(u1);
                results.add(u2);
            */

            ResultSet rst = stmt.executeQuery("SELECT U.USER_ID, U.FIRST_NAME, U.LAST_NAME FROM " + UsersTable +
                    " U JOIN " + CurrentCitiesTable + " CC ON CC.USER_ID = U.USER_ID " +
                    "JOIN " + HometownCitiesTable + " HC ON HC.USER_ID = U.USER_ID " +
                    "WHERE CC.CURRENT_CITY_ID != HC.HOMETOWN_CITY_ID " +
                    "ORDER BY U.USER_ID ASC");
            while(rst.next())
            {
                results.add(new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3)));
            }
        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    @Override
    // Query 4
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the IDs, links, and IDs and names of the containing album of the top
    //            <num> photos with the most tagged users
    //        (B) For each photo identified in (A), find the IDs, first names, and last names
    //            of the users therein tagged
    public FakebookArrayList<TaggedPhotoInfo> findPhotosWithMostTags(int num) throws SQLException {
        FakebookArrayList<TaggedPhotoInfo> results = new FakebookArrayList<TaggedPhotoInfo>("\n");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                PhotoInfo p = new PhotoInfo(80, 5, "www.photolink.net", "Winterfell S1");
                UserInfo u1 = new UserInfo(3901, "Jon", "Snow");
                UserInfo u2 = new UserInfo(3902, "Arya", "Stark");
                UserInfo u3 = new UserInfo(3903, "Sansa", "Stark");
                TaggedPhotoInfo tp = new TaggedPhotoInfo(p);
                tp.addTaggedUser(u1);
                tp.addTaggedUser(u2);
                tp.addTaggedUser(u3);
                results.add(tp);
            */
            Statement stmt1 = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly);

			ResultSet rst = stmt.executeQuery(
			        "SELECT T.TAG_PHOTO_ID AS PHOTO_ID FROM " +  TagsTable + " T " +
                            "GROUP BY T.TAG_PHOTO_ID " +
                            "ORDER BY COUNT(T.TAG_PHOTO_ID) DESC, T.TAG_PHOTO_ID ASC"
            );


            String photoDetails = "SELECT A.ALBUM_ID, P.PHOTO_LINK, A.ALBUM_NAME FROM " + PhotosTable + " P " +
                    "JOIN " + AlbumsTable + " A ON A.ALBUM_ID = P.ALBUM_ID " +
                    "WHERE P.PHOTO_ID = ";
            int counter = 0;
            while(rst.next() && (counter++ < num))
            {
                long pid = rst.getLong(1);
                ResultSet rst1 = stmt1.executeQuery(photoDetails + pid);
                rst1.first();
                PhotoInfo p = new PhotoInfo(pid, rst1.getLong(1), rst1.getString(2), rst1.getString(3));

                rst1 = stmt1.executeQuery(
                        "SELECT U.USER_ID, U.FIRST_NAME, U.LAST_NAME FROM " + TagsTable + " T " +
                                "JOIN " + UsersTable + " U ON U.USER_ID = T.TAG_SUBJECT_ID " +
                                "WHERE T.TAG_PHOTO_ID = " + pid +
                                "ORDER BY U.USER_ID ASC"
                );
                TaggedPhotoInfo tp = new TaggedPhotoInfo(p);
                while(rst1.next())
                {
                    tp.addTaggedUser(new UserInfo(rst1.getLong(1), rst1.getString(2), rst1.getString(3)));
                }
                results.add(tp);
            }



        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    @Override
    // Query 5
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the IDs, first names, last names, and birth years of each of the two
    //            users in the top <num> pairs of users that meet each of the following
    //            criteria:
    //              (i) same gender
    //              (ii) tagged in at least one common photo
    //              (iii) difference in birth years is no more than <yearDiff>
    //              (iv) not friends
    //        (B) For each pair identified in (A), find the IDs, links, and IDs and names of
    //            the containing album of each photo in which they are tagged together
    public FakebookArrayList<MatchPair> matchMaker(int num, int yearDiff) throws SQLException {
        FakebookArrayList<MatchPair> results = new FakebookArrayList<MatchPair>("\n");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo u1 = new UserInfo(93103, "Romeo", "Montague");
                UserInfo u2 = new UserInfo(93113, "Juliet", "Capulet");
                MatchPair mp = new MatchPair(u1, 1597, u2, 1597);
                PhotoInfo p = new PhotoInfo(167, 309, "www.photolink.net", "Tragedy");
                mp.addSharedPhoto(p);
                results.add(mp);
            */

            ResultSet rst1 = stmt.executeQuery(
                    "SELECT N.USER1_ID, U1.FIRST_NAME, U1.LAST_NAME, U1.YEAR_OF_BIRTH,  " +
                            "N.USER2_ID, U2.FIRST_NAME, U2.LAST_NAME, U2.YEAR_OF_BIRTH FROM  " +
                            "( " +
                            "SELECT DISTINCT T1.TAG_SUBJECT_ID AS USER1_ID, T2.TAG_SUBJECT_ID AS USER2_ID, T1.TAG_PHOTO_ID AS PHOTO_ID FROM " + TagsTable + " T1 " +
                            "JOIN " + TagsTable + " T2 ON T1.TAG_PHOTO_ID = T2.TAG_PHOTO_ID AND T1.TAG_SUBJECT_ID < T2.TAG_SUBJECT_ID " +
                            ") N " +
                            "JOIN " + UsersTable + " U1 ON U1.USER_ID = N.USER1_ID " +
                            "JOIN " + UsersTable + " U2 ON U2.USER_ID = N.USER2_ID " +
                            "WHERE U1.GENDER = U2.GENDER " +
                            "AND NOT EXISTS (SELECT USER1_ID, USER2_ID FROM  " +
                            "( " +
                            "(SELECT USER1_ID, USER2_ID FROM " + FriendsTable + " WHERE USER1_ID < USER2_ID) " +
                            "UNION " +
                            "(SELECT USER2_ID AS USER1_ID, USER1_ID AS USER2_ID FROM " + FriendsTable + " WHERE USER1_ID > USER2_ID) " +
                            ") F " +
                            "WHERE F.USER1_ID = N.USER1_ID AND F.USER2_ID = N.USER2_ID " +
                            ") " +
                            "AND ABS(U1.YEAR_OF_BIRTH - U2.YEAR_OF_BIRTH) <= 2 " +
                            "ORDER BY N.USER1_ID ASC, N.USER2_ID ASC"
            );

            Statement stmt2 = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly);




            while(rst1.next())
            {

                UserInfo u1 = new UserInfo(rst1.getLong(1), rst1.getString(2),rst1.getString(3));
                UserInfo u2 = new UserInfo(rst1.getLong(5), rst1.getString(6),rst1.getString(7));
                MatchPair mp = new MatchPair(u1, rst1.getLong(4), u2, rst1.getLong(8));

                long uid1 = rst1.getLong(1);
                long uid2 = rst1.getLong(5);

                ResultSet rst2 = stmt2.executeQuery(
                        "SELECT P.PHOTO_ID, P.ALBUM_ID, P.PHOTO_LINK, A.ALBUM_NAME  FROM " + PhotosTable + " P " +
                                "JOIN " + AlbumsTable + " A ON A.ALBUM_ID = P.ALBUM_ID " +
                                "JOIN " + TagsTable + " T1 ON T1.TAG_PHOTO_ID = P.PHOTO_ID " +
                                "JOIN " + TagsTable + " T2 ON T2.TAG_PHOTO_ID = P.PHOTO_ID AND T1.TAG_SUBJECT_ID < T2.TAG_SUBJECT_ID " +
                                "WHERE T1.TAG_SUBJECT_ID = " + uid1 + " AND T2.TAG_SUBJECT_ID = " + uid2
                );

                while(rst2.next())
                {
                    mp.addSharedPhoto(new PhotoInfo(rst2.getLong(1), rst2.getLong(2), rst2.getString(3), rst2.getString(4)));
                }
                results.add(mp);
            }



        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    @Override
    // Query 6
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the IDs, first names, and last names of each of the two users in
    //            the top <num> pairs of users who are not friends but have a lot of
    //            common friends
    //        (B) For each pair identified in (A), find the IDs, first names, and last names
    //            of all the two users' common friends
    public FakebookArrayList<UsersPair> suggestFriends(int num) throws SQLException {
        FakebookArrayList<UsersPair> results = new FakebookArrayList<UsersPair>("\n");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo u1 = new UserInfo(16, "The", "Hacker");
                UserInfo u2 = new UserInfo(80, "Dr.", "Marbles");
                UserInfo u3 = new UserInfo(192, "Digit", "Le Boid");
                UsersPair up = new UsersPair(u1, u2);
                up.addSharedFriend(u3);
                results.add(up);
            */

            String view = "CREATE VIEW FVIEW AS\n" +
                    "(SELECT USER1_ID, USER2_ID FROM " +  FriendsTable + " WHERE USER1_ID < USER2_ID)\n" +
                    "UNION\n" +
                    "(SELECT USER2_ID AS USER1_ID, USER1_ID AS USER2_ID FROM " +  FriendsTable + " WHERE USER1_ID > USER2_ID)";
            String mutualView = "CREATE VIEW MUTUALVIEW AS \n" +
                    "(\n" +
                    "(\n" +
                    "SELECT DISTINCT A.USER1_ID AS A_ID, A.USER2_ID AS B_ID, C.USER2_ID AS C_ID FROM FVIEW A\n" +
                    "JOIN FVIEW C ON A.USER2_ID = C.USER1_ID AND A.USER1_ID < C.USER2_ID\n" +
                    ")\n" +
                    "UNION\n" +
                    "(\n" +
                    "SELECT DISTINCT A.USER2_ID AS A_ID, A.USER1_ID AS B_ID, C.USER2_ID AS C_ID FROM FVIEW A\n" +
                    "JOIN FVIEW C ON A.USER1_ID = C.USER1_ID AND A.USER2_ID < C.USER2_ID\n" +
                    ")\n" +
                    "UNION\n" +
                    "(\n" +
                    "SELECT DISTINCT C.USER2_ID AS A_ID, A.USER1_ID AS B_ID, A.USER2_ID AS C_ID FROM FVIEW A\n" +
                    "JOIN FVIEW C ON A.USER1_ID = C.USER1_ID AND C.USER2_ID < A.USER2_ID\n" +
                    "JOIN FVIEW C ON A.USER1_ID = C.USER1_ID AND C.USER2_ID < A.USER2_ID\n" +
                    ")\n" +
                    "UNION\n" +
                    "(\n" +
                    "SELECT DISTINCT A.USER1_ID AS A_ID, A.USER2_ID AS B_ID, C.USER1_ID AS C_ID FROM FVIEW A\n" +
                    "JOIN FVIEW C ON A.USER2_ID = C.USER2_ID AND A.USER1_ID < C.USER1_ID\n" +
                    ")\n" +
                    "UNION\n" +
                    "(\n" +
                    "SELECT DISTINCT C.USER1_ID AS A_ID, A.USER2_ID AS B_ID, A.USER1_ID AS C_ID FROM FVIEW A\n" +
                    "JOIN FVIEW C ON A.USER2_ID = C.USER2_ID AND C.USER1_ID < A.USER1_ID\n" +
                    ")\n" +
                    ")";


            String dropView = "DROP VIEW FVIEW";
            String dropMutualView = "DROP VIEW MUTUALVIEW";
            String mutualFriends = "SELECT A_ID, C_ID FROM MUTUALVIEW\n" +
                    "GROUP BY A_ID, C_ID\n" +
                    "ORDER BY COUNT(B_ID) DESC, A_ID ASC, C_ID ASC";

            String userInfo = "SELECT USER_ID, FIRST_NAME, LAST_NAME FROM " +  UsersTable + " \n" +
                    "WHERE USER_ID = ";



            ResultSet viewst = stmt.executeQuery(view);
            viewst = stmt.executeQuery(mutualView);

            Statement mutualstmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly);
            ResultSet mutualst = mutualstmt.executeQuery(mutualFriends);


            Statement innerstmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly);
            Statement inner2stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly);

            int count = 0;
            while(mutualst.next() && count++ < num)
            {
                long aid = mutualst.getLong(1);
                long cid = mutualst.getLong(2);

                ResultSet rst = innerstmt.executeQuery(
                        "SELECT USER1_ID FROM FVIEW\n" +
                                "WHERE USER1_ID = " + aid + " AND USER2_ID = " + cid
                );
                if(rst.next())
                {
                    count--;
                    continue;
                }


                rst = innerstmt.executeQuery(userInfo + aid);
                rst.first();
                UserInfo u1 = new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3));

                rst = innerstmt.executeQuery(userInfo + cid);
                rst.first();
                UserInfo u2 = new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3));

                UsersPair up = new UsersPair(u1, u2);
                rst = innerstmt.executeQuery(
                        "SELECT B_ID FROM MUTUALVIEW\n" +
                                "WHERE A_ID = " + aid + " AND C_ID = " + cid +
                                "ORDER BY B_ID ASC"
                );
                while(rst.next())
                {
                    long id = rst.getLong(1);

                    ResultSet innerrst = inner2stmt.executeQuery(userInfo + id);
                    while(innerrst.next())
                    {
                        up.addSharedFriend(new UserInfo(innerrst.getLong(1), innerrst.getString(2), innerrst.getString(3)));
                    }

                }
                results.add(up);
            }



            viewst = stmt.executeQuery(dropView);
            viewst = stmt.executeQuery(dropMutualView);

        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    @Override
    // Query 7
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the name of the state or states in which the most events are held
    //        (B) Find the number of events held in the states identified in (A)
    public EventStateInfo findEventStates() throws SQLException {
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                EventStateInfo info = new EventStateInfo(50);
                info.addState("Kentucky");
                info.addState("Hawaii");
                info.addState("New Hampshire");
                return info;
            */

            ResultSet rst = stmt.executeQuery(
                            "SELECT STATE_NAME, COUNT(STATE_NAME) AS NUM FROM " + CitiesTable + " C " +
                                    "JOIN " + EventsTable + " E ON E.EVENT_CITY_ID = C.CITY_ID " +
                                    "GROUP BY STATE_NAME " +
                                    "ORDER BY NUM DESC"
            );

            rst.first();
            long max = rst.getLong(2);
            EventStateInfo info = new EventStateInfo(max);
            info.addState(rst.getString(1));
            while(rst.next())
            {
                if (rst.getLong(2) < max)
                    break;
                info.addState(rst.getString(1));
            }


            return info;               // placeholder for compilation
        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
            return new EventStateInfo(-1);
        }
    }
    
    @Override
    // Query 8
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find the ID, first name, and last name of the oldest friend of the user
    //            with User ID <userID>
    //        (B) Find the ID, first name, and last name of the youngest friend of the user
    //            with User ID <userID>
    public AgeInfo findAgeInfo(long userID) throws SQLException {
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo old = new UserInfo(12000000, "Galileo", "Galilei");
                UserInfo young = new UserInfo(80000000, "Neil", "deGrasse Tyson");
                return new AgeInfo(old, young);
            */
            String nested = "((SELECT USER1_ID AS USER_ID FROM " + FriendsTable + " WHERE USER2_ID = " + userID + ") " +
                            "UNION " +
                            "(SELECT USER2_ID AS USER_ID FROM " + FriendsTable + " WHERE USER1_ID = " + userID + ")" +
                            ")";
            ResultSet rst = stmt.executeQuery("SELECT N.USER_ID, U.FIRST_NAME, U.LAST_NAME, U.YEAR_OF_BIRTH, U.MONTH_OF_BIRTH, U.DAY_OF_BIRTH " +
                                    "FROM " + nested + " N " +
                                    "JOIN " + UsersTable + " U ON N.USER_ID = U.USER_ID " +
                                    "ORDER BY U.YEAR_OF_BIRTH ASC, U.MONTH_OF_BIRTH ASC, U.DAY_OF_BIRTH ASC, U.USER_ID DESC");
            rst.first();
            UserInfo old = new UserInfo(rst.getLong(1), rst.getString(2), rst.getString(3));

            rst.last();

            long userid = rst.getLong(1);
            String first = rst.getString(2);
            String last = rst.getString(3);
            long year = rst.getLong(4);
            long month = rst.getLong(5);
            long day = rst.getLong(6);
            rst.previous();
            while(rst.getLong(4) == year && rst.getLong(5) == month && rst.getLong(6) == day)
            {
                userid = rst.getLong(1);
                first = rst.getString(2);
                last = rst.getString(3);
                rst.previous();
            }
            UserInfo young = new UserInfo(userid, first, last);

            return new AgeInfo(old, young);                // placeholder for compilation
        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
            return new AgeInfo(new UserInfo(-1, "ERROR", "ERROR"), new UserInfo(-1, "ERROR", "ERROR"));
        }
    }
    
    @Override
    // Query 9
    // -----------------------------------------------------------------------------------
    // GOALS: (A) Find all pairs of users that meet each of the following criteria
    //              (i) same last name
    //              (ii) same hometown
    //              (iii) are friends
    //              (iv) less than 10 birth years apart
    public FakebookArrayList<SiblingInfo> findPotentialSiblings() throws SQLException {
        FakebookArrayList<SiblingInfo> results = new FakebookArrayList<SiblingInfo>("\n");
        
        try (Statement stmt = oracle.createStatement(FakebookOracleConstants.AllScroll, FakebookOracleConstants.ReadOnly)) {
            /*
                EXAMPLE DATA STRUCTURE USAGE
                ============================================
                UserInfo u1 = new UserInfo(81023, "Kim", "Kardashian");
                UserInfo u2 = new UserInfo(17231, "Kourtney", "Kardashian");
                SiblingInfo si = new SiblingInfo(u1, u2);
                results.add(si);
            */


            ResultSet rst = stmt.executeQuery(
                        "SELECT F.USER1_ID, U1.FIRST_NAME, U1.LAST_NAME, F.USER2_ID, U2.FIRST_NAME, U2.LAST_NAME FROM  " +
                        "( " +
                        "(SELECT USER1_ID, USER2_ID FROM " + FriendsTable + " WHERE USER1_ID < USER2_ID) " +
                        "UNION " +
                        "(SELECT USER2_ID AS USER1_ID, USER1_ID AS USER2_ID FROM " + FriendsTable + " WHERE USER1_ID > USER2_ID) " +
                        ") " +
                        "F " +
                        "JOIN " + UsersTable + " U1 ON U1.USER_ID = F.USER1_ID " +
                        "JOIN " + UsersTable + " U2 ON U2.USER_ID = F.USER2_ID " +
                        "JOIN " + HometownCitiesTable + " HC1 ON HC1.USER_ID = U1.USER_ID " +
                        "JOIN " + HometownCitiesTable + " HC2 ON HC2.USER_ID = U2.USER_ID " +
                        "WHERE HC1.HOMETOWN_CITY_ID = HC2.HOMETOWN_CITY_ID " +
                        "AND U1.LAST_NAME = U2.LAST_NAME " +
                        "AND ABS(U1.YEAR_OF_BIRTH - U2.YEAR_OF_BIRTH) < 10 " +
                        "ORDER BY F.USER1_ID ASC, F.USER2_ID ASC"
            );
            while(rst.next())
            {
                UserInfo u1 = new UserInfo(rst.getLong(1), rst.getString(2),rst.getString(3));
                UserInfo u2 = new UserInfo(rst.getLong(4), rst.getString(5), rst.getString(6));
                results.add(new SiblingInfo(u1, u2));
            }


        }
        catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        
        return results;
    }
    
    // Member Variables
    private Connection oracle;
    private final String UsersTable = FakebookOracleConstants.UsersTable;
    private final String CitiesTable = FakebookOracleConstants.CitiesTable;
    private final String FriendsTable = FakebookOracleConstants.FriendsTable;
    private final String CurrentCitiesTable = FakebookOracleConstants.CurrentCitiesTable;
    private final String HometownCitiesTable = FakebookOracleConstants.HometownCitiesTable;
    private final String ProgramsTable = FakebookOracleConstants.ProgramsTable;
    private final String EducationTable = FakebookOracleConstants.EducationTable;
    private final String EventsTable = FakebookOracleConstants.EventsTable;
    private final String AlbumsTable = FakebookOracleConstants.AlbumsTable;
    private final String PhotosTable = FakebookOracleConstants.PhotosTable;
    private final String TagsTable = FakebookOracleConstants.TagsTable;
}
