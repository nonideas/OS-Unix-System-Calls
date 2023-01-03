#include <stdio.h>      // printf
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h> // socket
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX 100

char* socket_path = "\0hidden";

int main(int argc, char* argv[])
{
    struct sockaddr_un addr;
    int rc;
    int data_socket;

    if (argc != 2)
    {
        perror("Usage: ./a.out socket_name\n");
        exit(-1);
    }

    if (argc > 1) socket_path = argv[1];

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error in server (func socket)\n");
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
        unlink(socket_path);
    }
    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("Error in server (func bind)\n");
        exit(-1);
    }

    if (listen(sockfd, 5) == -1)
    {
        close(sockfd);
        perror("Error in server (func listen)\n");
        exit(-1);
    }

    char buffer[MAX];

    while (1)
    {
        data_socket = accept(sockfd, NULL, NULL);
        if (data_socket == -1)
        {
            close(sockfd);
            perror("Error in server (func accept)!\n");
            continue;
        }
        while ((rc = read(data_socket, buffer, sizeof(buffer))) > 0)
        {
            for (int i = 0; i < rc; ++i)
            {
                buffer[i] = toupper(buffer[i]);
            }
            write(STDOUT_FILENO, buffer, rc);
        }
        if (rc == -1)
        {
            close(data_socket);
            perror("read");
            exit(1);
        }

        if (rc == 0)
        {
            printf("EOF\n");
            break;
        }
    }
    close(data_socket);
    close(sockfd);
    unlink(socket_path);
    return 0;
}