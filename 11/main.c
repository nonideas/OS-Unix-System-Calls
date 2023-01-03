#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

extern char** environ;

int my_execvpe(const char* filename, char** argv, char** envp)
{
    char** tmp = environ;
    environ = envp;
    execvp(filename, argv);
    environ = tmp;
    return -1;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s cmd\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed!");
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        char** new_environ = malloc(2);
        if (new_environ == NULL)
        {
            return 3;
        }
        new_environ[0] = "TZ=PST";
        new_environ[1] = NULL;
        if (my_execvpe(argv[1], argv, new_environ) == -1) {
            perror("exec");
        }
    }
    return 0;
}