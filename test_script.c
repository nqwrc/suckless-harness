#include <stdio.h>
#include <string.h>
int main() {
    char a[20] = {0};
    char b[20] = {0};
    printf("%d\n", memcmp(a, b, 20));
    return 0;
}
