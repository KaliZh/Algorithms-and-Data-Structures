#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    int key;
    char *info;
    struct Node *parent, *left, *right;
} Node;

Node *new_Node(int k, char *str, Node *p) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = k;
    node->info = (char*)malloc(strlen(str) * sizeof(char));
    strcat(node->info, str);
    node->parent = p;
    node->left = node->right = NULL;
    return node;
}

void free_Node(Node *node) {
    free(node->info);
    free(node->parent);
    free(node->left);
    free(node->right);
    free(node);
}

typedef struct Tree {
    int N;
    Node *root;
} Tree;

Tree *init_Tree() {
    Tree *T = (Tree*)malloc(sizeof(Tree));
    T->root = NULL;
    T->N = 0;
    return T;
}

int insert_Node(Node **p_Node, int k, char *str, Node *p) {
    while ((*p_Node) != NULL) {
        if ((*p_Node)->key == k)
            return 0;
        p = (*p_Node);
        if ((*p_Node)->key > k)
            p_Node = &((*p_Node)->left);
        else p_Node = &((*p_Node)->right);
    }
    (*p_Node) = new_Node(k, str, p);
    printf("%d %s\n", (*p_Node)->key, (*p_Node)->info);
    if ((*p_Node)->parent != NULL)
        printf("parent-> %d %s\n", (*p_Node)->parent->key, (*p_Node)->parent->info);
    return 1;
}

Node *delete_Node(Node **p_Node, int k) {
    if ((*p_Node) == NULL) 
        return (*p_Node);
    if (k < (*p_Node)->key) {
        (*p_Node)->left = delete_Node(&((*p_Node)->left), k);
    } else if (k > (*p_Node)->key) {
        (*p_Node)->right = delete_Node(&((*p_Node)->right), k);
    } else {
        if ((*p_Node)->left == NULL) {
            Node *temp = (*p_Node)->right;
            free_Node((*p_Node));
            return temp;
        } else if ((*p_Node)->right == NULL) {
            Node *temp = (*p_Node)->left;
            free_Node((*p_Node));
            return temp;
        }
        Node **temp = &((*p_Node)->right);
        while ((*temp)->left != NULL)
            temp = &((*temp)->left);
        (*p_Node)->key = (*temp)->key;
        (*p_Node)->info = (*temp)->info;
        (*p_Node)->right = delete_Node(&((*p_Node)->right), (*temp)->key);
    }
    return *p_Node;
}
/*Node *delete_Node(Node *node, int k) {
    if (node == NULL)
        return node;
    if (k < node->key) {
        node->left = delete_Node(node->left, k);
    } else if (k > node->key) {
        node->right = delete_Node(node->right, k);
    } else {
        if (node->left == NULL) {
            Node *temp = node->right;
            free_Node(node);
            return temp;
        } else if (node->right == NULL) {
            Node *temp = node->left;
            free_Node(node);
            return temp;
        }
        Node *temp = node->right;
        while (temp->left != NULL)
            temp = temp->left;
        node->key = temp->key;
        node->info = temp->info;
        printf("delte el: %d %s\n", node->key, node->info);
        node->right = delete_Node(node->right, temp->key);
    }
    return node;
}*/

void print_Tree(Node *node) {
    if (node == NULL)
        return;
    print_Tree(node->right);
    printf("%d %s\n", node->key, node->info);
    print_Tree(node->left);
}

int main() {
    Tree *T = init_Tree();
    char str[] = "aaa";
    if (insert_Node(&T->root, 23, str, NULL))
        T->N++;
    if (insert_Node(&T->root, 23, str, NULL))
        T->N++;
    if (insert_Node(&T->root, 11, str, NULL))
        T->N++;
    if (insert_Node(&T->root, 9, str, NULL))
        T->N++;
    if (insert_Node(&T->root, 44, str, NULL))
        T->N++;
    if (insert_Node(&T->root, 1, str, NULL))
        T->N++;
    printf("%d\n", T->N);
    print_Tree(T->root);
    T->root = delete_Node(&T->root, 23);
    --T->N;
    printf("%d\n", T->N);
    print_Tree(T->root);
    T->root = delete_Node(&T->root, 11);
    --T->N;
    printf("%d\n", T->N);
    print_Tree(T->root);
}
