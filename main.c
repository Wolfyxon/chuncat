#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

void print_help();
bool starts_with(char* a, char* b);

int main(int argc, char* argv[]) {
    if(argc < 2) {
        print_help();
        return 1;
    }

    char* command = argv[1];

    if(strcmp(command, "help") == 0) {
        print_help();
    } else if(strcmp(command, "split") == 0) {
        printf("TODO: split\n");
    } else if(strcmp(command, "join") == 0) {
        printf("TODO: join\n");
    } else {
        printf("chuncat: Unknown command '%s', see `chuncat help` \n", command);
    }
}

bool starts_with(char* a, char *b) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    
    if(len_a < len_b) 
        return false;

    return strncmp(a, b, len_b);
}

void print_help() {
    printf("Usage: chuncat <command> <files>... \n");
}