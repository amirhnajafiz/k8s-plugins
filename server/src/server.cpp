#include "server.h"

using namespace std;

int main()
{
    // creating server socket
    int s_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (s_socket == -1) {
        cerr << ErrSocket << endl;
        return EXIT_FAILURE;
    }

    // setting the socket address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // binding the socket
    if (bind(s_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        return EXIT_FAILURE;
    }

    // listening to the bounded socket
    if (listen(s_socket, 10) == -1) {
        return EXIT_FAILURE;
    }

    // server while loop for accepting clients
    while(true)
    {
        // variables for client connection info
        struct sockaddr_in c_addr;
        socklen_t c_add_size = sizeof(c_addr);

        // accept client
        int c_socket = accept(s_socket, (struct sockaddr*)&c_addr, &c_add_size);
        if (c_socket == -1) {
            cout << ErrConn << endl;
        }

        // extract client's IP
        char c_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &c_addr.sin_addr, c_ip, INET_ADDRSTRLEN);

        // append a log entry in server (client ip)
        cout << "conn: " << c_ip << endl;

        // send a message to client
        const char* message = "bazinga!\n";
        if (send(c_socket, message, strlen(message), 0) == -1) {
            cout << ErrSend << endl;
        }

        // close client connection
        close(c_socket);
    }
}
