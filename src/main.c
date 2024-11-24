#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMPLATE_LEN 8192
#define MAX_LINE_LEN 512
#define PSTR "pppppppp"

char types[128][64];
int types_count = 0;

char includes[128][64];
int includes_count = 0;

char template_file[MAX_TEMPLATE_LEN][MAX_LINE_LEN];
int template_file_line_count = 0;

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
        exit(1);

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, f)) > 0) {
        if (!linelen)
            continue;

        int i = 0;
        for (; line[i] != ' ' && i < linelen; i++);

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
        exit(1);

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, f)) > 0 && template_file_line_count < MAX_TEMPLATE_LEN)
        memcpy(template_file[template_file_line_count++], line, linelen - 1);

    free(line);
    fclose(f);
}

void write_file(char* path)
{
    FILE* f = fopen(path, "w");
    if (f == NULL)
        exit(1);

    for (int i = 0; i < template_file_line_count; i++) {
        if (strcmp("%INCLUDE%", template_file[i]) == 0) {
            if (includes_count == 0) {
                i++;
                continue;
            }

            for (int j = 0; j < includes_count; j++)
                fprintf(f, "#include %s\n", includes[j]);

            continue;
        }

        if (strcmp("%TEMPLATE_BEGIN%", template_file[i]) == 0) {
            int type_template_strlen = strlen("%TYPE%");
            int type_template_np_strlen = strlen("%TYPENP%");
            int template_end_index = 0;

            for (int k = 0; k < types_count; k++) {
                int type_strlen = strlen(types[k]);

                int type_ptr_count = 0;
                for (int j = 0; j < type_strlen; j++) {
                    if (types[k][j] == '*')
                        type_ptr_count++;
                }

                int j = i + 1;
                for (; strcmp("%TEMPLATE_END%", template_file[j]) != 0 && j < template_file_line_count; j++) {
                    int len = strlen(template_file[j]);

                    int s = 0, q = 0;
                    for (; q < len; q++) {
                        if (template_file[j][q] != '%')
                            continue;

                        if (strncmp("%TYPE%", template_file[j] + q, type_template_strlen) == 0) {
                            write_string_to_file(f, template_file[j] + s, q - s);
                            write_string_to_file(f, types[k], type_strlen);
                            q += type_template_strlen;
                            s = q;
                        }

                        if (strncmp("%TYPENP%", template_file[j] + q, type_template_np_strlen) == 0) {
                            write_string_to_file(f, template_file[j] + s, q - s);
                            write_string_to_file(f, PSTR, type_ptr_count);
                            write_string_to_file(f, types[k], type_strlen - type_ptr_count);
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

    read_types_file(argv[1]);
    read_template_file(argv[2]);
    write_file(argv[3]);

    return 0;
}
