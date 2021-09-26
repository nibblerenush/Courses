#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define buffer_size 5120

int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[1]));
    inet_aton("127.0.0.1", &local.sin_addr);
    
    bind(sockfd, (struct sockaddr*) &local, sizeof(local));
    
    char buffer[buffer_size + 1] = "\0";
    while (1) {
        buffer[0] = '\0';

        int count = read(sockfd, buffer, sizeof(buffer));
        buffer[count] = '\0';
        
        if (strcmp(buffer, "OFF\n") == 0) {
            break;
        }
        else {
            printf("%s\n", buffer);
        }
    }
    
    close(sockfd);
    return 0;
}
