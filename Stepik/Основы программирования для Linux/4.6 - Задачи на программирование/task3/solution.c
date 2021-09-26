#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int sigusr1_count = 0;
int sigusr2_count = 0;

void sigusr1_handler(int signalno) {
    (void)signalno;
    ++sigusr1_count;
}

void sigusr2_handler(int signalno) {
    (void)signalno;
    ++sigusr2_count;
}

void sigterm_handler(int signalno) {
    (void)signalno;
    printf("%d %d\n", sigusr1_count, sigusr2_count);
    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGUSR1, &sigusr1_handler);
    signal(SIGUSR2, &sigusr2_handler);
    signal(SIGTERM, &sigterm_handler);

    while (1) {};
    return 0;
}
