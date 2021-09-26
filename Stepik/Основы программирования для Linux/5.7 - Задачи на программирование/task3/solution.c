#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define buffer_size 5120

int compare (const void* vl, const void* vr) {
    char* l = (char*)vl;
    char* r = (char*)vr;

    if (*l > *r) {
        return -1;
    }
    else if (*l < *r) {
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[1]));
    inet_aton("127.0.0.1", &local.sin_addr);
    
    bind(sockfd, (struct sockaddr*) &local, sizeof(local));
    listen(sockfd, 5);
    int clientfd = accept(sockfd, NULL, NULL);
    
    char buffer[buffer_size + 1] = "\0";
    while (1) {
        buffer[0] = '\0';
        int count = read(clientfd, buffer, sizeof(buffer));
        
        if (strcmp(buffer, "OFF") == 0) {
            break;
        }
        else {
            qsort((void*)buffer, strlen(buffer), 1, compare);
            write(clientfd, buffer, strlen(buffer) + 1);
        }
    }

    close(clientfd);
    close(sockfd);
    return 0;
}
