#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse_string(char *s) {
    char *res = (char*)malloc(1);
    int ind, len = 0;
    *res = '\0';
    int i;
    for (i = 0; i < strlen(s); i++)
        if (s[i] != ' ') {
            ind = len;
            while (s[i] != ' ' && s[i] != '\0') {
                i++;
                len++;
            }
            len++;
            res = (char*)realloc(res, len);
            int j = i - 1;
            while (s[j] != ' ' && j > -1) {
                res[ind] = s[j];
                --j;
                ind++;
            }
            res[ind] = ' ';
        }
    res[ind] = '\0';
    return res;
}

char *get_string() {
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
    str = get_string();
    str = reverse_string(str);
    printf("%s", str);

    return 0;
}
