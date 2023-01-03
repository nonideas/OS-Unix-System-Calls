#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ulimit.h>
#include <limits.h>

char cwd[PATH_MAX];

int main(int argc, char* const argv[]) {
    extern char** environ;

    uid_t RID, EUID, RGID, EGID;
    uid_t PID, PPID, PGID;
    long ulimit_value, new_ulimit;
    struct rlimit rlim;

    extern char* optarg;
    extern int optind, optopt;
    int c;
    while ((c = getopt(argc, argv, "ispuU:cC:dvV:")) != -1) {
        switch (c) {
        case 'i':
            RID = getuid();
            EUID = geteuid();
            RGID = getgid();
            EGID = getegid();
            printf("RID = %d\nEUID = %d\nRGID = %d\nEGID = %d\n\n", RID, EUID, RGID, EGID);
            break;

        case 's':
            setpgid(getpid(), getpid());
            break;

        case 'p':
            PID = getpid();
            PPID = getppid();
            PGID = getpgid(PID);
            printf("PID = %d\nPPID = %d\nPGID = %d\n\n", PID, PPID, PGID);
            break;

        case 'u':
            ulimit_value = ulimit(UL_GETFSIZE);
            printf("ulimit = %ld\n\n", ulimit_value);
            break;
        case 'U':
            new_ulimit = atol(optarg);
            ulimit_value = ulimit(UL_SETFSIZE, new_ulimit);
            printf("ulimit = %ld\n\n", ulimit_value);
            break;

        case 'c':
            getrlimit(RLIMIT_CORE, &rlim);
            printf("RLIMIT_CORE = %ld\n\n", rlim.rlim_cur);
            break;

        case 'C':
            getrlimit(RLIMIT_CORE, &rlim);
            rlim.rlim_cur = atol(optarg);
            setrlimit(RLIMIT_CORE, &rlim);
            printf("RLIMIT_CORE = %ld\n\n", rlim.rlim_cur);
            break;

        case 'd':
            getcwd(cwd, sizeof(cwd) * PATH_MAX);
            printf("cwd = %s\n\n", cwd);
            break;

        case 'v':
            for (int i = 0; environ[i] != NULL; i++) {
                printf("%s\n", environ[i]);
            }
            printf("\n");
            break;

        case 'V':
            ulimit_value = putenv(optarg);
            break;
        case ':':
            fprintf(stderr, "Option -%c requires an operand\n", optopt);
            break;

        case '?':
            fprintf(stderr, "Unrecognized option: -%c\n", optopt);
            break;
        }
    }

    return 0;
}