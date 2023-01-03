#include <stdio.h>
#include <string.h>
int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror(argv[0]);
        return 1;
    }

    FILE* pipe = popen("wc -l", "w");
    char str[BUFSIZ];
    int flag = 1;
    while (fgets(str, BUFSIZ, file) != NULL)
    {
        if (str[0] == '\n' && flag)
            fputs(str, pipe);
        if (str[strlen(str) - 1] == '\n') flag = 1;
        else flag = 0;
    }
    fclose(file);
    pclose(pipe);
    return 0;
}