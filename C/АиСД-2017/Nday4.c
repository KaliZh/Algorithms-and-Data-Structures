#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct List {
    char c;
    struct List *next;
} List;

void Print(List *p) {
    for (; p != NULL; p = p->next)
        printf("%c", p->c);
    printf("\n");
}

List *DeleteList(List *p) {
    List *tmp = NULL;
    while (p != NULL) {
        tmp = p;
        p = p->next;
        free(tmp);
    }
    return p;
}

List *Work(List *s) {
    List res = {'*', s}, *p = &res, *tmp;
    char f = '\0';
    while (s != NULL) {
        if (s->c == ' ') {
            tmp = s;
            s = s->next;
            free(tmp);
            p->next = s;
            continue;
        } else if (f == '\0') {
            f = s->c;
        }
        if (f != '\0' && s->c == f) {
            while (s != NULL && s->c != ' ')
                s = s->next;
            p = s;
            if (s != NULL)
                s = s->next;
            continue;
        }
        if (f != '\0' && s->c != f) {
            while (s != NULL && s->c != ' ') {
                tmp = s;
                s = s->next;
                free(tmp);
            }
            if (s != NULL) {
                tmp = s;
                s = s->next;
                free(tmp);
            }
            p->next = s;
        }
    }
    return res.next;
}

int Get(List **p) {
    char buf[101], *c;
    List head = {'*', NULL};
    List *last = &head;
    int n, rc = 1;
    do {
        n = scanf("%100[^\n]", buf);
        if (n > 0) {
            for (c = buf; *c != '\0'; c++) {
                last->next = (List*)malloc(sizeof(List));
                last = last->next;
                last->c = *c;
            }
            last->next = NULL;
        } else if (n < 0) {
            DeleteList(head.next);
            head.next = NULL;
            rc = 0;
        } else {
            scanf("%*c");
        }
    } while (n > 0);
    *p = head.next;
    return rc;
}


int main() {
    List *str;
    while (Get(&str)) {
        str = Work(str);
        Print(str);
    }

    return 0;
}
