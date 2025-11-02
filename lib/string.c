#include "kernel.h"

int strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void sprintf(char *buf, const char *fmt, ...) {
    // Упрощенная реализация для демонстрации
    strcpy(buf, fmt);
}