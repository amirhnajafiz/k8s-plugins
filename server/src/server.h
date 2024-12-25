#include <arpa/inet.h> // inet
#include <cstdlib> // errors
#include <cstring> // strlen
#include <iostream> // cout
#include <netinet/in.h> // sockaddr
#include <sys/socket.h> // socket
#include <unistd.h> // close

// list of the server errors
#define ErrConn "failed to accept client"
#define ErrSocket "failed to create server socket"
#define ErrSend "failed to send message"
