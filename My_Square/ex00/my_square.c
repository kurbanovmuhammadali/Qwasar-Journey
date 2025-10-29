#include <stdio.h>
#include <stdlib.h>

void my_square(int x, int y)
{
    int length, height;
    if (x <= 0 || y <= 0) printf("");

    for ( height = 0; height < y; height++){
        for ( length = 0; length < x; length++){
            if (height == 0 || height == y - 1)

            {
                if (length == 0 || length == x - 1)
                    putchar('o');
                else
                    putchar('-');
            }

            else

            {
                if (length == 0 || length == x - 1)
                    putchar('|');
                else
                    putchar(' ');
            }
        }
        putchar('\n');
    }
}
int main(int ac, char **av){

    if (ac != 3) return 0;

    int x = atoi(av[1]);
    int y = atoi(av[2]);

    if (x <= 0 || y <= 0) printf("");

    my_square(x, y);
    
    
    return 0;
}
