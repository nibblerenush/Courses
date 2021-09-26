#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    char command[256] = "\0";
    strcat(command, argv[1]);
    strcat(command, " ");
    strcat(command, argv[2]);

    int result = 0;
    FILE* command_pipe = popen(command, "r");

    while (1) {
        int c = fgetc(command_pipe);
        
        if (c == EOF) {
            break;
        }
        
        if ((char)c == '0') {
            ++result;
        }
    }

    printf("%d\n", result);
    pclose(command_pipe);
    return 0;
}
