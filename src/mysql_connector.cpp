#include "mysql_connector.h"

int check_u_p(std::string username, std::string password, sql::Connection *con) {
    sql::Statement *stmt;
    sql::ResultSet *res;

    stmt = con->createStatement();
    stmt->execute("USE db");

    sql::SQLString str;
    str.append("SELECT Username, Password FROM connections WHERE (Username=");
    str.append("\'");
    str.append(username);
    str.append("\' ");
    str.append("and ");
    str.append("Password=");
    str.append("\'");
    str.append(password);
    str.append("\')");
    // std::cout << str << std::endl;

    res = stmt->executeQuery(str);

    while (res->next()) {
        if (res->isNull("Username") || res->isNull("Password")) {
            return false;
        }
        else {
            // std::cout << res->getString("Username") << std::endl;
            // std::cout << res->getString("Password") << std::endl;
            return true;
        }
    }
    return false;
}