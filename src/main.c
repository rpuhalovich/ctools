#include <stdio.h>
#include <string.h>

char types[128][64];
int types_count = 0;

char includes[128][64];
int includes_count = 0;

char template_file[10000][512];
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
        for (;line[i] != ' '; i++) {}

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

    for (int i = 0; i < out_file_name_length; i++) {
        fprintf(fp, "yeet\n");

        fprintf(fp, "%s\n", );
    }

    fclose(f);
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: ctemplate path/to/types.ctypes path/to/template_file.[ct,ht]\n");
        return 0;
    }

    char* template_file_path = argv[2];

    populate_types_and_includes(argv[1]);
    read_template_file(argv[2]);

    char out_path[256];
    int len = get_out_filename(out_path, argv[2]);
    out_path[len] = '\0';

    return 0;
}
