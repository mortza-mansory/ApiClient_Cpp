#include "JsonHelper.h"

// Example: {"key": "value"}
std::string JsonHelper::Serialize(const std::string& key, const std::string& value) {
    return "{\"" + key + "\": \"" + value + "\"}";
}

std::string JsonHelper::Deserialize(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\": \"";
    auto start = json.find(searchKey);
    if (start == std::string::npos)
        return "";
    start += searchKey.length();
    auto end = json.find("\"", start);
    if (end == std::string::npos)
        return "";
    return json.substr(start, end - start);
}
