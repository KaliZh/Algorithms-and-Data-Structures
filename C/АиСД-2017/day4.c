#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct List {
    char c;
    struct List *next;
} List;

void print_list(List *p) {
    for (; p != NULL; p = p->next)
        printf("%c", p->c);
    printf("\n");
}

List *delete_list(List *p) {
    List *tmp = NULL;
    while (p != NULL) {
        tmp = p;
        p = p->next;
        free(tmp);
    }
    return p;
}

List *reverse_string(List *s) {
    List res = {'*', NULL}, *p = &res, *head, *temp;
    int sz = 0;
    while (s != NULL) {
        if (s->c == ' ') {
            p->next = s->next;
            free(s);
            s = p->next;
        } else {
            head = temp = s;
            s = s->next;
            while (s != NULL && s->c != ' ') {
                head->next = s->next;
                s->next = temp;
                temp = s;
                s = head->next;
                sz++;
            }
            p->next = temp;
            p = s;
            if (s != NULL) {
                s = s->next;
                sz++;
            }
        }
    }
    if (sz)
        res.next = (List*)realloc(res.next, sz);
    return res.next;
}

int get_list(List **p) {
    char buf[21], *c;
    List head = {'*', NULL};
    List *last = &head;
    int n, rc = 1;
    do {
        n = scanf("%20[^\n]", buf);
        if (n > 0) {
            for (c = buf; *c != '\0'; ++c) {
                last->next = (List*)malloc(sizeof(List));
                last = last->next;
                last->c = *c;
            }
            last->next = NULL;
            continue;
        } 
        if (n < 0) {
            delete_list(head.next);
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
    List *s, *res;
    int sz = 0;
    while (get_list(&s)) {
        res = reverse_string(s);
        print_list(res);
        delete_list(res);
        sz = 0;
    }

    return 0;
}
