// query6 : Find the Average friend count per user for users
//
// Return a decimal variable as the average user friend count of all users
// in the users document.

function find_average_friendcount(dbname){
  db = db.getSiblingDB(dbname)
  // TODO: return a decimal number of average friend count
  var numFriends = 0;
  var numUsers = 0;
  var users = db.users.find();
  users.forEach(function(u){
	numUsers++;
    numFriends += u.friends.length    
    });
  return numFriends/numUsers;
}
