#include <iostream>
#include <string>
#include "Database.h"
#include "Routes.h"
#include "HttpHelper.h"

int main() {
    std::string serverIP = "127.0.0.1";
    int serverPort = 8080;

    Database::InitializeDatabase();

    HttpHelper::start(serverIP, serverPort, startRoutes);

    return 0;
}
