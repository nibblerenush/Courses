#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void read_and_count(int fd, int* result, int* closed_fds) {
    char buffer[256] = "\0";
    int count = read(fd, buffer, sizeof(buffer));
    
    if (count == -1) {
        return;
    }
    
    if (count == 0) {
        close(fd);
        ++(*closed_fds);
    }
    else {
        int i = 0;
        for (i = 0; i < count; ++i) {
            *result += buffer[i] - '0';
        }
    }
}

int main(int argc, char** argv) {
    int fd1 = open("./in1", O_RDONLY | O_NONBLOCK);
    int fd2 = open("./in2", O_RDONLY | O_NONBLOCK);
    
    int result = 0;
    fd_set read_set;
    int closed_fds = 0;
    
    while (1) {
        FD_ZERO(&read_set);
        FD_SET(fd1, &read_set);
        FD_SET(fd2, &read_set);
        
        if (select(fd2 + 1, &read_set, NULL, NULL, NULL)) {
            if (FD_ISSET(fd1, &read_set)) {
                read_and_count(fd1, &result, &closed_fds);
            }
            
            if (FD_ISSET(fd2, &read_set)) {
                read_and_count(fd2, &result, &closed_fds);
            }
        }

        if (closed_fds == 2) {
            break;
        }
    }

    printf("%d\n", result);
    return 0;
}
