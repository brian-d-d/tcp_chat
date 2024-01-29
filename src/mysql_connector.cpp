#include "mysql_connector.h"

int check_login_details(std::string username, std::string password, mysqlx::Table& connections_table) {
    mysqlx::RowResult results = connections_table.select("Username", "Password")
                                                 .where("Username like :username AND Password like :password")
                                                 .bind("username", username).bind("password", password).execute();

    mysqlx::Row row = results.fetchOne();

    if (row.isNull()) {
        return false;
    }
    else {
        return true;
    }
}

std::pair<std::string, int> get_account_endpoint(std::string username, mysqlx::Table& connections_table) {
    mysqlx::RowResult results = connections_table.select("IP_Address", "Port", "Logged_In")
                                                 .where("Username like :username")
                                                 .bind("username", username).execute();

    mysqlx::Row row = results.fetchOne();

    std::pair<std::string, int> ip_port;

    //The second statement is getting column 2 (Logged_In) and then getting the first byte which is a boolean
    //So it checks if that username is logged in or not
    if (row.isNull() || !(row.getBytes(2)).first[0]) {
        ip_port.first = "NULL";
    }
    else {
        ip_port.second = row[1];
        mysqlx::string str = row[0].get<mysqlx::string>();
        ip_port.first = str;
        std::cout << ip_port.first << " : " << ip_port.second << std::endl;
    }

    return ip_port;
}

int create_account(std::string username, std::string password, mysqlx::Table& connections_table) {
    mysqlx::RowResult results = connections_table.select("Username")
                                                 .where("Username like :username")
                                                 .bind("username", username).execute();

    mysqlx::Row row = results.fetchOne();
        
    if (row.isNull()) {
        connections_table.insert("Username", "Password")
                         .values(username, password).execute(); 
        return true;   
    }            
    return false;
}

void bind_account(std::string username, std::string ip_addr, int port, mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("IP_Address", ip_addr)
                     .set("Port", port)
                     .set("Logged_In", true)
                     .where("Username like :username")
                     .bind("username", username).execute();
}

void unbind_account(std::string username, mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("IP_Address", NULL)
                     .set("Port", NULL)
                     .set("Logged_In", false)
                     .where("Username like :username")
                     .bind("username", username).execute();
}

void unbind_all_accounts(mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("IP_Address", NULL)
                     .set("Port", NULL)
                     .set("Logged_In", false).execute();
}

void delete_account(std::string username, mysqlx::Table& connections_table) {
    connections_table.remove()
                     .where("Username like :username")
                     .bind("username", username).execute();
}

void change_password(std::string username, std::string password, mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("Password", password)
                     .where("Username like :username")
                     .bind("username", username).execute();
}