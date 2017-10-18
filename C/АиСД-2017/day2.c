#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

int is_even(int a) {
    int i;
    for (i = a; i; i /= 10)
        if ((i % 10) % 2 != 0)
            return 0;
    return 1;
}

int get_int(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Write NATURAL number\n");
            scanf("%*[^\n]");
        } else if (n && *a < 0) {
            n = 0;
            printf("Write NATURAL number\n");
            scanf("%*[^\n]");
        }

    } while (!n);
    return 1;
}

void random(int *a) {
    *a = rand() % 201 - 100;
}

int main() {
    int **a, **b;
    int m, i, j, c;
    srand(time(NULL));
    printf("Do you want use random? (1/0)\n");
    scanf("%d", &c); 
    printf("Write number of rows: ");
    if (!get_int(&m)) {
        printf("Error!");
        return 0;
    }
    int na[m], nb[m];
    a = (int**)malloc(m * sizeof(int*));
    b = (int**)malloc(m * sizeof(int*));
    for (i = 0; i < m; i++) {
        printf("Write number of columns for %d row: ", i);
        if (!get_int(&na[i])) {
            printf("Error!");
            return 0;
        }
        nb[i] = 0;
        a[i] = (int*)malloc(na[i] * sizeof(int));
        b[i] = (int*)malloc(na[i] * sizeof(int));
        for (j = 0; j < na[i]; j++) {
            if (!c) {
                printf("a[%d][%d] = ", i, j);
                if (!get_int(&a[i][j])) {
                    printf("Error!");
                    return 0;
                }
            } else {
                random(&a[i][j]);
            }
            if (is_even(a[i][j])) {
                b[i][nb[i]] = a[i][j];
                nb[i]++;
            }
        }
        b[i] = (int*)realloc(b[i], (nb[i] + 1) * sizeof(int));
    }
    printf("initial matrix\n");
    for (i = 0; i < m; i++) 
    {
        for (j = 0; j < na[i]; j++)
            printf("%d ", a[i][j]);
        free(a[i]);
        printf("\n");
    }
    printf("new matirx\n");
    for (i = 0; i < m; i++)
    {
        if (!nb[i]) {
            printf("null!\n");
            continue;
        }
        for (j = 0; j < nb[i]; j++)
            printf("%d ", b[i][j]);
        free(b[i]);
        printf("\n");
    }
    free(a); 
    free(b);

    return 0;
}
