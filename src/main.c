#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

char types[128][64];
int types_count = 0;

char includes[128][64];
int includes_count = 0;

#define MAX_LINE_LEN 512
char template_file[10000][MAX_LINE_LEN];
int template_file_count = 0;

#define CTEMPLATE_DIR "./gen"

void populate_types_and_includes(char* path);
void read_template_file(char* path);
void write_file(char* path);
void write_string_to_file(FILE* f, char* str, int strlen);
int get_out_filename_dir(char* out_path, char* path, char* dir);

char _tmpstr[MAX_LINE_LEN];
void write_string_to_file(FILE* f, char* str, int strlen)
{
    memcpy(_tmpstr, str, strlen);
    _tmpstr[strlen] = '\0';
    fprintf(f, "%s", _tmpstr);
}

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

    free(line);
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

    free(line);
    fclose(f);
}

int get_out_filename_dir(char* out_path, char* path, char* dir)
{
    int len = strlen(path);
    char str[256];
    memset(str, 0, sizeof(str));

    int i = len;
    for (; i >= 0 && path[i] != '/'; i--) {
    }

    memcpy(str, path + i + 1, sizeof(char) * (len - i - 4));

    sprintf(out_path, "%s/%s.%c", dir, str, path[len - 2]);

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

        if (strcmp("%TEMPLATE_BEGIN%", template_file[i]) == 0) {
            int typetemplatestrlen = strlen("%TYPE%");
            int template_end_index = 0;

            for (int k = 0; k < types_count; k++) {
                int typestrlen = strlen(types[k]);

                int j = i + 1;
                for (; strcmp("%TEMPLATE_END%", template_file[j]) != 0; j++) {
                    int len = strlen(template_file[j]);

                    int s = 0, q = 0;
                    for (; q < len; q++) {
                        if (template_file[j][q] == '%' && strncmp("%TYPE%", template_file[j] + q, typestrlen) == 0) {
                            write_string_to_file(f, template_file[j] + s, q - s);
                            write_string_to_file(f, types[k], typestrlen);
                            q += typetemplatestrlen;
                            s = q;
                        }
                    }

                    if (s < q)
                        write_string_to_file(f, template_file[j] + s, q - s);

                    write_string_to_file(f, "\n", 1);
                }

                if (k < types_count - 1)
                    write_string_to_file(f, "\n", 1);

                template_end_index = j;
            }

            i = template_end_index;
            continue;
        }

        fprintf(f, "%s\n", template_file[i]);
    }

    fclose(f);
}

int main(int argc, char** argv)
{
    if (argc != 3)
        return 1;

    char* types_file_path = argv[1];
    char* template_file_path = argv[2];

    populate_types_and_includes(types_file_path);
    read_template_file(template_file_path);

    mkdir(CTEMPLATE_DIR, 0777);

    char out_path[256];
    get_out_filename_dir(out_path, template_file_path, CTEMPLATE_DIR);

    write_file(out_path);

    return 0;
}
