// query 7: Find the city average friend count per user using MapReduce
// Using the same terminology in query6, we are asking you to write the mapper,
// reducer and finalizer to find the average friend count for each city.


var city_average_friendcount_mapper = function() {
  // implement the Map function of average friend count
  var ret = {length : this.friends.length, num : 1};
  emit(this.hometown.city, ret);
};

var city_average_friendcount_reducer = function(key, values) {
  // implement the reduce function of average friend count
  var result = {length : 0, num : 0};
  for (value of values)
  {
      result.length += value.length;
      result.num += value.num;
  }
  return result;
};

var city_average_friendcount_finalizer = function(key, reduceVal) {
  // We've implemented a simple forwarding finalize function. This implementation 
  // is naive: it just forwards the reduceVal to the output collection.
  // Feel free to change it if needed. However, please keep this unchanged:
  // the var ret should be the average friend count per user of each city.
  var ret = reduceVal.length / reduceVal.num;
  return ret;
}
