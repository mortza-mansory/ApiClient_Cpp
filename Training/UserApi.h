#ifndef USER_API_H
#define USER_API_H

#include <string>

class UserApi {
public:
    static std::string GetUsers();
    static std::string AddUser(const std::string& userName);
    static std::string DeleteUser(int userId);
};

#endif // USER_API_H
