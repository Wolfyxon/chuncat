#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

typedef struct {
    int (*func)(int, char*[]);
    const char* name;
    const char* description;
} Command;

int cmd_help(int _argc, char* _argv[]);
bool starts_with(char* a, char* b);

const Command COMMANDS[] = {
    {
        .func = cmd_help,
        .name = "help",
        .description = "Shows help"
    }
};

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cmd_help(argc, argv);
        return 1;
    }

    char* command_name = argv[1];

    for(int i = 0; i < sizeof(COMMANDS); i++) {
        Command cmd = COMMANDS[i];

        if(strcmp(cmd.name, command_name) == 0) {
            return cmd.func(argc, argv);
        }
    }

    printf("chuncat: Unknown command '%s', see `chuncat help` \n", command_name);
    return 1;
}

bool starts_with(char* a, char *b) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    
    if(len_a < len_b) 
        return false;

    return strncmp(a, b, len_b);
}

int cmd_help(int _argc, char* _argv[]) {
    printf("Usage: chuncat <command> <files>... \n");
}