#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char** argv) {
    char dest[256] = "./";
    strcat(dest, argv[1]);
    
    void* header = dlopen(dest, RTLD_LAZY);

    int (*func) (int) = (int (*) (int)) dlsym(header, argv[2]);

    printf("%d\n", func(atoi(argv[3])));

    return 0;
}
