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

    int l = 0, r = strlen(res) - 1;
    while (l < r) {
        char tmp = res[l];
        res[l] = res[r];
        res[r] = tmp;
        r--;
        l++;
    }

    for (int i = 0; i < starcount; i++)
        res[i] = 'p';

    l = starcount, r = strlen(res) - 1;
    while (l < r) {
        char tmp = res[l];
        res[l] = res[r];
        res[r] = tmp;
        r--;
        l++;
    }

    res[strlen(res)] = '\0';

    return res;
}
