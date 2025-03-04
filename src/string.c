char* replace(Arena* arena, char* src, size_t srclen, char* from, char* to) // assumes from and to are null terminated
{
    char* tmp = allocate(arena, MEGABYTES(4));

    size_t s = 0, f = 0;
    for (s = 0, f = 0; f < srclen; f++) {
        if (strncmp(from, src + f, strlen(from)) == 0) {
            memcpy(tmp + strlen(tmp), src + s, f - s);
            memcpy(tmp + strlen(tmp), to, strlen(to));
            f += strlen(from);
            s = f;
        }
    }

    memcpy(tmp + strlen(tmp), src + s, f - s);

    char* res = allocate(arena, sizeof(char) * strlen(tmp));
    memcpy(res, tmp, sizeof(char) * strlen(tmp));

    return res;
}

char* toNpString(Arena* arena, char* str)
{
    char* res = allocate(arena, sizeof(char) * strlen(str) + 1);
    memcpy(res, str, sizeof(char) * strlen(str) + 1);

    int starcount = 0;
    for (int i = 0; i < strlen(res); i++) {
        if (res[i] == '*') starcount++;
    }

    for (int l = 0, r = strlen(res) - 1; l < r; r--, l++) {
        char tmp = res[l];
        res[l] = res[r];
        res[r] = tmp;
    }

    for (int i = 0; i < starcount; i++)
        res[i] = 'p';

    for (int l = starcount, r = strlen(res) - 1; l < r; r--, l++) {
        char tmp = res[l];
        res[l] = res[r];
        res[r] = tmp;
    }

    res[strlen(res)] = '\0';

    return res;
}

// NOTE: taken from busybox implementation
char* zstrsep(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* ptr;

    if (!start)
        return NULL;

    if (!*delim) {
        ptr = start + strlen(start);
    } else {
        ptr = strpbrk(start, delim);
        if (!ptr) {
            *stringp = NULL;
            return start;
        }
    }

    *ptr = '\0';
    *stringp = ptr + 1;

    return start;
}
