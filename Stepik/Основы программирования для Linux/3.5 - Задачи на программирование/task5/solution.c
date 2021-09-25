#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("%d\n", getpid());

        chdir("/");
        setsid();

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        while (1) {};
    }

    return 0;
}
