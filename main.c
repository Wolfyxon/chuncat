#include <stdio.h>
#include <string.h>
#include <glob.h>

void print_help();

int main(int argc, char* argv[]) {
    if(argc < 2) {
        print_help();
        return 1;
    }

    char* command = argv[1];

    if(strcmp(command, "help") == 0) {
        print_help();
    } else {
        printf("chuncat: Unknown command '%s', see `chuncat help` \n", command);
    }
}

void print_help() {
    printf("Usage: chuncat <command> <files>... \n");
}