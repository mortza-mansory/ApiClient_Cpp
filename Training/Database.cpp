#include "Database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>

static std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

struct User {
    int id;
    std::string name;
};

// this can parse users from JSON using regex.
 static std::vector<User> parseUsers(const std::string& json) {
    std::vector<User> users;
    std::regex userRegex("\\{\\s*\"id\"\\s*:\\s*(\\d+)\\s*,\\s*\"name\"\\s*:\\s*\"([^\"]+)\"\\s*\\}");
    auto begin = std::sregex_iterator(json.begin(), json.end(), userRegex);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        int id = std::stoi((*it)[1].str());
        std::string name = (*it)[2].str();
        users.push_back({ id, name });
    }
    return users;
}

static std::string serializeUsers(const std::vector<User>& users) {
    std::string json = "{ \"users\": [";
    bool first = true;
    for (const auto& u : users) {
        if (!first) json += ", ";
        json += "{ \"id\": " + std::to_string(u.id) + ", \"name\": \"" + u.name + "\" }";
        first = false;
    }
    json += "] }";
    return json;
}

void Database::InitializeDatabase() {
    std::ifstream file("users.json");
    if (!file.good()) {
        writeFile("users.json", "{ \"users\": [] }");
        std::cout << "Database initialized with an empty user list." << std::endl;
    }
}

std::string Database::GetUsers() {
    return readFile("users.json");
}

void Database::AddUser(const std::string& userName) {
    std::string content = readFile("users.json");
    std::vector<User> users = parseUsers(content);
    int maxId = 0;
    for (const auto& u : users) {
        if (u.id > maxId) maxId = u.id;
    }
    User newUser{ maxId + 1, userName };
    users.push_back(newUser);
    std::string newContent = serializeUsers(users);
    writeFile("users.json", newContent);
}

bool Database::DeleteUser(int userId) {
    std::string content = readFile("users.json");
    std::vector<User> users = parseUsers(content);
    size_t originalSize = users.size();
    users.erase(std::remove_if(users.begin(), users.end(), [userId](const User& u) {
        return u.id == userId;
        }), users.end());
    if (users.size() == originalSize) {
        return false; // No user was deleted.
    }
    else {
        std::string newContent = serializeUsers(users);
        writeFile("users.json", newContent);
        return true;
    }
}
