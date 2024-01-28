#include "mysql_connector.h"

int check_account(std::string username, std::string password, mysqlx::Table& connections_table) {
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

void update_account(std::string username, std::string ip_addr, int port, mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("IP_Address", ip_addr)
                     .set("Port", port)
                     .where("Username like :username")
                     .bind("username", username).execute();
}

void create_account(std::string username, std::string password, mysqlx::Table& connections_table) {
    connections_table.insert("Username", "Password")
                     .values(username, password)
                     .execute();            
}

void unbind_account(std::string username, mysqlx::Table& connections_table) {
    connections_table.update()
                     .set("IP_Address", NULL)
                     .set("Port", NULL)
                     .where("Username like :username")
                     .bind("username", username).execute();
}

std::pair<std::string, int> get_account_endpoint(std::string username, mysqlx::Table& connections_table) {
    mysqlx::RowResult results = connections_table.select("IP_Address", "Port")
                                                 .where("Username like :username")
                                                 .bind("username", username).execute();

    mysqlx::Row row = results.fetchOne();
    std::pair<std::string, int> ip_port;
    
    ip_port.second = row[1];
    mysqlx::string str = row[0].get<mysqlx::string>();
    ip_port.first = str;

    std::cout << ip_port.first << " : " << ip_port.second << std::endl;

    return ip_port;
}