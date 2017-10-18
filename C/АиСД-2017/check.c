#include <stdio.h>
#include <stdlib.h>

void f(int n, int a[]) {
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
}

int main() {
    int a[] = {}
    f(3, a);
    f(0, 0);
    return 0;
}
