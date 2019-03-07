1. 	Initialize Server Environment:
	- cd to LifeVectorServer directory
	- run install.sh to setup all required packages for Environment 

2. To run tests on server to database connectors
	- run 'make' to compile all .cpp files
	- make test - cleanses .o and executables and recompiles all .cpp to .of
	- make all - compiles all .cpp files and executes all test cases present
		- Tests Run:
			- Test User Controller (UCTest)
			- Test Archive Classes (ArchLibTest)
			- Test Squasher Algorithm (SquasherTest) 
			- Test Report Tests (ReportTest)

		- to run them individually
			- make TestUC
			- make TestArchive
			- make TestSquasher
			- make TestReport







