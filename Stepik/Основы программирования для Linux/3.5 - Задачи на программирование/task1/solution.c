#include <stdio.h>
#include <string.h>

int main() {
    FILE* status_file = fopen("/proc/self/status", "r");
    
    char param[256] = "\0";
    char value[256] = "\0";
    char ppid_str[256] = "PPid:";
    
    while (strcmp(param, ppid_str) != 0) {
        fscanf(status_file, "%s", param);
    }
    fscanf(status_file, "%s", value);

    printf("%s\n", value);
    fclose(status_file);
    return 0;
}
