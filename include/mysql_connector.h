#ifndef __MYSQL_CONNECTOR_H
#define __MYSQL_CONNECTOR_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>

#include <connecter_cpp/mysqlx/xdevapi.h>

int check_account(std::string username, std::string password, mysqlx::Table& connections_table);

void update_account(std::string username, std::string ip_addr, int port, mysqlx::Table& connections_table);

void create_account(std::string username, std::string password, mysqlx::Table& connections_table);

#endif
