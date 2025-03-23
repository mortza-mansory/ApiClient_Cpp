#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
public:
    static void InitializeDatabase();
    static std::string GetUsers();
    static void AddUser(const std::string& userName);
    static bool DeleteUser(int userId);
};

#endif // DATABASE_H
