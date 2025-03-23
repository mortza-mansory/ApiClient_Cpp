#include "UserService.h"
#include "UserApi.h"

std::string UserService::ShowAllUsers() {
    return UserApi::GetUsers();
}

std::string UserService::CreateUser(const std::string& userName) {
    return UserApi::AddUser(userName);
}

std::string UserService::RemoveUser(int userId) {
    return UserApi::DeleteUser(userId);
}
