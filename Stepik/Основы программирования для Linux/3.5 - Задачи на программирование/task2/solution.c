#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

int is_digit_str(const char* str) {
    int i = 0;
    for (i = 0; i < strlen(str); ++i) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int result = 0;
    
    DIR* proc_dir = opendir("/proc");
    struct dirent* dirent_file = NULL;
    
    while (dirent_file = readdir(proc_dir)) {
        if (is_digit_str(dirent_file->d_name)) {
            char absolute_file_name[256] = "/proc/";
            strcat(absolute_file_name, dirent_file->d_name);
            strcat(absolute_file_name, "/status");
            
            FILE* file = fopen(absolute_file_name, "r");
            
            char param[256] = "\0";
            char process_name[256] = "\0";
            
            fscanf(file, "%s %s", param, process_name);
            if (strcmp(process_name, "genenv") == 0) {
                ++result;
            }
            
            fclose(file);
        }
    }

    printf("%d\n", result);
    return 0;
}
