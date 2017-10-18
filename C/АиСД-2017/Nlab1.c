#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include <time.h>

typedef struct LinearForm {
    int n;
    int fcoef;
    int *coef;
    int *arg;
} LinearForm;

int GetInt(int *a, int minn, int maxx) {
    if (*a != -101) {
        printf("%d\n", *a);
        if (*a < minn || *a > maxx)
            return 0;
        else return 1;
    }
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Enter integer, please!\n");
            scanf("%*[^\n]");
        } else if (*a < minn || *a > maxx) {
            n = 0;
            printf("Integer should be not more than %d, not less than %d.\n", maxx, minn);
            scanf("%*[^\n]");
        }
    } while (!n);
    return 1;
}

void FreeForm(LinearForm *lf) {
    free(lf->coef);
    free(lf->arg);
    free(lf);
}

LinearForm *AllocForm(int n) {
    LinearForm *lf = (LinearForm*)malloc(sizeof(LinearForm));;
    lf->coef = (int*)malloc(n * sizeof(int));
    lf->arg = (int*)malloc(n * sizeof(int));
    return lf;
}

void PrintForm(LinearForm *lf) {
    printf("Linear form of %d variable: ", lf->n);
    printf("%d + ", lf->fcoef);
    int i;
    for (i = 0; i < lf->n; i++) {
        if ((i + 1) == lf->n) {
            if (lf->arg[i] != -101)
                printf("(%d * %d)", lf->arg[i], lf->coef[i]);
            else printf("(%d * x%d)", lf->coef[i], i + 1);
        } else {
            if (lf->arg[i] != -101)
                printf("(%d * %d) + ", lf->arg[i], lf->coef[i]);
            else printf("(%d * x%d) + ", lf->coef[i], i + 1);
        }
    }
    printf("\n");
}

LinearForm *MakeForm(int n, int fcoef, int *coef) {
    LinearForm *lf = AllocForm(n);
    lf->n = n;
    lf->fcoef = fcoef;
    int i;
    for (i = 0; i < lf->n; i++) {
        lf->coef[i] = coef[i];
        lf->arg[i] = -101;
    }
    return lf;
}

void MakeArg(LinearForm *a, int argind, int *args) {
    int i;
    for (i = 0; i < a->n; i++)
        if (i != argind)
            a->arg[i] = args[i];
}

void SolveForm(LinearForm *a, int argind, int *args) { 
    MakeArg(a, argind, args);
    int i;
    double argx = a->fcoef;
    for (i = 0; i < a->n; i++)
        if (i != argind)
            argx += (a->coef[i] * a->arg[i]);
    argx = (-1.0 * argx) / a->coef[argind];
    PrintForm(a);
    printf("solved x%d is %lf\n", argind + 1, argx);
}

LinearForm *MultForm(LinearForm *a, int x) {
    int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    fcoef = a->fcoef * x;
    int i;
    for (i = 0; i < a->n; i++)
        coef[i] = a->coef[i] * x;
    printf("multiplied\n");
    LinearForm *b = MakeForm(a->n, fcoef, coef);
    FreeForm(a);
    free(coef);
    return b;
}

LinearForm *SubsForms(LinearForm *a, LinearForm *b) {
    int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    fcoef = a->fcoef - b->fcoef;
    int i;     
    for (i = 0; i < a->n; i++)
        coef[i] = a->coef[i] - b->coef[i];
    printf("substracted\n");
    LinearForm *c = MakeForm(a->n, fcoef, coef);
    FreeForm(a);
    FreeForm(b);
    free(coef);
    return c;
}

LinearForm *AddForms(LinearForm *a, LinearForm *b) {
    int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    fcoef = a->fcoef + b->fcoef;
    int i;
    for (i = 0; i < a->n; i++)
        coef[i] = a->coef[i] + b->coef[i];
    printf("added\n");
    LinearForm *c = MakeForm(a->n, fcoef, coef);
    FreeForm(a);
    FreeForm(b);
    free(coef);
    return c;
}

LinearForm *RandomForm(int n) {
    if (!n)
        n = rand() % 99 + 2;
    int *coef = (int*)malloc(n * sizeof(int));
    int fcoef = rand() % 201 - 100;
    int i;
    for (i = 0; i < n; i++)
        coef[i] = rand() % 201 - 100;
    LinearForm *rf = MakeForm(n, fcoef, coef);
    free(coef);
    return rf;
}

void RandomMode() {
    LinearForm *lf, *lf1;
    lf = RandomForm(0);
    PrintForm(lf);
    lf1 = RandomForm(lf->n);
    PrintForm(lf1);
    lf = AddForms(lf, lf1);
    PrintForm(lf);
    lf1 = RandomForm(lf->n);
    lf = SubsForms(lf, lf1);
    PrintForm(lf);
    int x = rand() % 201 - 100;
    printf("%d\n", x);
    lf = MultForm(lf, x);
    PrintForm(lf);
    int *args = (int*)malloc(lf->n * sizeof(int)), argind = rand() % lf->n + 1;
    int i;
    for (i = 0; i < lf->n; i++)
        if (i != argind)
            args[i] = rand() % 201 - 100;
    SolveForm(lf, argind, args);
    free(args);
    FreeForm(lf);
}

int *GetArgs(int n, int argind, int *args) {
    printf("Enter arguments, please!\n");
    int i;
    if (!args) {
        args = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++)
            args[i] = -101;
    }
    for (i = 0; i < n; i++) {
        if (i != argind) {
            printf("x%d = ", i + 1);
            if (!GetInt(&args[i], -100, 100)) {
                printf("Something goes wrong..\n");
                return NULL;
            }
        } else printf("x%d = ?\n", i + 1);
    }
    return args;
}

int GetArgInd(LinearForm *a, int argind) {
    printf("Choose the argument that you want to find, please!\n");
    PrintForm(a);
    printf("Enter the number of argument.\n");
    if (!GetInt(&argind, 1, a->n)) {
        printf("Something goes wrong..\n");
        return 0;
    }
    return argind - 1;
}

int GetScalar(int x) {
    printf("Enter the scalar, please!\n");
    if (!GetInt(&x, -100, 100)) {
        printf("Something goes wrong..\n");
        return 0;
    }
    return x;
}

LinearForm *GetForm(int n, int fcoef, int *coef) {
    printf("Enter the number of arguments, please!\n");
    if (!GetInt(&n, 1, 100)) {
        printf("Something goes wrong..\n");
        return NULL;
    }
    printf("Enter free term, please!\n");
    if (!GetInt(&fcoef, -100, 100)) {
        printf("Something goes wrong..\n");
        return NULL;
    }
    int i;
    if (!coef) {
        coef = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++)
            coef[i] = -101;
    }
    printf("Enter %d coeficients, please!\n", n);
    for (i = 0; i < n; i++)
        if (!GetInt(&coef[i], -100, 100)) {
            printf("Something goes wrong..\n");
            return NULL;
        }
    LinearForm *lf = MakeForm(n, fcoef, coef);
    free(coef);
    return lf;
}

int GetCommand(int n) {
    printf("Choose command, please!\n");
    printf("Enter 1 if you want to add some form.\n");
    printf("Enter 2 if you want to substract some form.\n");
    printf("Enter 3 if you want to multiply by scalar the form.\n");
    printf("Enter 4 if you want to solve the form.\n");
    printf("Enter 5 if you want to see the current form.\n");
    printf("Enter 0 if you want to quit from normal mode.\n");
    if (!GetInt(&n, 0, 5)) {
        printf("Something goes wrong..\n");
        return 0;
    }
    return n;
}

void TestMode() {
    printf("Run tests for GetForm.\n");
    LinearForm *lf;
    int *a = (int*)malloc(3 * sizeof(int));
    a[0] = 23; a[1] = -1; a[2] = 11;
    assert(GetForm(0, 0, NULL) == NULL);
    assert(GetForm(101, 0, NULL) == NULL);
    assert(GetForm(-23, 0, NULL) == NULL);
    assert((lf = GetForm(3, 12, a)) != NULL);
    printf("OK!\n\n");

    printf("Run tests for GetCommand.\n");
    assert(GetCommand(-1) == 0);
    assert(GetCommand(6) == 0);
    assert(GetCommand(1) == 1);
    printf("OK!\n\n");

    printf("Run tests for AddForms.\n");
    a = (int*)malloc(3 * sizeof(int));
    a[0] = -1; a[1] = -2; a[2] = -3;
    LinearForm *lf1 = MakeForm(lf->n, 4, a);
    assert((lf = AddForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 16);
    assert(lf->coef[0] == 22);
    assert(lf->coef[1] == -3);
    assert(lf->coef[2] == 8);
    a[0] = 1; a[1] = 2; a[2] = 3;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = AddForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 20);
    assert(lf->coef[0] == 23);
    assert(lf->coef[1] == -1);
    assert(lf->coef[2] == 11);
    a[0] = 0; a[1] = 0; a[2] = 0;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = AddForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 24);
    assert(lf->coef[0] == 23);
    assert(lf->coef[1] == -1);
    assert(lf->coef[2] == 11);
    a[0] = -1; a[1] = 2; a[2] = 0;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = AddForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 28);
    assert(lf->coef[0] == 22);
    assert(lf->coef[1] == 1);
    assert(lf->coef[2] == 11);
    printf("OK!\n\n");
    
    printf("Run tests for SubsForms.\n");
    a[0] = -1; a[1] = -2; a[2] = -3;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = SubsForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 24);
    assert(lf->coef[0] == 23);
    assert(lf->coef[1] == 3);
    assert(lf->coef[2] == 14);
    a[0] = 1; a[1] = 2; a[2] = 3;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = SubsForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 20);
    assert(lf->coef[0] == 22);
    assert(lf->coef[1] == 1);
    assert(lf->coef[2] == 11);
    a[0] = 0; a[1] = 0; a[2] = 0;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = SubsForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 16);
    assert(lf->coef[0] == 22);
    assert(lf->coef[1] == 1);
    assert(lf->coef[2] == 11);
    a[0] = -1; a[1] = 2; a[2] = 0;
    lf1 = MakeForm(lf->n, 4, a);
    assert((lf = SubsForms(lf, lf1)) != NULL);
    assert(lf->fcoef == 12);
    assert(lf->coef[0] == 23);
    assert(lf->coef[1] == -1);
    assert(lf->coef[2] == 11);
    printf("OK!\n\n");

    printf("Run tests for MultForm.\n");
    assert(GetScalar(-102) == 0);
    assert(GetScalar(213) == 0);
    assert(GetScalar(10) == 10);
    assert((lf = MultForm(lf, 10)) != NULL);
    assert(lf->fcoef == 120);
    assert(lf->coef[0] == 230);
    assert(lf->coef[1] == -10);
    assert(lf->coef[2] == 110);
    printf("OK!\n\n");

    printf("Run tests for SolveForm.\n");
    assert(GetArgInd(lf, -200) == 0);
    assert(GetArgInd(lf, 500) == 0);
    assert(GetArgInd(lf, 4) == 0);
    assert(GetArgInd(lf, 0) == 0);
    assert(GetArgInd(lf, 2) == 1);
    a[0] = -105; a[2] = 13;
    assert(GetArgs(lf->n, 1, a) == NULL);
    a[0] = -10; a[2] = 103;
    assert(GetArgs(lf->n, 1, a) == NULL);
    a[0] = -15; a[2] = 13;
    assert(GetArgs(lf->n, 1, a) != NULL);
    SolveForm(lf, 1, a);
    printf("OK!\n\n");
    FreeForm(lf);
}

void NormalMode() {
    LinearForm *lf = GetForm(-101, -101, NULL);
    int cmd;
    do {
        cmd = GetCommand(-101);
        if (cmd == 1) {
            LinearForm *lf1 = GetForm(lf->n, -101, NULL);
            lf = AddForms(lf, lf1);
        } else if (cmd == 2) {
            LinearForm *lf1 = GetForm(lf->n, -101, NULL);
            lf = SubsForms(lf, lf1);
        } else if (cmd == 3) {
            int x = GetScalar(-101);
            lf = MultForm(lf, x);
        } else if (cmd == 4) {
            int argind = GetArgInd(lf, -101), *args = GetArgs(lf->n, argind, NULL);
            SolveForm(lf, argind, args);
            free(args);
        } else if (cmd == 5) {
            PrintForm(lf);
        }
    } while (cmd);
    FreeForm(lf);
}

int GetMode() {
    printf("Choose mode, please!\n");
    printf("Enter 1 if you want to use normal mode.\n");
    printf("Enter 2 if you want to use random mode.\n");
    printf("Enter 3 if you want to use test mode.\n");
    printf("Enter 0 if you want to switch off program.\n");
    int n = -101;
    if (!GetInt(&n, 0, 3)) {
        printf("Something goes wrong..\n");
        return 0;
    }
    return n;
}

void Mode() {
    int mode;
    do {
        mode = GetMode();
        if (mode == 1)
            NormalMode();
        else if (mode == 2) 
            RandomMode();
        else if (mode == 3)
            TestMode();
    } while (mode);
}

int main() {
    printf("This program can add, substract, multiply by scalar and solve Linear Forms.\n");
    printf("The number of coeficients are in the range of 1 - 100 and values are in the range of [-100; 100].\n");
    srand(time(NULL));
    Mode();
    return 0;
}
