#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

void sigurg_handler(int signalno) {
    (void)signalno;
    exit(0);
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("%d\n", getpid());

        chdir("/");
        setsid();

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        signal(SIGURG, &sigurg_handler);

        while (1) {};
    }

    return 0;
}
