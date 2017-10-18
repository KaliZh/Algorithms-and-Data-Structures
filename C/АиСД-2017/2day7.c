#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
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

struct node {
    int key;
    char *info;
    struct node *parent, *left, *right;
};
 
struct node *newNode(int k, char *str, struct node *p) {
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = k;
    temp->info = (char*)malloc(strlen(str) * sizeof(char));
    strcpy(temp->info, str);
    temp->parent = p;
    temp->left = temp->right = NULL;
    return temp;
}

void freeNode(struct node *node) {
    free(node->info);
    free(node);
}
 
void inorder(struct node *root) {
    if (root != NULL) {
        inorder(root->right);
        printf("%d %s\n", root->key, root->info);
        inorder(root->left);
    }
}
 
struct node* insert(struct node* node, int k, char *str, struct node *p) {
    if (node == NULL) 
        return newNode(k, str, p);
    if (node->key == k) {
        printf("key is used!\n");
        return node;
    }
    p = node;
    if (k < node->key)
        node->left  = insert(node->left, k, str, p);
    else
        node->right = insert(node->right, k, str, p);
    return node;
}
 
struct node * minValueNode(struct node* node) {
    struct node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}
 
struct node* deleteNode(struct node* root, int key) {
    if (root == NULL) 
        return root;
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            struct node *temp = root->right;
            freeNode(root);
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            freeNode(root);
            return temp;
        }
        struct node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

void findNode(struct node* root, int k) {
    while (root != NULL) {
        if (root->key == k) {
            printf("%d %s\n", root->key, root->info);
            return;
        }
        if (root->key > k)
            root = root->left;
        else root = root->right;
    }
    printf("Couldn't find!\n");
}

void findNode2(struct node* root, int k) {
    struct node temp;
    while (root != NULL) {
        if (root->key == k) {
            printf("%d %s\n", root->key, root->info);
            return;
        }
        temp = *root;
        if (root->key > k)
            root = root->left;
        else root = root->right;
    }
    printf("%d %s\n", temp.key, temp.info);
}

void printNode(struct node* root, int level) {
    int i = level;
    if (root != NULL) {
        printNode(root->right, level + 1);
        while (i-- > 0)
            printf("  ");
        printf("%d\n", root->key);
        printNode(root->left, level + 1);
    }
}

void menu() {
    struct node *root = NULL;
    while(1 == 1) {
        printf("\ninsert - 1\n");
        printf("delete - 2\n");
        printf("find - 3\n");
        printf("find2 - 4\n");
        printf("print - 5\n");
        printf("bye - 0\n");
        int temp;
        if (!get_int(&temp, 0, 5)) 
            return;
        if (temp == 1) {
            printf("key: ");
            int k;
            if (!get_int(&k, 0, 100)) 
                return;
            printf("info: ");
            //char *str = get_string();
            char str[50];
            scanf("%s", str);
            root = insert(root, k, str, NULL);
        } else if (temp == 2) {
            int k;
            printf("key: ");
            if (!get_int(&k, 0, 100))
                return;
            root = deleteNode(root, k);
        } else if (temp == 3) {
            int k;
            printf("key: ");
            if (!get_int(&k, 0, 100))
                return;
            findNode(root, k);
        } else if (temp == 4) {
            int k;
            printf("key: ");
            if (!get_int(&k, 0, 100))
                return;
            findNode2(root, k);
        } else if (temp == 5) {
            inorder(root);
            printNode(root, 0);
        } else {
            return;
        }
    }
}
 
int main()
{
    /*struct node *root = NULL;
    char str[] = "aa";
    root = insert(root, 50, str, NULL);
    root = insert(root, 30, str, NULL);
    root = insert(root, 20, str, NULL);
    root = insert(root, 40, str, NULL);
    root = insert(root, 70, str, NULL);
    root = insert(root, 60, str, NULL);
    root = insert(root, 80, str, NULL);
 
    inorder(root);
    printf("\n");

    root = insert(root, 80, str, NULL);
 
    inorder(root);
    printf("\n");

    findNode(root, 50);
    findNode(root, 60);
    findNode(root, 90);
    printf("\n");

    findNode2(root, 50);
    findNode2(root, 60);
    findNode2(root, 90);
    printf("\n");
 
    printf("\nDelete 20\n");
    root = deleteNode(root, 20);
    inorder(root);
 
    printf("\nDelete 30\n");
    root = deleteNode(root, 30);
    inorder(root);
 
    printf("\nDelete 50\n");
    root = deleteNode(root, 50);
    inorder(root);
    printNode(root, 0);
    printf("\n");*/
    menu();
 
    return 0;
}
