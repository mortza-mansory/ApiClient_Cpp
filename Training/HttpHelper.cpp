#include "HttpHelper.h"
#include <sstream>
#include <iostream>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

size_t HttpHelper::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string HttpHelper::Get(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::string HttpHelper::Post(const std::string& url, const std::string& data) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::string HttpHelper::Put(const std::string& url, const std::string& data) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

void HttpHelper::start(const std::string& ip, int port, const std::function<void(SOCKET)>& routes) {
    std::atomic<bool> running{ true };

    // init Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // allow address reuse
    char opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &(address.sin_addr));
    address.sin_port = htons(port);

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    std::cout << "HTTP Server started on " << ip << ":" << port << std::endl;
    std::cout << "Waiting for incoming HTTP connections..." << std::endl;

    std::thread serverThread([&]() {
        while (running.load()) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(server_fd, &readfds);

            timeval tv;
            tv.tv_sec = 1;  // 1 second timeout
            tv.tv_usec = 0;

            // winsock ignor the first parameter
            int activity = select(0, &readfds, nullptr, nullptr, &tv);
            if (activity == SOCKET_ERROR) {
                std::cerr << "select error: " << WSAGetLastError() << std::endl;
                break;
            }
            if (activity == 0) {
                continue; // no connection continue looping
            }
            if (FD_ISSET(server_fd, &readfds)) {
                sockaddr_in client_address;
                int client_addrlen = sizeof(client_address);
                SOCKET new_socket = accept(server_fd, reinterpret_cast<sockaddr*>(&client_address), &client_addrlen);
                if (new_socket == INVALID_SOCKET) {
                    std::cerr << "Accept error: " << WSAGetLastError() << std::endl;
                    continue;
                }

                char client_ip[INET_ADDRSTRLEN] = { 0 };
                inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
                std::cout << "Accepted HTTP connection from " << client_ip << std::endl;

                routes(new_socket);

                closesocket(new_socket);
            }
        }
        closesocket(server_fd);
        });

    std::cout << "Press Enter to stop the HTTP server..." << std::endl;
    std::cin.get();
    running = false;
    serverThread.join();
    WSACleanup();
}
