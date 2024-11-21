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

#define CTEMPLATE_DIR "./gen"

void read_types_file(char* path);
void read_template_file(char* path);
void write_file(char* path);
void write_string_to_file(FILE* f, char* str, int strlen);

char _tmpstr[MAX_LINE_LEN];
void write_string_to_file(FILE* f, char* str, int strlen)
{
    memcpy(_tmpstr, str, strlen);
    _tmpstr[strlen] = '\0';
    fprintf(f, "%s", _tmpstr);
}

void read_types_file(char* path)
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
            int type_template_strlen = strlen("%TYPE%");
            int type_template_np_strlen = strlen("%TYPENP%");
            int template_end_index = 0;

            for (int k = 0; k < types_count; k++) {
                int typestrlen = strlen(types[k]);

                int type_ptr_count = 0;
                for (int j = 0; j < typestrlen; j++) {
                    if (types[k][j] == '*')
                        type_ptr_count++;
                }

                char* pstr = "pppppppp";

                int j = i + 1;
                for (; strcmp("%TEMPLATE_END%", template_file[j]) != 0; j++) {
                    int len = strlen(template_file[j]);

                    int s = 0, q = 0;
                    for (; q < len; q++) {
                        if (template_file[j][q] == '%' && strncmp("%TYPE%", template_file[j] + q, type_template_strlen) == 0) {
                            write_string_to_file(f, template_file[j] + s, q - s);
                            write_string_to_file(f, types[k], typestrlen);
                            q += type_template_strlen;
                            s = q;
                        }

                        if (template_file[j][q] == '%' && strncmp("%TYPENP%", template_file[j] + q, type_template_np_strlen) == 0) {
                            write_string_to_file(f, template_file[j] + s, q - s);

                            write_string_to_file(f, pstr, type_ptr_count);
                            write_string_to_file(f, types[k], typestrlen - type_ptr_count);

                            q += type_template_np_strlen;
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
    if (argc != 4)
        return 1;

    char* types_file_path = argv[1];
    char* template_file_path = argv[2];
    char* out_file_path = argv[3];

    read_types_file(types_file_path);
    read_template_file(template_file_path);
    write_file(out_file_path);

    return 0;
}
