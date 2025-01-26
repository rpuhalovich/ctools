#include <stdlib.h>

// from: https://gist.github.com/rodrigoalvesvieira/662e400f34dd9de38176
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define SystemOpenURL(url) system("start " url);
#elif __APPLE__
#define SystemOpenURL(url) system("open " url);
#elif __linux__
#define SystemOpenURL(url) system("xdg-open " url);
#else
#error "Unknown compiler"
#endif

int main(void)
{
    SystemOpenURL("https://devdocs.io/c/");
    return 0;
}
