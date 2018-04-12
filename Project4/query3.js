//query3
//create a collection "cities" to store every user that lives in every city
//Each document(city) has following schema:
/*
{
  _id: city
  users:[userids]
}
*/

function cities_table(dbname) {
    db = db.getSiblingDB(dbname);
    // TODO: implemente cities collection here
    /*db.users.mapReduce(
	function() { emit( this.hometown.city, this.user_id ); },
	function(key, values){
	    var users_arr = [];
	    values.forEach(function(v){
		Array.prototype.push.apply(users_arr, v.user_id);
	    });
	    return {users: users_arr};
	},
	{
	 query: {},
	 out: "cities"
	}
    );*/
    db.users.aggregate([
	{
	    "$group": {
		"_id": "$hometown.city",
		"users": {
		    "$push": "$user_id"
		}
	    }
	},
	{
	    "$project": {
		"_id" : 1, "users" : 1
	    }
	},
	{
	    "$out" : "cities"
	}
    ]);
	
    //db.cities.update({},
//	{$rename: {'value' : 'users'}});
    // Returns nothing. Instead, it creates a collection inside the datbase.

}
