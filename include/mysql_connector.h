#ifndef __MYSQL_CONNECTOR_H
#define __MYSQL_CONNECTOR_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
  g++ main.cpp -std=c++11 -o framework -lmysqlcppconn
*/
#include "cppconn/mysql_connection.h"
#include "cppconn/mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

int check_u_p(std::string username, std::string password, sql::Connection *con);

#endif
