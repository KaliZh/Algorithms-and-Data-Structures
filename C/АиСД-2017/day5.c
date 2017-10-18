#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 10

typedef struct Item {
    int key;
    char *info;
    int used;
} Item;

void init_item(Item *item) {
    item->used = 0;
    item->info = NULL;
}

void make_item(Item *item, int k, char *str) {
    item->key = k;
    item->info = (char*)malloc(strlen(str) * sizeof(char));
    strcat(item->info, str);
    item->used = 1;
}

typedef struct Table {
    int N;
    Item I[SIZE];
} Table;

Table *alloc_table() {
    Table *T = (Table*)malloc(sizeof(Table));
    int i;
    for (i = 0; i < SIZE; i++)
        init_item(&T->I[i]);
    T->N = 0;
    return T;
}

void sort_table(Table *T) {
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = i + 1; j < SIZE; j++)
            if ((T->I[i].used && T->I[j].used) && (T->I[i].key < T->I[j].key)) {
                Item Itemp = T->I[i];
                T->I[i] = T->I[j];
                T->I[j] = Itemp;
            }
}

int add_table(Table *T, int k, char *str) {
    if (T->N < SIZE) {
        int i;
        for (i = 0; i < SIZE; i++)
            if (T->I[i].key == k && T->I[i].used) {
                printf("Key is already used!\n");
                return 0;
            }
        for (i = 0; i < SIZE; i++)
            if (!T->I[i].used)
                break;
        make_item(&T->I[i], k, str);
        T->N++;
        return 1;
    }
    printf("Table overflow!\n");
    return 0;
}

void del_table(Table *T, int kf, int kl) {
    int i; 
    for (i = 0; i < SIZE; i++)
        if ((T->I[i].key >= kf) && (T->I[i].key <= kl) && T->I[i].used) {
            init_item(&T->I[i]);
            --T->N;
        }
}

Table *find_table(Table *T, int kf, int kl) {
    Table *T1 = alloc_table();
    int i;
    for (i = 0; i < SIZE; i++)
        if ((T->I[i].key >= kf) && (T->I[i].key <= kl) && T->I[i].used)
            add_table(T1, T->I[i].key, T->I[i].info);
    return T1;
}

void print_table(Table *T) {
    printf("Table:\n");
    int i, c = 0;
    for (i = 0; i < SIZE; i++)
        if (T->I[i].used) {
            printf("%d %s\n", T->I[i].key, T->I[i].info);
            c = 1;
        }
    if (!c) printf("empty.\n");
}

char *get_string() {
    scanf("%*c");
    char *p = (char*)malloc(sizeof(char));
    char buf[101];
    int n, len = 0;
    *p = '\0';
    do {
        n = scanf("%100[^\n]", buf);
        if (n < 0) {
            free(p);
            p = NULL;
            continue;
        }
        if (!n) {
            scanf("%*c");
        } else {
            len += strlen(buf);
            p = (char*)realloc(p, len + 1);
            strcat(p, buf);
        }
    } while (n > 0);
    return p;
}

int get_int(int *a, int minn, int maxx) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Enter integer!\n");
            scanf("%*[^\n]");
        } else if (*a < minn || *a > maxx) {
            n = 0;
            printf("Integer should be in range [%d, %d].\n", minn, maxx);
            scanf("%*[^\n]");
        }
    } while (!n);
    return 1;
}

void add(Table *T) {
    printf("Enter the key.\n");
    int k;
    if (!get_int(&k, -100, 100)) {
        printf("Error!\n");
        return;
    }
    printf("Enter some information.\n");
    //char str[50];
    //scanf("%s", str);
    char *str = get_string();
    if (!add_table(T, k, str)) {
        printf("Try again.\n");
        return;
    }
    printf("Added.\n");
    sort_table(T);
}

void del(Table *T) {
    int kf, kl;
    printf("Enter the first key.\n");
    if (!get_int(&kf, -100, 100)) {
        printf("Error!\n");
        return;
    }
    printf("Enter the last key.\n");
    if (!get_int(&kl, -100, 100)) {
        printf("Error!\n");
        return;
    }
    del_table(T, kf, kl);
    printf("Deleted.\n");
}

void find(Table *T) {
    int kf, kl;
    printf("Enter the first key.\n");
    if (!get_int(&kf, -100, 100)) {
        printf("Error!\n");
        return;
    }
    printf("Enter the last key.\n");
    if (!get_int(&kl, -100, 100)) {
        printf("Error!\n");
        return;
    }
    Table *T1 = find_table(T, kf, kl);
    print_table(T1);
    free(T1);
}

void menu(Table *T) {
    int command;
    do {
        printf("If you want to add some information to table by key - enter 1.\n");
        printf("If you want to delete information from table by range of keys - enter 2.\n");
        printf("If you want to find information in table by range of keys - enter 3.\n");
        printf("If you want to see all information in table - enter 4.\n");
        printf("If you want to switch off the program - enter 0.\n");
        if (!get_int(&command, 0, 4)) {
            printf("Error!\n");
            return;
        }
        if (command == 1) add(T);
        else if (command == 2) del(T);
        else if (command == 3) find(T);
        else if (command == 4) print_table(T);
    } while (command);
}

int main() {
    Table *T = alloc_table();
    menu(T);
    free(T);

    return 0;
}
