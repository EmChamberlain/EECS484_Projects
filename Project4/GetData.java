import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.TreeSet;
import java.util.Vector;
import org.json.JSONObject;
import org.json.JSONArray;

public class GetData{

	// DO NOT modify the following variable names
    static String prefix = "hewen.";

    Connection oracleConnection = null;

    String cityTableName = null;
    String userTableName = null;
    String friendsTableName = null;
    String currentCityTableName = null;
    String hometownCityTableName = null;
    String programTableName = null;
    String educationTableName = null;
    String eventTableName = null;
    String participantTableName = null;
    String albumTableName = null;
    String photoTableName = null;
    String coverPhotoTableName = null;
    String tagTableName = null;

    // This is the data structure to store all users' information
    JSONArray users_info = new JSONArray();	

	
    // DO NOT modify this constructor
    public GetData(String u, Connection c) {
	super();
	String dataType = u;
	oracleConnection = c;
	cityTableName = prefix+dataType+"_CITIES";
	userTableName = prefix+dataType+"_USERS";
	friendsTableName = prefix+dataType+"_FRIENDS";
	currentCityTableName = prefix+dataType+"_USER_CURRENT_CITIES";
	hometownCityTableName = prefix+dataType+"_USER_HOMETOWN_CITIES";
	programTableName = prefix+dataType+"_PROGRAMS";
	educationTableName = prefix+dataType+"_EDUCATION";
	eventTableName = prefix+dataType+"_USER_EVENTS";
	albumTableName = prefix+dataType+"_ALBUMS";
	photoTableName = prefix+dataType+"_PHOTOS";
	tagTableName = prefix+dataType+"_TAGS";
    }
	

    @SuppressWarnings("unchecked")
    public JSONArray toJSON() throws SQLException{ 
		
	// TODO: query USERS 
	// for each user, query FRIENDS and form a list
    // query user cities information
    // populate 'user_info' with 800 JSONObject(s)

	//Connection conn = new Connection();	
	/* BEGIN of example */	
	// Example usage of JSONArray and JSONObject
	// The JSONArray user_info contains a list of JSONObjects
	// All user information should be stored in user_info
	// You will need to REMOVE the following:
	Statement st = oracleConnection.createStatement();
	Statement fst = oracleConnection.createStatement();
	/*String q = "SELECT U1.USER_ID, U1.FIRST_NAME, U1.LAST_NAME, U1.GENDER,  "+
		   "FROM USERS U1, FRIENDS F, USERS U2 " +
 		   "WHERE (U1.USER_ID = F.USER1_ID AND U2.USER_ID = F.USER2_ID) " +
		   "OR (U1.USER_ID NOT IN (" +
		   "SELECT USER1_ID " +
		   "FROM FRIENDS " +
		   ") AND U1.USER_ID NOT IN (" +
		   "SELECT USER2_ID " +
		   "FROM FRIENDS " +
		   "))";*/
	String allUsers = "" +
"SELECT U.USER_ID, U.FIRST_NAME, U.LAST_NAME, U.GENDER, U.YEAR_OF_BIRTH, "+
"U.MONTH_OF_BIRTH, U.DAY_OF_BIRTH, C.CITY_NAME, C.STATE_NAME, C.COUNTRY_NAME "+
"FROM " + userTableName + " U INNER JOIN " + hometownCityTableName + " H ON U.USER_ID = H.USER_ID INNER JOIN " + cityTableName + " C ON C.CITY_ID = H.HOMETOWN_CITY_ID ";
	ResultSet rs = st.executeQuery(allUsers);
	//JSONArray all_users_info = new JSONArray();
	JSONObject user_info = new JSONObject();
	while(rs.next()) {
		user_info = new JSONObject();
		user_info.put("user_id", rs.getLong(1));
		user_info.put("first_name", rs.getString(2));
		user_info.put("last_name", rs.getString(3));
		user_info.put("gender", rs.getString(4));
		user_info.put("YOB", rs.getLong(5));
		user_info.put("MOB", rs.getLong(6));
		user_info.put("DOB", rs.getLong(7));
		JSONObject hometown = new JSONObject();
		hometown.put("city", rs.getString(8));
		hometown.put("state", rs.getString(9));
		hometown.put("country", rs.getString(10));
		user_info.put("hometown", hometown);
		String friends = "" +
"SELECT USER2_ID " +
"FROM " + friendsTableName + " " +
"WHERE USER1_ID = " + rs.getString(1);
		ResultSet frs = fst.executeQuery(friends);
		JSONArray friends_info = new JSONArray();
		while(frs.next()) {
			friends_info.put(frs.getLong(1));
		}
		user_info.put("friends", friends_info);
		users_info.put(user_info);
	}
	// A JSONObject is an unordered collection of name/value pairs. Add a few name/value pairs.
	//JSONObject test = new JSONObject();	// declare a new JSONObject
	// A JSONArray consists of multiple JSONObjects. 
	//JSONArray users_info = new JSONArray();

	//test.put("user_id", "testid");		// populate the JSONObject
	//test.put("first_name", "testname");

	//JSONObject test2 = new JSONObject();
	//test2.put("user_id", "test2id");
	//test2.put("first_name", "test2name");

	//users_info.put(test);		// add the JSONObject to JSONArray     
	//users_info.put(test2);		// add the JSONObject to JSONArray	
	/* END of example */

	return users_info;
    }
	

	// DO NOT MODIFY this function
    // This outputs to a file "output.json"
    public void writeJSON(JSONArray users_info) {
	try {
	    FileWriter file = new FileWriter(System.getProperty("user.dir")+"/output.json");
	    file.write(users_info.toString());
	    file.flush();
	    file.close();
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }
}

