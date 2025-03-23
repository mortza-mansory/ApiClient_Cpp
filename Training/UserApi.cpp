#include "UserApi.h"
#include "Database.h"

std::string UserApi::GetUsers() {
    return Database::GetUsers();
}

std::string UserApi::AddUser(const std::string& userName) {
    Database::AddUser(userName);
    return Database::GetUsers();
}

std::string UserApi::DeleteUser(int userId) {
    bool success = Database::DeleteUser(userId);
    if (success)
        return Database::GetUsers();
    else
        return "{\"error\": \"User not found\"}";
}
