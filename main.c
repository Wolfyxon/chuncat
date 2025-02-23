#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

typedef struct {
    int (*func)(int, char*[]);
    const char* name;
    const char* description;
} Command;

int cmd_help(int _argc, char* _argv[]);
int cmd_split(int argc, char* argv[]);
bool starts_with(char* a, char* b);

const Command COMMANDS[] = {
    {
        .func = cmd_help,
        .name = "help",
        .description = "Shows help"
    },
    {
        .func = cmd_split,
        .name = "split",
        .description = "Splits files"
    }
};

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cmd_help(argc, argv);
        return 1;
    }

    char* command_name = argv[1];

    for(int i = 0; i < sizeof(COMMANDS) / sizeof(Command); i++) {
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

int cmd_split(int argc, char* argv[]) {
    if(argc < 5) {
        printf("Usage: chuncat split <mode> <amount | bytes> <file> \n");
        return 1;
    }
    
    char* mode = argv[2];
    char* str_amt = argv[3];
    char* file_path = argv[4];

    if(strcmp(mode, "count") != 0 && strcmp(mode, "bytes")) {
        printf("chuncat: '%s' is not a valid mode. Use 'count' or 'bytes'. \n", mode);
        return 1;
    }

    if(!isdigit(*str_amt)) {
        printf("chuncat: '%s' is not a valid number. \n", str_amt);
        return 1;
    }

    int amt = atoi(str_amt);

    if(amt <= 0) {
        printf("chuncat: Number must be greater than 0 \n");
        return 1;
    }

    FILE* input_file = fopen(file_path, "r");

    if(input_file == NULL) {
        // TODO: Describe the error
        printf("chuncat: Failed to open '%s' \n", file_path);
    }
}