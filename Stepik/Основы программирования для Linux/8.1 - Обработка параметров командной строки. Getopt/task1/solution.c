#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int opterr;

int main(int argc, char** argv) {
    opterr = 0;

    static struct option long_options[] = {
        {"query", required_argument, NULL, 'q'},
        {"longinformationrequest ", no_argument, NULL, 'i'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    int opchar = 0;
    int opindex = 0;

    while ((opchar = getopt_long(argc, argv, "q:iv", long_options, &opindex)) != -1) {
        switch (opchar) {
        case 'q':
        case 'i':
        case 'v':
            break;
        case '?':
            printf("-\n");
            exit(0);
        }
    }

    printf("+\n");
    return 0;
}
