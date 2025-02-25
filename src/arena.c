#include <assert.h>

#define KILOBYTES(number) ((number) * 1024ull)
#define MEGABYTES(number) (KILOBYTES(number) * 1024ull)
#define GIGABYTES(number) (MEGABYTES(number) * 1024ull)
#define TERABYTES(number) (GIGABYTES(number) * 1024ull)

typedef struct {
    unsigned char* memory;
    unsigned char* ptr;
    size_t capacity;
} Arena;

void* allocate(Arena* a, size_t size)
{
    assert(((a->ptr - a->memory) + size) <= a->capacity);
    unsigned char* ptr = a->ptr;
    a->ptr += size;
    return ptr;
}

void freeArena(Arena* a)
{
    memset(a->memory, 0, a->capacity);
    free(a->memory);
    free(a);
}

void clear(Arena* a)
{
    memset(a->memory, 0, a->capacity);
    a->ptr = a->memory;
}

Arena* newArena(size_t size)
{
    Arena* a = malloc(sizeof(Arena));
    assert(a);
    memset(a, 0, sizeof(Arena));

    a->memory = malloc(size);
    memset(a->memory, 0, size);

    a->ptr = a->memory;
    a->capacity = size;

    return a;
}
