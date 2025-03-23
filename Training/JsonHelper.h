#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <string>

class JsonHelper {
public:
    static std::string Serialize(const std::string& key, const std::string& value);
    static std::string Deserialize(const std::string& json, const std::string& key);
};

#endif // JSON_HELPER_H
