#include <stdio.h>      // printf
#include <string.h>     // strncpy, memset
#include <stdlib.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // write
#include <sys/un.h>
#include <netinet/in.h>

char* socket_path = "\0hidden";

int main(int argc, char* argv[])
{
    struct sockaddr_un addr;
    int rc;

    if (argc != 2)
    {
        perror("Usage: ./a.out socket_name\n");
        exit(-1);
    }

    char* buffer[100];
    if (argc > 1) socket_path = argv[1];

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error in client (func socket)\n");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    if (*socket_path == '\0')
    {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
    }
    else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    }

    int c_ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (c_ret == -1)
    {
        perror("Error in client (func connect)\n");
        close(sockfd);
        exit(-1);
    }

    while ((rc = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        int w_ret = write(sockfd, buffer, rc);
        if (w_ret != rc)
        {
            if (rc > 0) fprintf(stderr, "partical write");
            else
            {
                perror("Error in client (func write)\n");
                close(sockfd);
                exit(-1);
            }
        }
    }

    close(sockfd);
    return 0;
}