
#include <stdio.h>
#include <stdlib.h>

void repeat_char(char c, int count) {
    for (int i = 0; i < count; i++)
        putchar(c);
}

int calculate_max_width(int size) {
    int stars = 1;
    int max = 1;
    int shift = 4;

    for (int stage = 0; stage < size; stage++) {
        int lines = 4 + stage;
        if (stage > 0) {
            stars -= shift;
            if (stage % 2 == 0)
                shift += 2;
        }

        for (int i = 0; i < lines; i++) {
            if (stars > max)
                max = stars;
            stars += 2;
        }
    }
    return max;
}

void print_star_line(int stars, int max_width) {
    int spaces = (max_width - stars) / 2;
    repeat_char(' ', spaces);
    repeat_char('*', stars);
    putchar('\n');
}

void print_trunk(int size, int max_width) {
    int trunk_width = size;

    int spaces = (max_width - trunk_width) / 2;
    if (size % 2 == 0) {
        // For even sizes, shift trunk one space right
        spaces += 1;
    }

    for (int i = 0; i < size; i++) {
        repeat_char(' ', spaces);
        repeat_char('|', trunk_width);
        putchar('\n');
    }
}

void print_tree(int size) {
    if (size <= 0)
        return;

    int stars = 1;
    int max_width = calculate_max_width(size);
    int shift = 4;

    for (int stage = 0; stage < size; stage++) {
        int lines = 4 + stage;

        if (stage > 0) {
            stars -= shift;
            if (stage % 2 == 0)
                shift += 2;
        }

        for (int i = 0; i < lines; i++) {
            print_star_line(stars, max_width);
            stars += 2;
        }
    }

    print_trunk(size, max_width);
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        return 1;

    int size = atoi(argv[1]);
    if (size < 0 || size > 9)
        return 0;

    print_tree(size);

    return 0;
}
