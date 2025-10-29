#include "helpers.h"

int str_len(const char *str) {
    int count = 0;
    while (str[count]) count++;
    return count;
}

int str_compare(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int contains_duplicates(const char *str) {
    for (int i = 0; str[i]; i++) {
        for (int j = i + 1; str[j]; j++) {
            if (str[i] == str[j]) return 1;
        }
    }
    return 0;
}

int is_char_valid(char c) {
    return (c >= '0' && c <= '8');
}
