#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "Incorrect number of arguments.\nUsage: <envname>\n");
        exit(EXIT_FAILURE);
    }

    char* env = getenv(argv[1]);
    if (env == NULL){
        fprintf(stderr, "Given environment variable doesn't exist\n");
    }
    else {
        printf("%s=%s\n", argv[1], env);
    }

    return EXIT_SUCCESS;
}

