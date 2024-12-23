#include <sys/socket.h>

int main()
{
    // creating server socket
    int s_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (s_socket == -1) {
        return 1;
    }
}
