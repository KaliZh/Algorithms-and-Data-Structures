#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

struct Matrix {
    int sz;
    double **val;
};

void free_matrix(Matrix *M) {
    int i;
    for (i = 0; i < M->sz; i++)
        free(M->val[i]);
    free(M->val);
}

Matrix *alloc_matrix() {
    return (Matrix*)malloc(sizeof(Matrix));
}

void print_matrix(Matrix *M, char ch) {
    int i, j;
    printf("The size of matrix %c is %d.\n", ch, M->sz);
    for (i = 0; i < M->sz; i++) {
        for (j = 0; j < M->sz; j++)
            printf("%.2lf ", M->val[i][j]);
        printf("\n");
    }
    printf("\n");
}

int get_bool(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Write 1 or 0, if you agree or disagree\n");
            scanf("%*[^\n]");
        } else if (*a < 0 || *a > 1) {
            n = 0;
            printf("Write 1 or 0, if you agree or disagree\n");
            scanf("%*[^\n]");
        }

    } while (!n);
    return 1;
}

int get_double(double *a) {
    int n;
    do {
        n = scanf("%lf", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Enter real number.\n");
            scanf("%*[^\n]");
        }
    } while (!n);
    return 1;
}

int get_int(int *a, int min_sz, int max_sz) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Enter integer.\n");
            scanf("%*[^\n]");
        } else if (*a < min_sz || *a > max_sz) {
            n = 0;
            printf("Enter integer less than or equal to %d and more than or equal to %d.\n", max_sz, min_sz);
            scanf("%*[^\n]");
        }

    } while (!n);
    return 1;
}

void random_val(double *a) {
    *a = ((rand() % 201 - 100) * 22.0) / 23;
}

double **get_matrix_val(int sz) {
    int n;
    printf("Program will use random numbers for matrix. Do you agree? (1/0)\n");
    if (!get_bool(&n)) {
        printf("Error!");
        return 0;
    }
    double **val = (double**)malloc(sz * sizeof(double*));
    int i, j;
    if (n) {
        for (i = 0; i < sz; i++) {
            val[i] = (double*)malloc(sz * sizeof(double));
            for (j = 0; j < sz; j++)
                random_val(&val[i][j]);
        }
    } else {
        printf("Enter numbers for %dx%d matrix.\n", sz, sz);
        for (i = 0; i < sz; i++)  {
            val[i] = (double*)malloc(sz * sizeof(double));
            for (j = 0; j < sz; j++) 
                if (!get_double(&val[i][j])) {
                    printf("Error!");
                    return 0;
                }
        }
    }
    return val;
}

int get_matrix_sz(int min_sz, int max_sz) {
    int n;
    printf("Program will use matrix of default size. Do you agree? (1/0)\n");
    if (!get_bool(&n)) {
        printf("Error!");
        return 0;
    }
    int sz;
    if (n) {
        sz = 10;
    } else {
        printf("Enter the size of matrix.\n");
        if (!get_int(&sz, min_sz, max_sz)) {
            printf("Error!");
            return 0;
        }
    }
    return sz;
}

Matrix *get_matrix(int min_sz, int max_sz) {
    Matrix *M = alloc_matrix();
    M->sz = get_matrix_sz(min_sz, max_sz);
    M->val = get_matrix_val(M->sz);
    return M;
}

Matrix cmd_get_matrix(int sz) {
    Matrix M;
    M.sz = sz;
    M.val = get_matrix_val(sz);
    return M;
}

double **cmd_add_matrix(Matrix *A, int test) {
    Matrix B;
    int i, j;
    if (test) {
        int c = 9;
        B.sz = A->sz;
        B.val = (double**)malloc(A->sz * sizeof(double*));
        for (i = 0; i < B.sz; i++) {
            B.val[i] = (double*)malloc(A->sz * sizeof(double));
            for (j = 0; j < B.sz; j++)
                B.val[i][j] = c--;
        }
    } else {
        printf("Program need another matrix with the same size for this command.\n");
        B = cmd_get_matrix(A->sz);
        print_matrix(&B, 'B');
    }
    double **C = (double**)malloc(A->sz * sizeof(double*));;
    for (i = 0; i < A->sz; i++) {
        C[i] = (double*)malloc(A->sz * sizeof(double));;
        for (j = 0; j < A->sz; j++)
            C[i][j] = A->val[i][j] + B.val[i][j];
    }
    if (!test)
        printf("-------=-------\n\n");
    else free_matrix(&B);
    return C;
}

double **cmd_mult_matrix(Matrix *A, int test) {
    Matrix B;
    int i, j;
    if (test) {
        double c = 9;
        B.sz = A->sz;
        B.val = (double**)malloc(A->sz * sizeof(double*));
        for (i = 0; i < B.sz; i++) {
            B.val[i] = (double*)malloc(A->sz * sizeof(double));
            for (j = 0; j < B.sz; j++)
                B.val[i][j] = c--;
        }
    } else {
        printf("Program need another matrix with the same size for this command.\n");
        Matrix B = cmd_get_matrix(A->sz);
        print_matrix(&B, 'B');
    }
    double **C = (double**)malloc(A->sz * sizeof(double*));;
    int k;
    for (i = 0; i < A->sz; i++) {
        C[i] = (double*)malloc(A->sz * sizeof(double));;
        for (k = 0; k < A->sz; k++)
            for (j = 0; j < A->sz; j++)
                C[i][k] += (A->val[i][j] * B.val[j][k]);
    }
    if (!test)
        printf("-------=-------\n\n");
    else free_matrix(&B);
    return C;
}

double cmd_get_scalar() {
    int n;
    printf("Program will use random number. Do you agree? (1/0)\n");
    if (!get_bool(&n)) {
        printf("Error!");
        return 0;
    }
    double sc;
    if (n) {
        random_val(&sc);
    } else {
        printf("Enter the scalar.\n");
        if (!get_double(&sc)) {
            printf("Error!");
            return 0;
        }
    }
    printf("The scalar is %.2lf.\n", sc);
    return sc;
}

double **cmd_mult_scalar_matrix(Matrix *A, int test) {
    double x;
    double **C = (double**)malloc(A->sz * sizeof(double*));;
    if (test) {
        x = 11;
    } else {
        printf("Program need some scalar for this command.\n");
        x = cmd_get_scalar();
    }
    int i, j;
    for (i = 0; i < A->sz; i++) {
        C[i] = (double*)malloc(A->sz * sizeof(double));;
        for (j = 0; j < A->sz; j++)
            C[i][j] = A->val[i][j] * x;
    }
    if (!test)
        printf("-------=-------\n\n");
    return C;
}

void random_row(int *n, int sz) {
    *n = rand() % sz;
}

double cmd_get_row(int sz) {
    int n;
    printf("Program will use random number. Do you agree? (1/0)\n");
    if (!get_bool(&n)) {
        printf("Error!");
        return 0;
    }
    int r;
    if (n) {
        random_row(&r, sz);
    } else {
        printf("Enter the number of row.\n");
        if (!get_int(&r, 0, sz)) {
            printf("Error!");
            return 0;
        }
    }
    printf("The number of row is %d.\n", r);
    return r;
}

double **cmd_add_row_matrix(Matrix *A, int test) {
    double x;
    int r, r1;
    double **C = (double**)malloc(A->sz * sizeof(double*));;
    if (test) {
        x = 10;
        r = 0;
        r1 = 0;
    } else {
        printf("Program need some scalar for this command.\n");
        x = cmd_get_scalar();
        printf("Program need number of row for this command.\n");
        r = cmd_get_row(A->sz);
        printf("Program need number of row for this command.\n");
        r1 = cmd_get_row(A->sz);
    }
    int i, j;
    for (i = 0; i < A->sz; i++) {
        C[i] = (double*)malloc(A->sz * sizeof(double));;
        for (j = 0; j < A->sz; j++)
            C[i][j] = A->val[i][j];
    }
    for (i = 0; i < A->sz; i++)
        C[r1][i] += (C[r][i] * x);
    if (!test)
        printf("-------=-------\n\n");
    return C;
}

int get_command() {
    int n;
    printf("Enter 1, if you want to add some matrix to matrix A.\n");
    printf("Enter 2, if you want to multiply some matrix to matrix A.\n");
    printf("Enter 3, if you want to multiply some scalar to matrix A.\n");
    printf("Enter 4, if you want to add some row multiplied by some scalar to another row of matrix A.\n");
    printf("Enter 0 for closing program.\n");
    if (!get_int(&n, 0, 4)) {
        printf("Error!");
        return 0;
    }
    return n;
}

void cmd_matrix(Matrix *A) {
    int cmd;
    do {
        cmd = get_command();
        if (cmd == 1) {
            A->val = cmd_add_matrix(A, 0);
            print_matrix(A, 'A');
            free_matrix(A);
            continue;
        }
        if (cmd == 2) {
            A->val = cmd_mult_matrix(A, 0);
            print_matrix(A, 'A');
            free_matrix(A);
            continue;
        }
        if (cmd == 3) {
            A->val = cmd_mult_scalar_matrix(A, 0);
            print_matrix(A, 'A');
            free_matrix(A);
            continue;
        }
        if (cmd == 4) {
            A->val = cmd_add_row_matrix(A, 0);
            print_matrix(A, 'A');
            free_matrix(A);
            continue;
        }
    } while (cmd);
}

void test() {
    printf("Program runs tests.\n");
    Matrix A;
    A.sz = 3;
    int i, j, c = 1;
    A.val = (double**)malloc(3 * sizeof(double*));
    for (i = 0; i < 3; i++) {
        A.val[i] = (double*)malloc(3 * sizeof(double));
        for (j = 0; j < 3; j++)
            A.val[i][j] = c++;
    }

    double res[3][3], **B;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            res[i][j] = 10;
    assert((B = cmd_add_matrix(&A, 1)) != NULL);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            assert(res[i][j] == B[i][j]);
    //for (i = 0; i < 3; i++)
    //    free(B[i]);
    //free(B);

    double d[] = {30, 24, 18, 84, 69, 54, 138, 114, 90};
    c = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            res[i][j] = d[c++];
    assert((B = cmd_mult_matrix(&A, 1)) != NULL);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            assert(res[i][j] == B[i][j]);
    for (i = 0; i < 3; i++)
        free(B[i]);
    free(B);
   
    d[0] = 11, d[1] = 22, d[2] = 33, d[3] = 44, d[4] = 55, d[5] = 66, d[6] = 77, d[7] = 88, d[8] = 99;
    c = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            res[i][j] = d[c++];
    assert((B = cmd_mult_scalar_matrix(&A, 1)) != NULL);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            assert(res[i][j] == B[i][j]);
    for (i = 0; i < 3; i++)
        free(B[i]);
    free(B);

    d[0] = 11, d[1] = 22, d[2] = 33, d[3] = 4, d[4] = 5, d[5] = 6, d[6] = 7, d[7] = 8, d[8] = 9;
    c = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            res[i][j] = d[c++];
    assert((B = cmd_add_row_matrix(&A, 1)) != NULL);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            assert(res[i][j] == B[i][j]);
    for (i = 0; i < 3; i++)
        free(B[i]);
    free(B);
    printf("Program successfully ran tests.\n");
}

int main() {
    srand(time(NULL));
    test();
    Matrix *A = get_matrix(1, 1000);
    print_matrix(A, 'A');
    cmd_matrix(A);
    free(A);

    return 0;
}
