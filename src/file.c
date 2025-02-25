char* readFile(Arena* arena, char* path)
{
    Arena* scratch = newArena(MEGABYTES(16));
    char* res = allocate(arena, 1);

    char* tmp = allocate(scratch, MEGABYTES(8));
    size_t tmplen = 0;

    FILE* f = fopen(path, "r");
    if (f == NULL)
        exit(1);

    char line[512];
    while (fgets(line, sizeof(line), f) != NULL) {
        int linelen = strlen(line);
        for (int i = 0; i < linelen; i++)
            tmp[tmplen++] = line[i];
    }

    res = allocate(arena, sizeof(char) * tmplen);
    memcpy(res, tmp, sizeof(char) * tmplen);

defer:
    freeArena(scratch);
    fclose(f);
    return res;
}
