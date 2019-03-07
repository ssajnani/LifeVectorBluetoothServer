CC=g++
CFLAGS=-c -w
CXXFLAGS=$(CFLAGS)
CMATH=-lm
PFLAG=-lmysqlclient
PFLAG2=-L/usr/include/curl/lib -lcurl
PFLAG3=-lcrypto
OBJ = *.o

# Compile All Files and Run All Tests
all: main TestUC TestArchive TestSquasher TestReport
	@echo Compile and Run All Tests - Successful!

# TESTS EXE GENERATOR #

# Test for User Controller
UCTest: ./Test_Runners/UserControllerTest.o $(OBJ)
	$(CC) -o $@ $^ $(CMATH) $(PFLAG) $(PFLAG2) $(PFLAG3)

# Test for Report Generation
ReportTest: ./Test_Runners/fetchTest.cpp $(OBJ)
	$(CC) -o $@ $^ $(CMATH) $(PFLAG) $(PFLAG2) $(PFLAG3)

# Test for Archive Library and Archive Classes
ArchLibTest: ./Test_Runners/ALibTest.o $(OBJ)
	$(CC) -o $@ $^ $(CMATH) $(PFLAG) $(PFLAG2) $(PFLAG3)

# Test the Squashing Class and algorithm
SquasherTest: ./Test_Runners/TestSquash.o $(OBJ)
	$(CC) -o $@ $^ $(CMATH) $(PFLAG) $(PFLAG2) $(PFLAG3)


# Compile All .cpp files into main.o 
main: VisitLog.o UserVisitInfo.o LocationInformation.o CoordinateInformation.o ArchivedLocation.o ArchiveLibrary.o SystemTimeManager.o EncryptionModule.o CURLplusplus.o placesAPI.o googleAPI.o RawDataRepository.o squasher.o md5.o DataDepositor.o Database.o UserController.o hashfunc.o UserLibrary.o user.o fetchReport.o
	@echo All .cpp Files Compiled!

UserControllerTest.o: ./Test_Runners/UserControllerTest.cpp
	$(CC) $(CFLAGS) -o $@ $<

TestSquash.o: ./Test_Runners/TestSquash.cpp
	$(CC) $(CFLAGS) -o $@ $<

ALibTest.o: ./Test_Runners/ALibTest.cpp
	$(CC) $(CFLAGS) -o $@ $<

fetchTest.o: ./Test_Runners/fetchTest.cpp
	$(CC) $(CFLAGS) -o $@ $<



# Individual .cpp Compilations

fetchReport.o: fetchReport.cpp
	$(CC) $(CFLAGS) -o $@ $<

user.o: user.cpp
	$(CC) $(CFLAGS) -o $@ $<

UserLibrary.o: UserLibrary.cpp
	$(CC) $(CFLAGS) -o $@ $<

hashfunc.o: hashfunc.cpp
	$(CC) $(CFLAGS) -o $@ $<

UserController.o: UserController.cpp
	$(CC) $(CFLAGS) -o $@ $<

Database.o: Database.cpp
	$(CC) $(CFLAGS) -o $@ $<

DataDepositor.o: DataDepositor.cpp
	$(CC) $(CFLAGS) -o $@ $<

md5.o: md5.cpp
	$(CC) $(CFLAGS) -o $@ $<

squasher.o: squasher.cpp
	$(CC) $(CFLAGS) -o $@ $<

RawDataRepository.o: RawDataRepository.cpp
	$(CC) $(CFLAGS) -o $@ $<

googleAPI.o: googleAPI.cpp
	$(CC) $(CFLAGS) -o $@ $< 	

placesAPI.o: placesAPI.cpp
	$(CC) $(CFLAGS) -o $@ $< 	

CURLplusplus.o: CURLplusplus.cpp
	$(CC) $(CFLAGS) -o $@ $< 	

EncryptionModule.o: EncryptionModule.cpp
	$(CC) $(CFLAGS) -o $@ $<

SystemTimeManager.o: SystemTimeManager.cpp
	$(CC) $(CFLAGS) -o $@ $<

ArchivedLocation.o: ArchivedLocation.cpp 
	$(CC) $(CFLAGS) -o $@ $<

ArchiveLibrary.o: ArchiveLibrary.cpp
	$(CC) $(CFLAGS) -o $@ $<

CoordinateInformation.o: CoordinateInformation.cpp
	$(CC) $(CFLAGS) -o $@ $<

LocationInformation.o: LocationInformation.cpp
	$(CC) $(CFLAGS) -o $@ $<

UserVisitInfo.o: UserVisitInfo.cpp
	$(CC) $(CFLAGS) -o $@ $<

VisitLog.o: VisitLog.cpp
	$(CC) $(CFLAGS) -o $@ $<

# Test Executables

TestArchive: ArchLibTest
	@echo Running Execution to Test Archive Library Files
	./ArchLibTest

TestUC: UCTest
	@echo Running User Controller Tests For Functionality
	./UCTest

TestSquasher: SquasherTest
	@echo Running Test of Squasher Algorithm
	./SquasherTest

TestReport: ReportTest
	@echo Running Test for Generating Sample Report for Front End
	./ReportTest

# Cleanses Files and Recompiles All Files
test: clean main
	@echo ALL .cpp FILES COMPILED SUCCESSFULLY

# Cleanses Folder of All Generated .o and executables
clean:
	rm -f *.o Test_Runners/*.o main UCTest ReportTest ArchLibTest SquasherTest
	@echo All Files Cleansed 
