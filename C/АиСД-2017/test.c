#include <stdlib.h>
#include <stdio.h>

int main() {
    FILE *p_FILE = fopen("test.txt", "r+b");
    char str[11];
    fgets(str, 11, p_FILE);
    puts(str);
    fseek(p_FILE, 0, SEEK_SET);
    fwrite("abc", sizeof(char), 3, p_FILE);
    fclose(p_FILE);

    return 0;
}
