
// #include <unistd.h>
// #include <stdarg.h>
// #include <stdlib.h>

// void print_terminal(char c){
//     write(1, &c, 1);
// }

// void reading_string(const char *s){
//       while (*s) print_terminal(*s++);
// }

// int my_str_len(const char *s){
//     int len = 0;
//     while (s[len]) len++;
//     return len;
// }

// void my_reverse(char *string, int length){
//     for(int i =0; i< length/2; i++){
//      char temporary = string[i];
//      string[i]= string[length - i - 1];
//      string[length- i - 1] = temporary;
     
//     }
// }

// char *itoa_base(unsigned long number, int basename, int is_signed, int use_uppercase){
//     char *lower_digits = "0123456789abcdef";
//     char *upper_digits = "0123456789ABCDEF";
//     const char *digits = use_uppercase ? upper_digits : lower_digits; 

//     char *buffer = malloc(65); // for storing memory for string 
//     if (!buffer) return NULL;

//     int i = 0;
//     int negative = 0;

//     if (is_signed && (long)number < 0) {
//         negative = 1; //check if its is negative if yes it puts '-' before the number
//         number = -(long)number;
//     }

//     if (number == 0) {
//         buffer[i++] = '0'; //show 0 if the n is 0
//     } else {
//         while (number) {
//             buffer[i++] = digits[number % basename];// converts numbers into the strings
//             number /= basename;
//         }
//         if (negative)
//             buffer[i++] = '-';
//     }

//     buffer[i] = '\0'; //adds nulterminate
//     my_reverse(buffer, i);//makes the digits that we converted in reverse order
//     return buffer; //returns the string
// }

// int my_printf(const char *format, ...) {
//     va_list arguments; //make a list for adding a varibale in printf function
//     va_start(arguments, format); //stars reading an argument and fmt or thing inside "" in the printf
//     int total = 0; // counts the total number of character printed

//     while (*format) {
//         if (*format == '%' && *(format + 1)) {//if it has sometinhg after % then we do the sorting part
//             format++;
//             char *s = NULL;//for holding converted string

//             if (*format == 'd') s = itoa_base(va_arg(arguments, int), 10, 1, 0);//for %d we use itobase function and vaarg for reading one argument at a time
//             else if (*format == 'u') s = itoa_base(va_arg(arguments, unsigned int), 10, 0, 0);
//             else if (*format == 'o') s = itoa_base(va_arg(arguments, unsigned int), 8, 0, 0);
//             else if (*format == 'x') s = itoa_base(va_arg(arguments, unsigned int), 16, 0, 0); // lowercase for %x
//             else if (*format == 'X') s = itoa_base(va_arg(arguments, unsigned int), 16, 0, 1);
//             else if (*format == 'p') {
//                 unsigned long p = (unsigned long)va_arg(arguments, void *);//converts pointer to string in base 16 which means hex(#ff0000)
//                 reading_string("0x");//adds '0x' first for %p
//                 total += 2;
//                 s = itoa_base(p, 16, 0, 0); // Lowercase for %p
//             }
//             else if (*format == 'c') {//for one character
//                 print_terminal(va_arg(arguments, int));//gets only one character
//                 total++; format++; continue;//print it immediatly and skip other part of the loop eith the continue command;
//             }
//             else if (*format == 's') {//gets string
//                 char *str = va_arg(arguments, char *);
//                 if (!str) str = "(null)";//if it is null prints null
//                 reading_string(str);
//                 total += my_str_len(str); format++; continue;// adds to total
//             }

//             if (s) {
//                 reading_string(s);//to print string
//                 total += my_str_len(s);
//                 free(s);//free memory creatred by itoa
//             }
//         } else {
//             print_terminal(*format);//if it is a normal character without %
//             total++;
//         }
//         format++;// why ?
//     }

//     va_end(arguments);// ends the list clear its
//     return total;//return the output
// }

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

void print_terminal(char c) {
    write(1, &c, 1);
}

void reading_string(const char *s) {
    while (*s) print_terminal(*s++);
}

int my_str_len(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

void my_reverse(char *string, int length) {
    for (int i = 0; i < length / 2; i++) {
        char temporary = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = temporary;
    }
}

char *itoa_base(unsigned long number, int basename, int is_signed, int use_uppercase) {
    char *lower_digits = "0123456789abcdef";
    char *upper_digits = "0123456789ABCDEF";
    const char *digits = use_uppercase ? upper_digits : lower_digits;

    char *buffer = malloc(65);
    if (!buffer) return NULL;

    int i = 0;
    int negative = 0;

    if (is_signed && (long)number < 0) {
        negative = 1;
        number = -(long)number;
    }

    if (number == 0) {
        buffer[i++] = '0';
    } else {
        while (number) {
            buffer[i++] = digits[number % basename];
            number /= basename;
        }
        if (negative)
            buffer[i++] = '-';
    }

    buffer[i] = '\0';
    my_reverse(buffer, i);
    return buffer;
}

int my_printf(const char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    int total = 0;

    while (*format) {
        if (*format == '%' && *(format + 1)) {
            format++;
            char *s = NULL;

            if (*format == 'd') {
                s = itoa_base(va_arg(arguments, int), 10, 1, 0);
            } else if (*format == 'u') {
                s = itoa_base(va_arg(arguments, unsigned int), 10, 0, 0);
            } else if (*format == 'o') {
                s = itoa_base(va_arg(arguments, unsigned int), 8, 0, 0);
            } else if (*format == 'x') {
                s = itoa_base(va_arg(arguments, unsigned int), 16, 0, 0); // lowercase hex
            } else if (*format == 'X') {
                s = itoa_base(va_arg(arguments, unsigned int), 16, 0, 1); // uppercase hex
            } else if (*format == 'p') {
                unsigned long p = (unsigned long)va_arg(arguments, void *);
                reading_string("0x");
                total += 2;
                s = itoa_base(p, 16, 0, 0); // lowercase hex
                if (s) {
                    reading_string(s);
                    total += my_str_len(s);
                    free(s); //fix: prevent memory leak
                }
                format++;
                continue; //fix: skip rest of loop
            } else if (*format == 'c') {
                print_terminal(va_arg(arguments, int));
                total++;
                format++;
                continue;
            } else if (*format == 's') {
                char *str = va_arg(arguments, char *);
                if (!str) str = "(null)"; //fix: avoid NULL crash
                reading_string(str);
                total += my_str_len(str);
                format++;
                continue;
            }

            if (s) {
                reading_string(s);
                total += my_str_len(s);
                free(s); //fix: free allocated memory
            }
        } else {
            print_terminal(*format);
            total++;
        }
        format++;
    }

    va_end(arguments);
    return total;
}