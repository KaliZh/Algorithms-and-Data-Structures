#include <complex>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <iostream>

using namespace std;

struct LinearForm {
    int n;
    complex<int> fcoef;
    complex<int> *coef;
    complex<int> *arg;
};

int GetInt(int *a, int minn, int maxx) {
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
    /*LinearForm *lf = (LinearForm*)malloc(sizeof(LinearForm));;
    lf->coef = (int*)malloc(n * sizeof(int));
    lf->arg = (int*)malloc(n * sizeof(int));*/
    LinearForm *lf = new LinearForm;
    lf->coef = new complex<int>[n];
    lf->arg = new complex<int>[n];
    return lf;
}

void PrintForm(LinearForm *lf) {
    printf("Linear form of %d variable: ", lf->n);
    //printf("%d + ", lf->fcoef);
    cout << lf->fcoef << " + ";
    int i;
    for (i = 0; i < lf->n; i++) {
        if ((i + 1) == lf->n) {
            if (lf->arg[i] != -101)
                cout << "(" << lf->arg[i] << " * " << lf->coef[i] << ")";
                //printf("(%d * %d)", lf->arg[i], lf->coef[i]);
            else cout << "(" << lf->coef[i] << " * x" << i + 1 << ")";
                //printf("(%d * x%d)", lf->coef[i], i + 1);
        } else {
            if (lf->arg[i] != -101)
                cout << "(" << lf->arg[i] << " * " << lf->coef[i] << ")";
                //printf("(%d * %d) + ", lf->arg[i], lf->coef[i]);
            else cout << "(" << lf->coef[i] << " * " << i + 1 << ")";
                //printf("(%d * x%d) + ", lf->coef[i], i + 1);
        }
    }
    printf("\n");
}

LinearForm *MakeForm(int n, complex<int> fcoef, complex<int> *coef) {
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
    complex<int> argx = a->fcoef;
    for (i = 0; i < a->n; i++)
        if (i != argind)
            argx += (a->coef[i] * a->arg[i]);
    argx = (-1 * argx) / a->coef[argind];
    PrintForm(a);
    //printf("solved x%d is %lf\n", argind + 1, argx);
    cout << "x" << argind + 1 << " = " << argx << "\n";
}

LinearForm *MultForm(LinearForm *a, int x) {
    //int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    complex<int> fcoef, *coef = new complex<int>[a->n];
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
    //int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    complex<int> fcoef, *coef = new complex<int>[a->n];
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
    //int fcoef, *coef = (int*)malloc(a->n * sizeof(int));
    complex<int> fcoef, *coef = new complex<int>[a->n];
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
    //int *coef = (int*)malloc(n * sizeof(int));
    //int fcoef = rand() % 201 - 100;
    complex<int> fcoef(rand() % 201 - 100, rand() % 201 - 100), *coef = new complex<int>[n];
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

int GetComplex(int *real, int *imag) {
    cout << "Enter a real part: ";
    if (!GetInt(real, -100, 100))
        return 0;
    cout << "Enter a imaginary part: ";
    if (!GetInt(imag, -100, 100))
        return 0;
    return 1;
}

LinearForm *GetForm() {
    printf("Enter the number of arguments, please!\n");
    int n;
    if (!GetInt(&n, 1, 100)) {
        printf("Something goes wrong..\n");
        return NULL;
    }
    printf("Enter free term, please!\n");
    int real, imag;
    if (!GetComplex(&real, &imag)) {
        printf("Something goes wrong..\n");
        return NULL;
    }
    complex<int> fcoef(real, imag), *coef = new complex<int>[n];
    int i;
    printf("Enter %d coeficients, please!\n", n);
    for (i = 0; i < n; i++) {
        real = imag = 0;
        if (!GetComplex(&real, &imag)) {
            printf("Something goes wrong..\n");
            return NULL;
        }
        complex<int> temp(real, imag);
        coef[i] = temp;
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

void NormalMode() {
    LinearForm *lf = GetForm();
    int cmd;
    do {
        cmd = GetCommand(-101);
        if (cmd == 1) {
            LinearForm *lf1 = GetForm();
            lf = AddForms(lf, lf1);
        } else if (cmd == 2) {
            LinearForm *lf1 = GetForm();
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
    printf("Enter 0 if you want to switch off program.\n");
    int n = -101;
    if (!GetInt(&n, 0, 2)) {
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
    } while (mode);
}

int main() {
    printf("This program can add, substract, multiply by scalar and solve Linear Forms.\n");
    printf("The number of coeficients are in the range of 1 - 100 and values are in the range of [-100; 100].\n");
    srand(time(NULL));
    Mode();
    return 0;
}
