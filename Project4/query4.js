
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
    var users = db.users.find({}).toArray();
    users.forEach(function(u){
	if (u.gender == "male"){
	    var up_tol = u.yob + year_diff;
	    var low_tol = u.yob - year_diff;
	    var matches = [];
	    matches = db.users.find({
		gender : "female", 
		user_id : { 
		    $nin : u.friends
		},
		hometown.city : u.hometown.city,
		yob : {
		    $lt : up_tol,
		    $gt : low_tol
		}
	    }).toArray();
	    var match = []
	    matches.forEach(function(m){
		Array.prototype.push.apply(match, u.user_id);
		Array.prototype.push.apply(match, m.user_id);
		Array.prototype.push.apply(pairs, u.user_id); 
	    });
	}
    });
    return pairs;
}
