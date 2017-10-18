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
    char *info;
    struct Node *next;
} Node;

void initNode(Node *node) {
    node->info = NULL;
    node->next = NULL;
}

Node *newNode(int k, char *str, Node *pNode) {
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->key = k;
    temp->info = (char*)malloc(strlen(str) * sizeof(char));
    strcpy(temp->info, str);
    temp->next = pNode;
    return temp;
}

void freeNode(Node *node) {
    while (node) {
        Node *temp = node->next;
        free(node->info);
        free(node);
        node = temp;
    }
}

void printNode(Node *node) {
    for (; node != NULL; node = node->next)
        printf("%d %s\n", node->key, node->info);
}

typedef struct hashTable {
    Node node[SIZE];
} hashTable;

void initTable(hashTable *T) {
    int i;
    for (i = 0; i < SIZE; i++)
        initNode(&T->node[i]);
}

void freeTable(hashTable *T) {
    int i;
    for (i = 0; i < SIZE; i++)
        freeNode(T->node[i].next);
}

int hash(int k) {
    return k % SIZE;
}

int insertNode(hashTable *T, int k, char *str) {
    int n = hash(k);
    if (!T->node[n].next) {
        T->node[n].next = newNode(k, str, NULL);
        return 1;
    }
    Node *temp = T->node[n].next;
    while (temp) {
        if (temp->key == k)
            return 0;
        temp = temp->next;
    }
    T->node[n].next = newNode(k, str, T->node[n].next);
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
    initTable(R);
    int n = hash(k);
    Node *temp = T->node[n].next;
    while (temp) {
        insertNode(R, temp->key, temp->info);
        temp = temp->next;
    }
    return R;
}

void printTable(hashTable *T) {
    int i, c = 0;
    for (i = 0; i < SIZE; i++)
        if (T->node[i].next) {
            printNode(T->node[i].next);
            c = 1;
        }
    if (!c)
        printf("Table is empty.\n");
}

int main() {
    hashTable T;
    initTable(&T);
    /*char str[] = "aaa";
    if (insertNode(&T, 23, str))
        printf("added!\n");
    else printf("fail!\n");
    if (insertNode(&T, 3, str))
        printf("added!\n");
    else printf("fail!\n");
    if (insertNode(&T, 2, str))
        printf("added!\n");
    else printf("fail!\n");
    if (insertNode(&T, 23, str))
        printf("added!\n");
    else printf("fail!\n");
    printTable(&T);
    printf("\n");
    if (deleteNode(&T, 43))
        printf("deleted!\n");
    else printf("fail!\n");
    if (deleteNode(&T, 33))
        printf("deleted!\n");
    else printf("fail!\n");
    if (deleteNode(&T, 22))
        printf("deleted!\n");
    else printf("fail!\n");
    if (deleteNode(&T, 23))
        printf("deleted!\n");
    else printf("fail!\n");
    printTable(&T);
    printf("\n");
    hashTable *R = findNodes(&T, 43);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 33);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 22);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 23);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 2);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 3);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 5);
    printTable(R);
    printf("\n");
    R = findNodes(&T, 7);
    printTable(R);
    printf("\n");
    freeTable(R);
    free(R);
    freeTable(&T);*/
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
            if (insertNode(&T, k, str))
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
            if (deleteNode(&T, k))
                printf("Success!\n");
            else printf("Fail!\nKey is not found.\n");
        } else if (n == 3) {
            printf("Enter key: ");
            int k;
            if (!getInt(&k, -100, 100)) {
                printf("Error!\n");
                break;
            }
            hashTable *R = findNodes(&T, k);
            printTable(R);
            freeTable(R);
            free(R);
        } else if (n == 4) {
            printTable(&T);
        }
    } while (n);
    freeTable(&T);

    return 0;
}
