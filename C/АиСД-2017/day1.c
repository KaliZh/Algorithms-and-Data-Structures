#include <stdio.h>

void sort(int b[], int n, int a[]) {
    int i, j, temp, temp1;
    for (i = 1; i < n; i++, b[j + 1] = temp, a[j + 1] = temp1)
        for (temp = b[i], temp1 = a[i], j = i - 1; j >= 0 && temp < b[j]; --j) {
            b[j + 1] = b[j];
            a[j + 1] = a[j];
        }
}

int erase_max(int x, int *newx) {
    int maxx = -1, i;
    for (i = x; i; i /= 10) 
        if ((i % 10) > maxx) 
            maxx = i % 10;
    int  d = 1, c = 0;
    for (i = x; i; i /= 10) 
        if ((i % 10) != maxx) {
            *newx += (i % 10) * d;
            d *= 10;
            c++;
        }
    return c;
}

int GetInt(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Write NATURAL number\n");
            scanf("%*[^\n]");
        }
        else if (n && *a < 0) {
            n = 0;
            printf("Write NATURAL number\n");
            scanf("%*[^\n]");
        }

    } while (!n);
    return 1;
}

int main() {
    int n, a[101], b[101], newx[101] = {0}, i;
    printf("Write natural number n\n");
    if (!GetInt(&n)) {
        printf("Error!");
        return 0;
    }
    for (i = 0; i < n; i++) {
        printf("Write %d element of sequence: ", i + 1);
        if (!GetInt(&a[i])) {
            printf("Error!");
            return 0;
        }
        b[i] = erase_max(a[i], &newx[i]);
    }
    printf("Initial sequence was ");
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    sort(b, n, newx);
    printf("New sequence is ");
    for (i = 0; i < n; i++)
        if (b[i] > 0)
            printf("%d ", newx[i]);
    printf("\n");

    return 0;
}
