#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char types[128][64];
int types_count = 0;

char includes[128][64];
int includes_count = 0;

#define MAX_LINE_LEN 512
char template_file[10000][MAX_LINE_LEN];
int template_file_count = 0;
int template_begin = 0;
int template_end = 0;

char out_file_name[256];
int out_file_name_length = 0;

void populate_types_and_includes(char* path)
{
    FILE* f = fopen(path, "r");
    if (f == NULL)
        return;

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, f)) > 0) {
        if (!linelen)
            continue;

        int i = 0;
        for (; line[i] != ' '; i++) {
        }

        if (strncmp("INCLUDE", line, strlen("INCLUDE")) == 0)
            memcpy(includes[includes_count++], (char*)(line + i + 1), linelen - i - 2);

        if (strncmp("TYPE", line, strlen("TYPE")) == 0)
            memcpy(types[types_count++], (char*)(line + i + 1), linelen - i - 2);
    }

    fclose(f);
}

void read_template_file(char* path)
{
    FILE* f = fopen(path, "r");
    if (f == NULL)
        return;

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, f)) > 0)
        memcpy(template_file[template_file_count++], line, linelen - 1);

    fclose(f);
}

int get_out_filename(char* out_path, char* path)
{
    int len = strlen(path);
    char str[256];
    memset(str, 0, sizeof(str));
    memcpy(str, path, sizeof(char) * (len - 3));
    sprintf(out_path, "%s_gen.%c", str, path[len - 2]);
    return strlen(out_path);
}

void write_file(char* path)
{
    FILE* f = fopen(path, "w");
    if (f == NULL)
        return;

    for (int i = 0; i < template_file_count; i++) {
        if (strcmp("%INCLUDE%", template_file[i]) == 0) {
            for (int j = 0; j < includes_count; j++)
                fprintf(f, "#include %s\n", includes[j]);
            continue;
        }

        fprintf(f, "%s\n", template_file[i]);
    }

    fclose(f);
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: ctemplate path/to/types.ctypes "
               "path/to/template_file.[ct,ht]\n");
        return 0;
    }

    char* types_file_path = argv[1];
    char* template_file_path = argv[2];

    populate_types_and_includes(types_file_path);
    read_template_file(template_file_path);

    char out_path[256];
    int len = get_out_filename(out_path, template_file_path);
    char* write_path = malloc(len);
    memcpy(write_path, out_path, len);

    write_file(out_path);

    free(write_path);

    return 0;
}
