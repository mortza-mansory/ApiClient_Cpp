#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include <string>
#include <curl/curl.h>
#include <functional>
#include <winsock2.h>

class HttpHelper {
public:
    static std::string Get(const std::string& url);
    static std::string Post(const std::string& url, const std::string& data);
    static std::string Put(const std::string& url, const std::string& data);

    static void start(const std::string& ip, int port, const std::function<void(SOCKET)>& routes);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // HTTP_HELPER_H
