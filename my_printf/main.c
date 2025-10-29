#include <stdio.h>

int my_printf(const char *format, ...); //your function

int main() {
    my_printf("Hello %s!\n", NULL);
    my_printf("Number: %d, Hex: %x, HEX: %X, Oct: %o, Unsigned: %u\n", -123, 255, 255, 255, 255);
    my_printf("Pointer: %p\n", &main);
    my_printf("Char: %c\n", 'A');
    return 0;
}