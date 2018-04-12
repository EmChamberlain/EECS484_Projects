
// query 4: find user pairs (A,B) that meet the following constraints:
// i) user A is male and user B is female
// ii) their Year_Of_Birth difference is less than year_diff
// iii) user A and B are not friends
// iv) user A and B are from the same hometown city
// The following is the schema for output pairs:
// [
//      [user_id1, user_id2],
//      [user_id1, user_id3],
//      [user_id4, user_id2],
//      ...
//  ]
// user_id is the field from the users collection. Do not use the _id field in users.
  
function suggest_friends(year_diff, dbname) {
    db = db.getSiblingDB(dbname);
    var pairs = [];
    // TODO: implement suggest friends
    // Return an array of arrays.
    var users = db.users.find().toArray();
    users.forEach(function(u){
	if (u.gender == "male"){
	    
	    matches = db.users.find({gender : "female"});
	    matches.forEach(function(m){
            var year = Math.abs(m.YOB - u.YOB) < year_diff;
            var friends = u.friends.includes(m.user_id) || m.friends.includes(u.user_id);
            var hometown = u.hometown.city == m.hometown.city;
            if(year && !friends && hometown)
            {
                pairs.push([u.user_id, m.user_id]);
            }
            
	    });
	}
    });
    return pairs;
}
