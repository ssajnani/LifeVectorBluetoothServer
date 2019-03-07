/**
 * @file Database.h
 * @brief Read and write database
 * 
 * @author Yimin Xu (yxu537@uwo.ca)
 * 
 * If mysql.h is not installed, "sudo apt-get install libmysqlclient-dev"
 * If that command didn't work, try "sudo apt-get install default-libmysqlclient-dev"
 * Need to add "-lmysqlclient" when compiling
 */

#ifndef DATABASE_H
#define DATABASE_H
#include<iostream>
#include<string>
#include<mysql/mysql.h> 

using namespace std;

/**
 * @brief Read and write database
 * 
 */
class Database
{
public:
	/**
	 * @brief Construct a new Database object
	 * 
	 */
	Database();

	/**
	 * @brief Destroy the Database object
	 * 
	 */
	~Database();

	/**
	 * @brief Initiate database
	 * 
	 * @param host MySQL hostname
	 * @param user MySQL username
	 * @param pw MySQL password
	 * @param db_name Database name
	 * @return true Connected
	 * @return false Connection failed
	 */
	bool initDB(string host, string user, string pw, string db_name);

	/**
	 * @brief Execute SQL without returning information
	 * 
	 * @param sql SQL query
	 * @return true Query OK
	 * @return false Query failed
	 */
	bool exeSQL(string sql);

	/**
	 * @brief Execute SQL with returning information
	 * 
	 * @param sql SQL query
	 * @return std::string Returning data from database
	 */
    std::string getSQLResult(string sql);
private:
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
};

#endif
