#include <stdio.h>

int main() {

#ifdef DEBUG_FLAG
    printf("Hello, Debug!\n");
#else
    printf("Hello, World!\n");
#endif

    return 0;
}
