#include <stdio.h>

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;




}

int main() {
    int x = 5, y = 10;

    printf("Max is %d\n", max(x, y));

    return 0;
}

