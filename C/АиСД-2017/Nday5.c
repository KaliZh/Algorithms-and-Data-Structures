#define _CRT_SECURE_NO_WARNINGS
#define SIZE 10
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct Item {
    int busy; //признак занятости элемента
    int key; //ключ элемента
    char*info; //указатель на информацию
} Item;

typedef struct Table {
    Item*elements[SIZE];
} Table;

int menu(const char*messages[], int);
int GetInt(int*, int, int, int); //проверка на ввод числа
int GetStr(char**); // проверка на ввод строки
int Hash(int); // получение хэш
void InitTable(Item*table[]); // инициализация таблицы
void FreeTable(Item*[]);
void InsertItem(Table*, int, char*); // включение элемента
void PrintTable(Table*); // вывод таблицы
void PrintElem(Table*);
Table *TableSearch(Table*, int);
int Add(Table*);
int Search(Table*);
int Delete(Table*);
int Print(Table*);
void Replace(Table*, int*, char**);
void Remove(Table*, int*, char**);
int Count(Table*, int*, int*);
void FreeM(int*, char**);

int main() {
    const char *messages[] = { "exit - 0", "add item - 1", "search item - 2", "delete - 3", "print table - 4", "replace - 5" };
    const int Nmes = sizeof(messages) / sizeof(messages[0]);
    int (*sect[])(Table*) = { NULL, Add, Search, Delete, Print, Remove };
    Table table;
    InitTable(table.elements);
    int a, *key;
    char **inf;
    while (a = menu(messages, Nmes)) {
        if (!a) exit(1);
        if (sect[a](&table) == NULL) 
            break;
    }
    //FreeTable(&table);
    return 0;
}

int menu(const char*messages[], int N) {
    char *errormessage = ""; //(сообщение об ошибке)
    int r, i, n;
    do {
        puts(errormessage);
        errormessage = "error, reenter please";
        for (i = 0; i < N; ++i)
            puts(messages[i]);
        n = GetInt(&r, 2, 0, N); //проверка на ввод выбора раздела
        if (n == 0) 
            r = 0;
    } while (r < 0 || r >= N);
    return r;
}

int Add(Table*table) { // добавление элемента в таблицу
    int key, vhash;
    char *info;
    printf("enter key = ");
    if (GetInt(&key, 3, 0, 0) == 0)
        return 0;
    printf("enter value: ");
    if (GetStr(&info) == 0)
        return 0;
    InsertItem(table, key, info, -1);
    return 1;
}

int Search(Table*table) { // поиск по таблице
    int key;
    Table *res;
    printf("enter key = ");
    if (GetInt(&key, 1, 0, 0) == 0)
        return 0;
    res = TableSearch(table, key);
    if (res) {
        PrintElem(res);
        return 1;
    }
    printf("\n");
    return 1;
}

Table*TableSearch(Table*table, int key) { // поиск по ключу
    int vhash;
    int j = key, n = 0, count = 0;
    Item *k;
    Table *res = (Table*)malloc(sizeof(Table));
    InitTable(res->elements);
    vhash = Hash(key);
    k = table->elements[vhash];
    while (k->busy != 0 && !count) {
        if (k->busy == 1 && k->key == key) {
            printf(" key - %d, info - %s, busy - 1", k->key, k->info);
            count++;
            printf("\n");
        }
        if (k->busy == -1) {
            printf("element with key %d not found", key);
            return NULL;
        }
        vhash = Hash(++j);
        k = table->elements[vhash];
        n++;
    }
    if (count == 0) {
        printf("element with key %d not found", key);
        _getch();
        return NULL;
    }
    return res;
}

int Delete(Table*table) { // удаление элемента с заданным ключом
    int key, vhash;
    int j, n = 0, count = 0;
    Item *k;
    printf("enter key = ");
    if (GetInt(&key, 1, 0, 0) == 0) 
        return 0;
    j = key;
    vhash = Hash(key);
    k = table->elements[vhash];
    while (n < SIZE) {
        if (k->busy == 1 && k->key == key) {
            k->key = NULL;
            k->info = NULL;
            k = (Item*)realloc(k, sizeof(Item));
            //free(k);
            k->busy = -1; //элемент удален
            count++;
        }
        vhash = Hash(++j);
        k = table->elements[vhash];
        n++;
    }
    if (!count) { // если элемент не найден
        printf("element with key %d not found\n", key);
        _getch();
    }
    else printf("deleted\n");
    return 1;
}

int Print(Table*table) { // печать таблицы
    PrintTable(table);
    return 1;
}

int GetInt(int*a, int mode, int min, int max) { // проверка на ввод чисел
    int n;
    do {
        n = scanf("%d", a, sizeof(int));
        if (n < 0) 
            return 0;
        if (n == 0) {
            printf("error, reenter please\n");
            scanf("%*[^\n]");
        } else if (mode == 2 && *a > max || *a < min) {
            printf("out of range, enter %d<n<%d\n", min, max);
            scanf("%*[^\n]");
        } else if (mode == 3 && *a < min) {
            printf("your digit < %d, repeat\n", min);
            scanf("%*[^\n]");
        } else {
            break;
        }
    } while (1);
    scanf("%*c");
    return 1;
}

int GetStr(char**s) { // проверка на ввод (info)
    char *str;
    int ch, si = 16;
    size_t len = 0;
    str = (char*)realloc(NULL, sizeof(char)*si);
    if (!str) 
        return 0;
    while (-1 != (ch = fgetc(stdin)) && ch != '\n') {
        str[len++] = ch;
        if (len == si) {
            str = (char*)realloc(str, sizeof(char)*(si += 16));
            if (!str) 
                return 0;
        }
    }
    if (ch == -1) {
        free(str);
        return 0;
    }
    str[len++] = '\0';
    *s = (char*)realloc(str, sizeof(char)*len);
    return 1;
}

int Hash(int i) {
    return i % SIZE;
}

void InsertItem(Table*table, int key, char* info) {
    int vhash, start, n = 0, j = key;
    Item *k;
    start = vhash = Hash(key);
    k = table->elements[vhash];
    while (k) {
        if (k->key == key && k->busy == 1) {
            printf("element with key %d already exists", key);
            return;
        }
        if (k->busy != 1) {
            k->key = key;
            k->busy = 1; // добавление элемента, теперь он занят
            k->info = info;
            return;
        }
        vhash = Hash(++j);
        k = table->elements[vhash];
        n++;
        if (start == vhash) {
            printf("no place for element with key %d", key);
            return;
        }
    }
}

void InitTable(Item*table[]) {
    int i;
    Item *k;
    for (i = 0; i < SIZE; i++) {
        k = (Item*)malloc(sizeof(Item));
        k->busy = 0;
        table[i] = k;
    }
}

void FreeTable(Table*table) {
    int i;
    Item*k;
    for (i = 0; i < SIZE; i++) {
        k = table->elements[i];
        free(k);
    }
}

void PrintTable(Table*table) {
    int i;
    Item *k;
    for (i = 0; i < SIZE; i++) {
        k = table->elements[i];
        if (k->busy == 1) { // если элемент существует
            printf(" key - %d, info - %s, busy - 1", k->key, k->info);
            printf("\n");
        }
        if (k->busy == 0) {
            printf(" key - , info - , busy - 0");
            printf("\n");
        }
        if (k->busy == -1) {
            printf(" key - , info - , busy - -1", k->key, k->info);
            printf("\n");
        }
    }
    _getch();
}

void PrintElem(Table*table) {
    int i;
    Item *k;
    for (i = 0; i < SIZE; i++) {
        k = table->elements[i];
        if (k->busy == 1) { // если элемент существует
            printf(" key - %d, info - %s, busy - 1", k->key, k->info);
            printf("\n");
        }
    }
}

void Remove(Table*table, int *key, char **inf) {
    int op = 0, dp = 0, fp = 0, i, j, t, opp;
    Item *k;
    op = Count(table, &dp, &fp);
    key = (int*)malloc(op * sizeof(int));
    inf = (char**)malloc(SIZE * sizeof(char*));
    j = 0;
    opp = op;
    while (opp) {
        for (i = dp; op && i < SIZE; i++) {
            k = table->elements[i];
            if (k->busy == 1) {
                key[j] = k->key;
                k->busy = -1;
                inf[j] = (char*)malloc(inf[j], (strlen(k->info) + 1) * sizeof(char));
                inf[j] = k->info;
                opp--;
                break;
            }
        }
        j++;
    }   
    j = 0;
    while (op) {
        for (i = dp; i < SIZE; i++) {
            k = table->elements[i];
            if (key[j] && Hash(key[j]) <= i && k->busy == -1) {
                k->busy = 1;
                k->key = key[j];
                k->info = (char*)realloc(k->info, (strlen(inf[j]) + 1) * sizeof(char));
                strcpy(k->info, inf[j]);
                free(inf[j]);
                inf[j] = NULL;
                op--;
                break;
            }
        }
        j++;
    }
    PrintTable(table);
    _getch();
    //Replace(table, key, info);
}

void Replace(Table*table, int *key, char **inf) {
    int op = 0, dp = 0, fp = 0, i, j;
    Item *k;
    op = Count(table, &dp, &fp);
    while (op) {
        j = 0;
        for (i = dp; i < SIZE; i++) {
            k = table->elements[i];
            if (Hash(key[j]) >= i && k->busy == -1) {
                k->busy = 1;
                k->key = key[j];
                k->info = realloc(k->info, strlen(inf[j]) * sizeof(char));
                //strcpy(k->info, info[j]);
                op--;
                free(inf[j]);
                free(key[j]);
                break;
            }
        }
        j++;
    }
    PrintTable(table);
    _getch();
}

int Count(Table*table, int*dp/*первый busy = -1*/, int*fp/*первый busy = 1*/) {
    int op = 0, i, d = 0, p = 0, f = 0;
    Item *k;
    for (i = 0; i < SIZE; i++) {
        k = table->elements[i];
        if (k->busy == -1 && !d) 
            p = i;
        if (k->busy == -1) 
            d++;
        if (d && k->busy == 1 && !op) 
            f = i;
        if (d && k->busy == 1) 
            op++;
    }
    *fp = f;
    *dp = p;
    return op;
}

void FreeM(int *key, char **info) {
    for (int i = 0; i < SIZE; i++) {
        free(info[i]);
        free(key[i]);
    }
}
