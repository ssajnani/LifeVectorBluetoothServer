/** 
 * @brief creates a database runny mysql and gives methods for insertion and queries
 */
#include <iostream>
#include <cstdlib>
#include "Database.h"
#include <typeinfo>
#include <sstream>
using namespace std;

/** 
 * @brief constructor for database class
 */
Database::Database()
{
	connection = mysql_init(NULL);
	if (connection == NULL)
	{
		cout << "Object Initialization Error:" << mysql_error(connection);
		exit(1);
	}
}
/** 
 * @brief destructor for database class
 */
Database::~Database()
{
	if (connection != NULL)
	{
		mysql_close(connection);
	}
}
/** 
 * @brief connects to the databse
 * @param host the name of the host
 * @param user the name of the user
 * @param pwd the password for that user
 * @param db_name the name of the database
 * @return true if a connection can be made otherwise return error message and exit
 */
bool Database::initDB(string host, string user, string pwd, string db_name)
{
	// Build a connection with database
	connection = mysql_real_connect(connection, host.c_str(),
									user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
	if (connection == NULL)
	{
		cout << "Connection Initialization Error:" << mysql_error(connection) << endl;
		exit(1);
	}
	return true;
}
/** 
 * @brief queries the database
 * @param a string representing an sql query
 * @return returns true if a query is successful otherwise print an error message and exit
 */
bool Database::exeSQL(string sql)
{
	if (mysql_query(connection, sql.c_str()))
	{
		cout << "Query Execution Error:" << mysql_error(connection) << endl;
		exit(1);
	}
	else
	{
		result = mysql_use_result(connection); // Get Result
		// mysql_field_count() return number of rows
		for (int i = 0; i < mysql_field_count(connection); ++i)
		{
			// Get next row
			row = mysql_fetch_row(result);
			if (row <= 0)
				break;
			// mysql_num_fields return number of columns
			for (int j = 0; j < mysql_num_fields(result); ++j)
			{
				cout << row[j] << " ";
			}
			cout << endl;
		}
		// Free memory
		mysql_free_result(result);
	}
	return true;
}
/** 
 * @brief gets a result from the database
 * @param a string representing an sql query
 * @return returns the result of the query if it's successful otherwise print and error and exit
 */
std::string Database::getSQLResult(string sql)
{
	stringstream ss;

	if (mysql_query(connection, sql.c_str()))
	{
		ss << "Result Query Error:" << mysql_error(connection) << endl;
		return ss.str();
	}
	else
	{
		result = mysql_use_result(connection); // Get Result
		// mysql_field_count() return number of rows
		for (int i = 0; i < mysql_field_count(connection); ++i)
		{
			// Get next row
			row = mysql_fetch_row(result);
			if (row <= 0)
				break;
			// mysql_num_fields return number of columns
			for (int j = 0; j < mysql_num_fields(result); ++j)
			{
				ss << row[j] << "\t";
			}
			ss << endl;
		}
		// Free memory
		mysql_free_result(result);
	}
	return ss.str();
}

