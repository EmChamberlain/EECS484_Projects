// find the oldest friend for each user who has a friend. 
// For simplicity, use only year of birth to determine age, if there is a tie, use the one with smallest user_id
// return a javascript object : key is the user_id and the value is the oldest_friend id
// You may find query 2 and query 3 helpful. You can create selections if you want. Do not modify users collection.
//
//You should return something like this:(order does not matter)
//{user1:userx1, user2:userx2, user3:userx3,...}

function oldest_friend(dbname){
  db = db.getSiblingDB(dbname);
  var results = {};
  // TODO: implement oldest friends
  // return an javascript object described above
  db.users.aggregate([
	{$unwind : "$friends"},
	{$project: {user_id : 1, friends : 1, _id : 0}},
	{$out : "flat_users"}
    ]);
  var users = db.users.find();
  users.forEach(function(u){
    friends = u.friends;
    var fs = db.flat_users.find({friends : u.user_id});
    fs.forEach(function(f){
        friends.push(f.user_id);
    });
    if (friends.length == 0)
    {
        return;
    }
    var oldest = friends[0];
    var oldest_year = -1;
	for (friend_id of friends)
    {
       var f = db.users.findOne({user_id : friend_id});
       if (f.YOB < oldest_year || oldest_year == -1)
       {
           oldest = friend_id;
           oldest_year = f.YOB;
       }
       if (f.YOB == oldest_year && oldest > f.user_id)
       {
           oldest = friend_id;
           oldest_year = f.YOB;
       }
    }
    results[u.user_id] = oldest;
    
  });
  
  return results
}
