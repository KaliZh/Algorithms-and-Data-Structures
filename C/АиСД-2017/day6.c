#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *get_string() {
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

typedef struct Item {
    int used;
    int key;
    int len;
    int offset;
} Item;

typedef struct Table {
    int SIZE;
    int N;
    Item *I;
    FILE *ptr_FILE;
} Table;

void save_table(Table *T) {
    fseek(T->ptr_FILE, sizeof(int), SEEK_SET);
    fwrite(&T->N, sizeof(int), 1, T->ptr_FILE);
    fwrite(T->I, sizeof(Item), T->N, T->ptr_FILE);
    fclose(T->ptr_FILE);
    free(T->I);
    free(T);
}

int load_table(Table *T, char *fname) {
    T->ptr_FILE = fopen(fname, "r+b");
    if (!T->ptr_FILE)
        return 0;
    fread(&T->SIZE, sizeof(int), 1, T->ptr_FILE);
    fread(&T->N, sizeof(int), 1, T->ptr_FILE);
    T->I = (Item*)calloc(T->SIZE, sizeof(Item));
    fread(T->I, sizeof(Item), T->N, T->ptr_FILE);
    return 1;
}

int create_table(Table *T, char *fname, int sz) {
    T->SIZE = sz;
    T->N = 0;
    T->ptr_FILE = fopen(fname, "w+b");
    if (!T->ptr_FILE) {
        T->I = NULL;
        return 0;
    }
    T->I = (Item*)calloc(T->SIZE, sizeof(Item));
    fwrite(&T->SIZE, sizeof(int), 1, T->ptr_FILE);
    fwrite(&T->N, sizeof(int), 1, T->ptr_FILE);
    fwrite(T->I, sizeof(Item), T->SIZE, T->ptr_FILE);
    return 1;
}

Table *init_table() {
    Table *T = (Table*)malloc(sizeof(Table));
    char *fname = NULL;
    printf("Enter file name.\n");
    fname = get_string();
    if (!fname)
        return NULL;
    if(!load_table(T, fname)) {
        printf("Enter the size of table.\n");
        int SZ;
        if (!get_int(&SZ, 0, 100))
            return NULL;
        if (!create_table(T, fname, SZ))
            return NULL;
    }
    free(fname);
    return T;
}

void make_item(Table *T, Item *item, int k, char *str) {
    item->used = 1;
    item->key = k;
    item->len = strlen(str) + 1; 
    fseek(T->ptr_FILE, 0, SEEK_END);
    item->offset = ftell(T->ptr_FILE);
    fwrite(str, sizeof(char), item->len, T->ptr_FILE);
    T->N++;
}

int add_table(Table *T, int k, char *str) {
    if (T->N < T->SIZE) {
        int i;
        for (i = 0; i < T->SIZE; i++)
            if (T->I[i].key == k && T->I[i].used) {
                printf("Key is already used!\n");
                return 0;
            }
        for (i = 0; i < T->SIZE; i++)
            if (!T->I[i].used)
                break;
        make_item(T, &T->I[i], k, str);
        return 1;
    }
    printf("Table overflow!\n");
    return 0;
}

void del_table(Table *T, int k_first, int k_last) {
    int i; 
    for (i = 0; i < T->SIZE; i++)
        if ((T->I[i].key >= k_first) && (T->I[i].key <= k_last) && T->I[i].used)
            T->I[i].used = 0;
}

Table *find_table(Table *T, int k_first, int k_last) {
    Table *T1 = (Table*)malloc(sizeof(Table));
    T1->SIZE = T->SIZE;
    T1->N = 0;
    T1->I = (Item*)calloc(T1->SIZE, sizeof(Item));
    T1->ptr_FILE = T->ptr_FILE;
    int i;
    for (i = 0; i < T->SIZE; i++)
        if ((T->I[i].key >= k_first) && (T->I[i].key <= k_last) && T->I[i].used) {
            T1->I[T1->N].used = T->I[i].used;
            T1->I[T1->N].key = T->I[i].key;
            T1->I[T1->N].len = T->I[i].len;
            T1->I[T1->N++].offset = T->I[i].offset;
        }
    return T1;
}

void print_table(Table *T) {
    printf("Table:\n");
    int i, c = 0;
    for (i = 0; i < T->SIZE; i++)
        if (T->I[i].used) {
            char *info = (char*)malloc(T->I[i].len);
            fseek(T->ptr_FILE, T->I[i].offset, SEEK_SET);
            fread(info, sizeof(char), T->I[i].len, T->ptr_FILE);
            printf("%d %s\n", T->I[i].key, info); 
            free(info);
            c = 1;
        }
    if (!c) printf("empty.\n");
}

void add(Table *T) {
    printf("Enter the key.\n");
    int k;
    if (!get_int(&k, -100, 100)) {
        printf("Error!\n");
        return;
    }
    printf("Enter some information.\n");
    char str[50];
    scanf("%s", str);
    //char *str = get_string();
    if (!add_table(T, k, str)) {
        printf("Try again.\n");
        return;
    }
    //free(str);
    printf("Added.\n");
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
    int k_first, k_last;
    printf("Enter the first key.\n");
    if (!get_int(&k_first, -100, 100)) {
        printf("Error!\n");
        return;
    }
    printf("Enter the last key.\n");
    if (!get_int(&k_last, -100, 100)) {
        printf("Error!\n");
        return;
    }
    Table *T1 = find_table(T, k_first, k_last);
    print_table(T1);
    free(T1->I);
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
    Table *T = init_table();
    if (T) {
        menu(T);
        save_table(T);
    } else { 
        printf("Error!\n");
    }

    return 0;
}
