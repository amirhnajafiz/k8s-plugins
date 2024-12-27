// create a program that accepts a string like "user@host:port", opens a connection to the host on the port, and prints the message received from the server.
// use socket programming to make the connection.
// accept the user@host:port string as a command line argument.
// until the server closes the connection, the client should keep reading the message from the server and printing it to the console.

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " user@host:port" << std::endl;
        return 1;
    }

    std::string userHostPort(argv[1]);
    size_t atPos = userHostPort.find('@');
    size_t colonPos = userHostPort.find(':');
    if (atPos == std::string::npos || colonPos == std::string::npos) {
        std::cerr << "Invalid user@host:port string" << std::endl;
        return 1;
    }

    std::string user = userHostPort.substr(0, atPos);
    std::string host = userHostPort.substr(atPos + 1, colonPos - atPos - 1);
    std::string portStr = userHostPort.substr(colonPos + 1);
    int port;
    std::istringstream(portStr) >> port;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    char buffer[1024] = {0};
    int valread;
    while ((valread = read(sock, buffer, 1024)) > 0) {
        std::cout << buffer;
        memset(buffer, 0, 1024);
    }

    return 0;
}
