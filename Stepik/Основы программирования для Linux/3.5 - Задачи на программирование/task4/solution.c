#include <stdio.h>
#include <string.h>

int result = 0;

void count_pids(const char* ppid) {
    ++result;
    
    char absolute_file_name[256] = "/proc/";
    strcat(absolute_file_name, ppid);
    strcat(absolute_file_name, "/task/");
    strcat(absolute_file_name, ppid);
    strcat(absolute_file_name, "/children");
    
    FILE* children_file = fopen(absolute_file_name, "r");
    
    char pid[256] = "\0";
    while (fscanf(children_file, "%s", pid) != EOF) {
        count_pids(pid);
    }
    
    fclose(children_file);
}

int main(int argc, char** argv) {
    count_pids(argv[1]);
    printf("%d\n", result);
    return 0;
}
