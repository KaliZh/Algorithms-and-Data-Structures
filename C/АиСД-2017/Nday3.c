#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void work(char *str) {
    char *ch = str;
    int c = 0;
    while (*ch != '\0') {
        if (*ch != ' ') {
            c = 1;
            break;
        }
        ch++;
    }
    if (!c)
        return;
    char *p = (char*)malloc(strlen(str));
    char *s = str, *res = p;
    int len = 0;
    char f = '\0';
    while (*s != '\0') {
        if (*s != ' ' && f == '\0')
            f = *s;
        if (*s == f) {
            while (*s != ' ' && *s != '\0') {
                *p = *s;
                len++;
                p++;
                s++;
            }
        *p = ' ';
        len++;
        p++;
        } else if (*s == ' ') {
            s++;
        } else {
            while (*s != ' ' && *s != '\0')
                s++;
        }
    }
    *p = '\0';
    res = (char*)realloc(res, len);
    strcpy(str, res);
}

char *get() {
    char *p = (char*)malloc(1);
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

int main() {
    char *str;
    str = get();
    work(str);
    printf("%s", str);

    return 0;
}
