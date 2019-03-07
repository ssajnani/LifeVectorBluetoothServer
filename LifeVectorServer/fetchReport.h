/**
 *@file fetchReport.h
 *@brief creates and fetches the report to send to the front end
 */
#ifndef FETCHREPORT_H
#define FETCHREPORT_H

#include "json.hpp"
#include <iostream>
#include <unistd.h>
#include <string>
#include "StringParser.h"
#include <sstream> 
#include <vector>

#include "Database.h"
#include "UserController.h"
#include "VisitLog.h"

#include "json.hpp"

#include "UserVisitInfo.h"
#include "LocationInformation.h"
#include "CoordinateInformation.h"
#include "ArchivedLocation.h"
#include "ArchiveLibrary.h"

#include "googleAPI.h"

#include <iostream>
#include <utility>
#include <cmath>
#include <stdlib.h>

/**
 *@brief 
 */

class fetchReport
{
  private:

    nlohmann::json report;

    Database *connect_db;

  public:
  /**
   *@brief constructor
   */
  fetchReport();   
  /**
   *@brief destructor
   */
  ~fetchReport();

  /**
   *@brief writes to an external report json file with location information for the user
   *@param visitL pointer to the visit log to get time spent and visits
   *@param db_connect a point to the database
   *@param loc_id
   */
  void process(VisitLog *visitL, Database *db_connect, int loc_id);
  /**
   *@brief get the report if needed
   *@return returns the json report
   */
  nlohmann::json getReport();

  /**
   *@brief print the report to the console
   *
   */
  void printReport();



};

#endif

