#include <stdio.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
int main(int argc, char** argv) {
    struct hostent* host = gethostbyname(argv[1]);

    int i = 0;
    while (host->h_addr_list[i] != NULL) {
        struct in_addr* addr = (struct in_addr*)host->h_addr_list[i];
        printf("%s\n", inet_ntoa(*addr));
        i++;
    }

    return 0;
}
