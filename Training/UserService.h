#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>

class UserService {
public:
    static std::string ShowAllUsers();
    static std::string CreateUser(const std::string& userName);
    static std::string RemoveUser(int userId);
};

#endif // USER_SERVICE_H
