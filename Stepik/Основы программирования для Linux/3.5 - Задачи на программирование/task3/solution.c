#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

char ppid_value[256] = "\0";

void get_ppid(const char* pid) {
    char absolute_file_name[256] = "/proc/";
    strcat(absolute_file_name, pid);
    strcat(absolute_file_name, "/status");
    
    FILE* status_file = fopen(absolute_file_name, "r");
    
    char param[256] = "\0";
    char ppid_str[256] = "PPid:";
    
    while (strcmp(param, ppid_str) != 0) {
        fscanf(status_file, "%s", param);
    }
    fscanf(status_file, "%s", ppid_value);

    fclose(status_file);
}

int main(int argc, char** argv) {
    strcat(ppid_value, argv[1]);
    while (strcmp(ppid_value, "1") != 0) {
        printf("%s\n", ppid_value);
        get_ppid(ppid_value);
    }
    printf("%s\n", ppid_value);

    return 0;
}
