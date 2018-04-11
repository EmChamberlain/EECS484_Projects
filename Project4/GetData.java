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

		
	/* BEGIN of example */	
	// Example usage of JSONArray and JSONObject
	// The JSONArray user_info contains a list of JSONObjects
	// All user information should be stored in user_info
	// You will need to REMOVE the following:

	// A JSONObject is an unordered collection of name/value pairs. Add a few name/value pairs.
	JSONObject test = new JSONObject();	// declare a new JSONObject
	// A JSONArray consists of multiple JSONObjects. 
	JSONArray users_info = new JSONArray();

	test.put("user_id", "testid");		// populate the JSONObject
	test.put("first_name", "testname");

	JSONObject test2 = new JSONObject();
	test2.put("user_id", "test2id");
	test2.put("first_name", "test2name");

	users_info.put(test);		// add the JSONObject to JSONArray     
	users_info.put(test2);		// add the JSONObject to JSONArray	
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

