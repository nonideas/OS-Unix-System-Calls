#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void printStat(pid_t pid, int stat)
{
    //printf("Process status - %d\n", stat);
    if (WIFEXITED(stat)) {
        printf("Process %d ended with exit code %d\n", pid, WEXITSTATUS(stat));
    }
    if (WIFSIGNALED(stat)) {
        printf("Process %d terminated by signal %d\n", pid, WTERMSIG(stat));
    }
    if (WCOREDUMP(stat)) {
        printf("Process %d ended with core dump", pid);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("usage: ./main [cmd] [arg]\n");
        return 1;
    }

    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("Fork failure\n");
        return 1;
    }
    if (pid == 0) {
        execvp(argv[1], argv + 1);
        perror("Execute argv error\n");
        return 1;
    }
    else {
        int childStat;
        if (wait(&childStat) == -1)
        {
            perror("Wait failure\n");
            return 1;
        }
        printStat(pid, childStat);
    }

    return 0;
}