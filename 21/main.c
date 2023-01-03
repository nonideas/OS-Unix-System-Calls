#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int count = 0;

void sigcatch(int sig)
{
    if (sig == SIGQUIT)
    {
        char buf[70];
        sprintf(buf, "numbers of SIGINT signals: %d\n", count);
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }

    count++;
    write(STDOUT_FILENO, "\07", sizeof(char));
}

void main()
{
    void (*istat)() = sigset(SIGINT, sigcatch);
    if (istat == SIG_ERR)
    {
        perror("sigset failure");
        exit(1);
    }
    istat = sigset(SIGQUIT, sigcatch);

    if (istat == SIG_ERR)
    {
        perror("sigset failure");
        exit(1);
    }

    while (1)
    {
        pause();
    }
}