#include "../include/client.h"

int main(int argc, char *argv[]) {
    // check if the user has provided the correct number of arguments
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " user@host:port" << std::endl;
        return 1;
    }

    // parse the user@host:port string
    std::string userHostPort(argv[1]);
    size_t atPos = userHostPort.find('@');
    size_t colonPos = userHostPort.find(':');
    if (atPos == std::string::npos || colonPos == std::string::npos) {
        std::cerr << "invalid user@host:port string" << std::endl;
        return 1;
    }

    // extract the user, host, and port
    std::string user = userHostPort.substr(0, atPos);
    std::string host = userHostPort.substr(atPos + 1, colonPos - atPos - 1);
    std::string portStr = userHostPort.substr(colonPos + 1);
    int port;
    std::istringstream(portStr) >> port;

    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "failed to create socket" << std::endl;
        return 1;
    }

    // connect to the server
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
        std::cerr << "invalid address" << std::endl;
        return 1;
    }

    // connect to the server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "connection failed" << std::endl;
        return 1;
    }

    // get server response
    char buffer[1024] = {0};
    int valread;
    while ((valread = read(sock, buffer, 1024)) > 0) {
        std::cout << buffer;
        memset(buffer, 0, 1024);
    }

    return 0;
}
