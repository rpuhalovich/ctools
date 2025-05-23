#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "arena.c"
#include "file.c"
#include "string.c"

#define MAX_TYPES_LEN 256
#define MAX_TYPES_LINE_LEN 128
#define MAX_TEMPLATE_LEN 8192
#define MAX_LINE_LEN 512
#define PSTR "pppppppp"

Arena* arena;

char types[MAX_TYPES_LEN][MAX_TYPES_LINE_LEN];
int types_count = 0;

char forwards_tag[MAX_TYPES_LEN][MAX_TYPES_LINE_LEN];
char forwards_struct[MAX_TYPES_LEN][MAX_TYPES_LINE_LEN];
int forwards_count = 0;

char template_file[MAX_TEMPLATE_LEN][MAX_LINE_LEN];
int template_file_line_count = 0;

typedef struct {
    int numTypes;
    char* type1;
    char* type2;
} MultiType;

MultiType** multiTypes;
size_t multiTypesLen;

char* templateBuffer;
size_t templateBufferSize;

char* _tmpstr;
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

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f) != NULL) {
        int linelen = strlen(line);

        int i = 0;
        for (; line[i] != ' ' && i < linelen; i++)
            ;

        if (strncmp(line, "FORWARD", strlen("FORWARD")) == 0) {
            int j = i + 1;
            for (; line[j] != ' ' && j < linelen; j++)
                ;

            memcpy(forwards_tag[forwards_count], (char*)(line + i + 1), j - i - 1);
            memcpy(forwards_struct[forwards_count], (char*)(line + j + 1), linelen - j - 2);

            forwards_count++;
        }

        if (strncmp(line, "TYPE", strlen("TYPE")) == 0) {
            char *token, *string, *tofree;
            tofree = string = strdup(line);
            assert(string != NULL);

            multiTypes[multiTypesLen] = allocate(arena, sizeof(MultiType));

            for (int i = 0; (token = zstrsep(&string, " ")) != NULL; i++) {
                if (strncmp(token, "\0", 1) == 0)
                    continue;

                int len;
                for (len = 0; token[len] != '\0' && token[len] != '\n'; len++);

                if (i == 1) {
                    multiTypes[multiTypesLen]->type1 = allocate(arena, sizeof(char) * MAX_TYPES_LINE_LEN);
                    memcpy(multiTypes[multiTypesLen]->type1, token, len);
                    multiTypes[multiTypesLen]->type1[len] = '\0';
                    multiTypes[multiTypesLen]->numTypes = 1;
                }
                if (i == 2) {
                    multiTypes[multiTypesLen]->type2 = allocate(arena, sizeof(char) * MAX_TYPES_LINE_LEN);
                    multiTypes[multiTypesLen]->numTypes = 2;
                    memcpy(multiTypes[multiTypesLen]->type2, token, len);
                    multiTypes[multiTypesLen]->type2[len] = '\0';
                }
            }

            free(tofree);

            multiTypesLen++;
        }
    }

    fclose(f);
}

void read_template_file(char* path)
{
    FILE* f = fopen(path, "r");
    if (f == NULL)
        exit(1);

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f) != NULL && template_file_line_count < MAX_TEMPLATE_LEN) {
        int linelen = strlen(line);
        memcpy(template_file[template_file_line_count++], line, linelen - 1);
    }

    fclose(f);
}

void write_file(char* path)
{
    FILE* f = fopen(path, "w");
    if (f == NULL)
        exit(1);

    fprintf(f, "// GENERATED BY CTEMPLATE\n\n");

    Arena* replaceArena = newArena(MEGABYTES(32));

    size_t s, q;
    for (s = 0, q = 0; q < templateBufferSize; q++) {
        if (strncmp("%FORWARD%", templateBuffer + q, strlen("%FORWARD%")) == 0) {
            write_string_to_file(f, templateBuffer + s, q - s);

            for (int j = 0; j < forwards_count; j++) {
                fprintf(f, "struct %s;\n", forwards_tag[j]);
                fprintf(f, "typedef struct %s %s;\n", forwards_tag[j], forwards_struct[j]);
            }

            q += strlen("%FORWARD%");
            s = q;

            continue;
        }

        if (strncmp("%TEMPLATE_BEGIN%", templateBuffer + q, strlen("%TEMPLATE_BEGIN%")) == 0) {
            write_string_to_file(f, templateBuffer + s, q - s);

            size_t templateStartIndex = q + strlen("%TEMPLATE_BEGIN%") + 1; // extra 1 to avoid newline
            size_t templateEndIndex = q;
            while (strncmp("%TEMPLATE_END%", templateBuffer + templateEndIndex, strlen("%TEMPLATE_END%")) != 0)
                templateEndIndex++;

            for (size_t i = 0; i < multiTypesLen; i++) {
                if (multiTypes[i]->numTypes != 1)
                    break;

                clear(replaceArena);

                char* np = toNpString(replaceArena, multiTypes[i]->type1);
                char* s1 = replace(replaceArena, templateBuffer + templateStartIndex, templateEndIndex - templateStartIndex, "%TYPE%", multiTypes[i]->type1);
                char* s2 = replace(replaceArena, s1, strlen(s1), "%TYPENP%", np);

                write_string_to_file(f, s2, strlen(s2));
            }

            q = templateEndIndex + strlen("%TEMPLATE_END%");
            s = q;

            continue;
        }

        if (strncmp("%TEMPLATE2_BEGIN%", templateBuffer + q, strlen("%TEMPLATE2_BEGIN%")) == 0) {
            write_string_to_file(f, templateBuffer + s, q - s);

            size_t templateStartIndex = q + strlen("%TEMPLATE2_BEGIN%") + 1; // extra 1 to avoid newline
            size_t templateEndIndex = q;
            while (strncmp("%TEMPLATE2_END%", templateBuffer + templateEndIndex, strlen("%TEMPLATE2_END%")) != 0)
                templateEndIndex++;

            for (size_t i = 0; i < multiTypesLen; i++) {
                if (multiTypes[i]->numTypes != 2)
                    continue;

                clear(replaceArena);

                char* np1 = toNpString(replaceArena, multiTypes[i]->type1);
                char* np2 = toNpString(replaceArena, multiTypes[i]->type2);

                char* s1 = replace(replaceArena, templateBuffer + templateStartIndex, templateEndIndex - templateStartIndex, "%TYPE1%", multiTypes[i]->type1);
                char* s2 = replace(replaceArena, s1, strlen(s1), "%TYPE1NP%", np1);

                char* s3 = replace(replaceArena, s2, strlen(s2), "%TYPE2%", multiTypes[i]->type2);
                char* s4 = replace(replaceArena, s3, strlen(s3), "%TYPE2NP%", np2);

                write_string_to_file(f, s4, strlen(s4));
            }

            q = templateEndIndex + strlen("%TEMPLATE2_END%");
            s = q;

            continue;
        }
    }

    write_string_to_file(f, templateBuffer + s, q - s);

    freeArena(replaceArena);
    fclose(f);
}

int main(int argc, char** argv)
{
    if (argc != 4)
        return 1;

    arena = newArena(MEGABYTES(16));
    _tmpstr = allocate(arena, MEGABYTES(8));

    multiTypes = allocate(arena, sizeof(MultiType) * MAX_TYPES_LEN);
    templateBuffer = allocate(arena, sizeof(char) * 1024 * 512);

    read_types_file(argv[1]);

    templateBuffer = readFile(arena, argv[2]);
    templateBufferSize = strlen(templateBuffer);

    read_template_file(argv[2]);
    write_file(argv[3]);

    freeArena(arena);

    return 0;
}
