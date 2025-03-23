#include "Routes.h"
#include <iostream>
#include <sstream>
#include <string>
#include "UserService.h"
#include <winsock2.h>

void startRoutes(SOCKET clientSocket) {
    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recv error: " << WSAGetLastError() << std::endl;
        return;
    }

    std::string request(buffer);
    std::string method, path;
    std::stringstream requestStream(request);
    requestStream >> method >> path;

    std::string responseBody;
    std::string httpStatus = "HTTP/1.1 200 OK\r\n";

    if (method == "GET" && path == "/users") {
        responseBody = UserService::ShowAllUsers();
    }
    else if (method == "POST" && path == "/users") {
        std::string body = request.substr(request.find("\r\n\r\n") + 4);
        if (body.empty()) {
            httpStatus = "HTTP/1.1 400 Bad Request\r\n";
            responseBody = "{\"error\": \"No user name provided\"}";
        }
        else {
            responseBody = UserService::CreateUser(body);
        }
    }
    else if (method == "DELETE") {
        std::string prefix = "/users/";
        if (path.find(prefix) == 0 && path.length() > prefix.length()) {
            std::string idStr = path.substr(prefix.length());
            try {
                int userId = std::stoi(idStr);
                responseBody = UserService::RemoveUser(userId);
            }
            catch (const std::exception& e) {
                httpStatus = "HTTP/1.1 400 Bad Request\r\n";
                responseBody = "{\"error\": \"Invalid user ID\"}";
            }
        }
        else {
            httpStatus = "HTTP/1.1 404 Not Found\r\n";
            responseBody = "404 Not Found";
        }
    }
    else {
        httpStatus = "HTTP/1.1 404 Not Found\r\n";
        responseBody = "404 Not Found";
    }

    std::string httpResponse =
        httpStatus +
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(responseBody.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        responseBody;

    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
}