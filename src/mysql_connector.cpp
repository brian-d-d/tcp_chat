#include "mysql_connector.h"

int check_u_p(std::string username, std::string password, mysqlx::Table& connections_table) {
    mysqlx::RowResult Results = connections_table.select("Username", "Password").
                               where("Username like :username AND Password like :password").
                               bind("username", username).bind("password", password).execute();

    mysqlx::Row row = Results.fetchOne();

    if (row.isNull()) {
        return false;
    }
    else {
        return true;
    }
}