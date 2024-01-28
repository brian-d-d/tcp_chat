#include "mysql_connector.h"

// int check_u_p(std::string username, std::string password, mysqlx::Table& connections_table) {
//     sql::Statement *stmt;
//     sql::ResultSet *res;

//     stmt = con->createStatement();
//     stmt->execute("USE db");

//     sql::SQLString str;
//     str.append("SELECT Username, Password FROM connections WHERE (Username=");
//     str.append("\'");
//     str.append(username);
//     str.append("\' ");
//     str.append("and ");
//     str.append("Password=");
//     str.append("\'");
//     str.append(password);
//     str.append("\')");
//     // std::cout << str << std::endl;

//     res = stmt->executeQuery(str);

//     while (res->next()) {
//         if (res->isNull("Username") || res->isNull("Password")) {
//             delete stmt;
//             delete res;
//             return false;
//         }
//         else {
//             // std::cout << res->getString("Username") << std::endl;
//             // std::cout << res->getString("Password") << std::endl;
//             delete stmt;
//             delete res;
//             return true;
//         }
//     }
//     delete stmt;
//     delete res;
//     return false;
// }

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
    return 1;
}