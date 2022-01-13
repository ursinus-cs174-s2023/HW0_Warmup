#include <stdio.h>

int main() {
    int x = 0;
    for (int i = 1; i < 19; i++) {
        x += i;
    }
    x += 3;
    printf("%i\n", x);
    return 0;
}
