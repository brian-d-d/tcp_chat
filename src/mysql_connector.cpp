#include "mysql_connector.h"

int check_account(std::string username, std::string password, mysqlx::Table& connections_table) {
    mysqlx::RowResult Results = connections_table.select("Username", "Password")
                                                 .where("Username like :username AND Password like :password")
                                                 .bind("username", username).bind("password", password).execute();

    mysqlx::Row row = Results.fetchOne();

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