#include "../include/client.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    // check if the user has provided the correct number of arguments
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " user@host:port" << endl;
        return 1;
    }

    // parse the user@host:port string
    string userHostPort(argv[1]);
    size_t atPos = userHostPort.find('@');
    size_t colonPos = userHostPort.find(':');
    if (atPos == string::npos || colonPos == string::npos) {
        cerr << "invalid user@host:port string" << endl;
        return 1;
    }

    // extract user, host and port
    string user = userHostPort.substr(0, atPos);
    string host = userHostPort.substr(atPos + 1, colonPos - atPos - 1);
    std::stringstream portStream(userHostPort.substr(colonPos + 1));

    int port;
    portStream >> port;

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "socket creation error" << endl;
        return 1;
    }

    // setup server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // convert host to binary form
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        cerr << "invalid address/ address not supported" << endl;
        return 1;
    }

    // connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "connection failed" << endl;
        return 1;
    }

    // read commands from console and send to server
    string command;
    char buffer[1024] = {0};
    while (true) {
        cout << "/" << user << "$ ";
        getline(cin, command);

        // send command to server
        send(sock, command.c_str(), command.length(), 0);

        // receive response from server
        int valread;
        while (true) {
            valread = read(sock, buffer, sizeof(buffer) - 1);
            if (valread <= 0) {
                break; // Exit loop if no more data or error occurs
            }
            buffer[valread] = '\0'; // Null-terminate the buffer
            cout << buffer;
            if (valread < sizeof(buffer) - 1) {
                break; // Exit loop if the buffer is not full
            }
        }
        cout << endl;

        // clear buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // close the socket
    close(sock);
    return 0;
}
