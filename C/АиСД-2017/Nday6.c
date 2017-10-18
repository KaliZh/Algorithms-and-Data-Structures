#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 10

char *getStr() {
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

int getInt(int *a, int minn, int maxx) {
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
    scanf("%*c");
    return 1;
}

typedef struct Node {
    int key;
    int offset;
    int len;
    struct Node *next;
} Node;

void initNode(Node *node) {
    node->next = NULL;
}

Node *newNode(int k, char *str, Node *pNode, FILE *file) {
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->key = k;
    temp->len = strlen(str) + 1;
    fseek(file, 0, SEEK_END);
    temp->offset = ftell(file);
    temp->next = pNode;
    return temp;
}

void freeNode(Node *node) {
    while (node) {
        Node *temp = node->next;
        free(node);
        node = temp;
    }
}

void printNode(Node *node, FILE *file) {
    for (; node; node = node->next) {
        char *temp = (char*)malloc(node->len);
        fseek(file, node->offset, SEEK_SET);
        fread(temp, sizeof(char), node->len, file);
        printf("%d %s\n", node->key, temp);
        free(temp);
    }
}

typedef struct hashTable {
    Node node[SIZE];
    char *ftabname;
    FILE *file;
} hashTable;

int hash(int k) {
    return k % SIZE;
}

void copyNode(hashTable *T, Node *node) {
    int n = hash(node->key);
    if (!T->node[n].next) {
        T->node[n].next = (Node*)malloc(sizeof(Node));
        T->node[n].next->key = node->key;
        T->node[n].next->len = node->len;
        T->node[n].next->offset = node->offset;
        T->node[n].next->next = NULL;
    } else {
        Node *temp = T->node[n].next; 
        T->node[n].next = NULL;
        T->node[n].next = (Node*)malloc(sizeof(Node));
        T->node[n].next->key = node->key;
        T->node[n].next->len = node->len;
        T->node[n].next->offset = node->offset;
        T->node[n].next->next = temp;
    }
}

int insertNode(hashTable *T, int k, char *str) {
    int n = hash(k);
    if (!T->node[n].next) {
        T->node[n].next = newNode(k, str, NULL, T->file);
        fseek(T->file, 0, SEEK_END);
        fwrite(str, sizeof(char), T->node[n].next->len, T->file);
        return 1;
    }
    Node *temp = T->node[n].next;
    while (temp) {
        if (temp->key == k)
            return 0;
        temp = temp->next;
    }
    T->node[n].next = newNode(k, str, T->node[n].next, T->file);
    fseek(T->file, 0, SEEK_END);
    fwrite(str, sizeof(char), T->node[n].next->len, T->file);
    return 1;
}

int deleteNode(hashTable *T, int k) {
    int n = hash(k);
    if (!T->node[n].next)
        return 0;
    Node *last = &T->node[n], *temp = T->node[n].next;
    while (temp) {
        if (temp->key == k) {
            last->next = temp->next;
            temp->next = NULL;
            freeNode(temp);
            return 1;
        }
        last = last->next;
        temp = temp->next;
    }
    return 0;
}

hashTable *findNodes(hashTable *T, int k) {
    hashTable *R = (hashTable*)malloc(sizeof(hashTable));
    int i;
    for (i = 0; i < SIZE; i++)
        initNode(&R->node[i]);
    R->file = T->file;
    R->ftabname = T->ftabname;
    int n = hash(k);
    Node *temp = T->node[n].next;
    while (temp) {
        copyNode(R, temp);
        temp = temp->next;
    }
    return R;
}

void printTable(hashTable *T) {
    int i, c = 0;
    for (i = 0; i < SIZE; i++)
        if (T->node[i].next) {
            printNode(T->node[i].next, T->file);
            c = 1;
        }
    if (!c)
        printf("Table is empty.\n");
}

int saveTable(hashTable *T) {
    fclose(T->file);
    FILE *file = fopen(T->ftabname, "wb");
    if (!file)
        return 0;
    Node *temp = NULL;
    int i;
    for (i = 0; i < SIZE; i++)
        if (T->node[i].next)
            for (temp = T->node[i].next; temp; temp = temp->next)
                fwrite(temp, sizeof(int), 3, file);
    fclose(file);
    return 1;
}

int loadTable(hashTable *T, char *fname) {
    FILE *file = NULL;
    char *fdatname = NULL;
    int len = strlen(fname);
    fdatname = strdup(fname);
    fdatname = (char*)realloc(fdatname, len + 5);
    strcat(fdatname, ".dat");
    T->ftabname = strdup(fname);
    T->ftabname = (char*)realloc(T->ftabname, len + 5); 
    strcat(T->ftabname, ".tab");
    file = fopen(T->ftabname, "rb");
    if (file) {
        T->file = fopen(fdatname, "r+b");
        if (!T->file) {
            free(fdatname);
            return 0;
        }
        Node temp;
        while (fread(&temp, sizeof(int), 3, file)) {
            int n = hash(temp.key);
            Node *last = &T->node[n];
            while (last->next)
                last = last->next;
            last->next = &temp;
            //copyNode(T, &temp);
        }
        fclose(file);
    } else {
        T->file = fopen(fdatname, "w+b");
        if (!T->file) {
            free(fdatname);
            return 0;
        }
    }
    free(fdatname);
    return 1;
}

int initTable(hashTable *T) {
    int i;
    for (i = 0; i < SIZE; i++)
        initNode(&T->node[i]);
    printf("Enter file name: ");
    char *fname = getStr();
    if (!fname || !loadTable(T, fname))
        return 0;
    free(fname);
    return 1;
}

void freeTable(hashTable *T) {
    int i;
    for (i = 0; i < SIZE; i++)
        freeNode(T->node[i].next);
}

void menu(hashTable *T) {
    int n;
    do {
        printf("Enter 1 for inserting information by key to table.\n");
        printf("Enter 2 for deleting information by key from table.\n");
        printf("Enter 3 for getting new table of synonims to given key.\n");
        printf("Enter 4 for printing table.\n");
        printf("Enter 0 for ending program.\n");
        if (!getInt(&n, 0, 4)) {
            printf("Error!\n");
            break;
        }
        if (n == 1) {
            printf("Enter key: ");
            int k;
            if (!getInt(&k, -100, 100)) {
                printf("Error!\n");
                break;
            }
            printf("Enter information: ");
            char *str = getStr();
            if (insertNode(T, k, str))
                printf("Success!\n");
            else printf("Fail!\nKey is busy.\n");
            free(str);
        } else if (n == 2) {
            printf("Enter key: ");
            int k;
            if (!getInt(&k, -100, 100)) {
                printf("Error!\n");
                break;
            }
            if (deleteNode(T, k))
                printf("Success!\n");
            else printf("Fail!\nKey is not found.\n");
        } else if (n == 3) {
            printf("Enter key: ");
            int k;
            if (!getInt(&k, -100, 100)) {
                printf("Error!\n");
                break;
            }
            hashTable *R = findNodes(T, k);
            printTable(R);
            freeTable(R);
            free(R);
        } else if (n == 4) {
            printTable(T);
        }
    } while (n);
}

int main() {
    hashTable T;
    if(!initTable(&T)) {
        freeTable(&T);
        printf("Error!");
        return 0;
    }
    menu(&T);
    if (saveTable(&T))
        freeTable(&T);
    else printf("Error!\n");

    return 0;
}
