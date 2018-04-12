compile:
	javac -Xlint:-unchecked -cp "ojdbc6.jar:json-20151123.jar:json_simple-1.1.jar:" Main.java GetData.java

run:
	@echo "Running the Java program to create the JSON file. "
	@echo "You must be on the university VPN or network. "
	@echo "Also check your username and password is correct in Main.java"
	@echo ""
	java -cp "ojdbc6.jar:json-20151123.jar:json_simple-1.1.jar:" Main
	@echo "An output file output.json should be created if everything ran fine."

setupsampledb:
	@echo "You must edit the following line in Makefile to correct the uniqname and password"
	@echo "You may need to run 'module load mongodb' as well on CAEN."
	mongoimport --host eecs484.eecs.umich.edu --username cjscavon --password Password123  --collection users --db cjscavon --file  sample.json --jsonArray

setupmydb:
	@echo "You must edit the following line in Makefile to correct the uniqname and password"
	@echo "You may need to run 'module load mongodb' as well on CAEN."
	mongoimport --host eecs484.eecs.umich.edu --username cjscavon --password Password123  --collection users --db cjscavon --file  output.json --jsonArray

mongoquerytest:
	@echo "Running test.js using the database. Run make setupsampledb or make setupmydb before this."
	@echo "You must edit the following line in Makefile to correct the uniqname and password"
	@echo "You may need to run 'module load mongodb' as well on CAEN."
	mongo cjscavon -u cjscavon -p Password123 --host eecs484.eecs.umich.edu < test.js
	echo "Local tests in test.js have been run."
	@echo "Optional: Do make dropdatabase to remove the mongo database."

dropdatabase:
	@echo "You must edit the following line in Makefile to correct the uniqname and password"
	mongo cjscavon -u cjscavon -p Password123 --host eecs484.eecs.umich.edu --eval "db.users.drop()"

submit:
	tar -zcf project4.tar.gz GetData.java query1.js query2.js query3.js query4.js query5.js query6.js query7.js
	@echo  "Submit your project at https://grader484.eecs.umich.edu"
