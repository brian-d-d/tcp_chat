#ifndef __MYSQL_CONNECTOR_H
#define __MYSQL_CONNECTOR_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>

#include "boost/asio.hpp"
#include <connecter_cpp/mysqlx/xdevapi.h>

int check_login_details(std::string username, std::string password, mysqlx::Table& connections_table);

std::pair<std::string, int> get_account_endpoint(std::string username, mysqlx::Table& connections_table);

void bind_account(std::string username, std::string ip_addr, int port, mysqlx::Table& connections_table);

void unbind_account(std::string username, mysqlx::Table& connections_table);

int create_account(std::string username, std::string password, mysqlx::Table& connections_table);

void unbind_all_accounts(mysqlx::Table& connections_table);

void delete_account(std::string username, mysqlx::Table& connections_table);

void change_password(std::string username, std::string password, mysqlx::Table& connections_table);

#endif
