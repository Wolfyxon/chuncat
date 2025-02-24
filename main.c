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
} command_t;

int cmd_help(int _argc, char* _argv[]);
int cmd_split(int argc, char* argv[]);
int cmd_version(int _argc, char* _argv[]);
bool starts_with(char* a, char* b);

const char* VERSION = "1.0";

const command_t COMMANDS[] = {
    {
        .func = cmd_help,
        .name = "help",
        .description = "Shows help"
    },
    {
        .func = cmd_version,
        .name = "version",
        .description = "Shows the tool's version"
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

    for(int i = 0; i < sizeof(COMMANDS) / sizeof(command_t); i++) {
        command_t cmd = COMMANDS[i];

        if(strcmp(cmd.name, command_name) == 0) {
            return cmd.func(argc, argv);
        }
    }

    fprintf(stderr, "chuncat: Unknown command '%s', see `chuncat help` \n", command_name);
    return 1;
}

bool starts_with(char* a, char *b) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    
    if(len_a < len_b) 
        return false;

    return strncmp(a, b, len_b);
}

FILE* open_chunk(char* base_path, int i) {
    char path[FILENAME_MAX];
    sprintf(path, "%s.%i", base_path, i);

    FILE* file = fopen(path, "wb");

    if(file == NULL) {
        fprintf(stderr, "chuncat: Failed to open '%s' \n", path);
        exit(1);
    }

    return file;
}

int cmd_help(int _argc, char* _argv[]) {
    printf("Usage: chuncat <command> <files>... \n\n");
    printf("Commands: \n");

    int longest = 0;

    int commands_len = sizeof(COMMANDS) / sizeof(command_t);

    for(int i = 0; i < commands_len; i++) {
        command_t cmd = COMMANDS[i];
        int name_len = strlen(cmd.name);

        if(name_len > longest) {
            longest = name_len;
        }
    }

    for(int i = 0; i < commands_len; i++) {
        command_t cmd = COMMANDS[i];
        int name_len = strlen(cmd.name);

        char* name_field = malloc(longest);
        strcpy(name_field, cmd.name);

        for(int ii = name_len; ii < longest; ii++) {
            name_field[ii] = *" ";
        }

        printf("    %s   %s \n", name_field, cmd.description);
    }
}

int cmd_version(int _argc, char* _argv[]) {
    printf("chuncat v. %s", VERSION);
}

int cmd_split(int argc, char* argv[]) {
    if(argc < 5) {
        fprintf(stderr, "Usage: chuncat split <mode> <file count | byte size> <file> \n");
        return 1;
    }
    
    char* mode = argv[2];
    char* str_amt = argv[3];
    char* file_path = argv[4];

    if(strcmp(mode, "count") != 0 && strcmp(mode, "bytes")) {
        fprintf(stderr, "chuncat: '%s' is not a valid mode. Use 'count' or 'bytes'. \n", mode);
        return 1;
    }

    if(!isdigit(*str_amt)) {
        printf("chuncat: '%s' is not a valid number. \n", str_amt);
        return 1;
    }

    int count_or_bytes = atoi(str_amt);

    if(count_or_bytes <= 0) {
        fprintf(stderr, "chuncat: Number must be greater than 0 \n");
        return 1;
    }

    FILE* input_file = fopen(file_path, "r");

    if(input_file == NULL) {
        // TODO: Describe the error
        fprintf(stderr, "chuncat: Failed to open '%s' \n", file_path);
    }

    fseek(input_file, 0, SEEK_END);
    int file_len = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if(file_len == 0) {
        fprintf(stderr, "File cannot be empty \n");
        return 1;
    }

    int chunk_size = file_len / count_or_bytes;
    char* chunk = malloc(chunk_size);
    int total_chunks = 0;
    int current_chunk_len = 0;

    FILE* current_file;
    
    if(strcmp(mode, "count") == 0) {
        count_or_bytes = chunk_size;
    }

    for(int i = 0; i < file_len; i++) {
        char byte = fgetc(input_file);
        chunk[current_chunk_len++] = byte;

        if(i == 0) {
            current_file = open_chunk(file_path, total_chunks++);
        }
        
        if(i % count_or_bytes == 0 && i != 0) {
            printf("write %i \n", i);
            fwrite(chunk, sizeof(char), current_chunk_len, current_file);
            fclose(current_file);

            current_file = open_chunk(file_path, total_chunks++);
            
            chunk = malloc(chunk_size);
            current_chunk_len = 0;
        }
    }

    if(current_chunk_len != 0) {
        fwrite(chunk, sizeof(char), current_chunk_len, current_file);
        fclose(current_file);
    }
    
    fclose(input_file);
}