// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define ASCII_SIZE 128  

// int main(int argc, char *argv[])
// {
//     if (argc < 2) {return 0;}
//     int freq[ASCII_SIZE] = {0};

//     for (int i = 0; i < argc; i++) 
//     {
//         char *text = argv[i];

//         while(*text) 
//         {
//             unsigned char trup = *text;
//             if (trup < ASCII_SIZE)
//             {
//                 freq[trup]++;
// //             }
// //             text++;
// //         }
// //     }

// //     for (int i = 0; i < ASCII_SIZE; i++) {
// //         if (freq[i] > 0) {
// //             if (i == ' ') {
// //                 printf(" :%d\n", freq[i]);
// //             } else {
// //                 printf("%c:%d\n", i, freq[i]);
// //             }
// //         }
// //     }

// //     return 0;
// // }

// #include <unistd.h>
// #define MAX_CHAR 256

// void my_ngram(char c, int count) {
//     char buf[12];
//     int i = 0;
//     if (count == 0) return;
//     write(1, &c, 1);
//     write(1, ":", 1);
//     if (count == 0) {
//         buf[i++] = '0';
//     } else {
//         int j = 0;
//         while (count > 0) {
//             buf[i++] = '0' + (count % 10);
//             count /= 10;
//         }
//         for (j = 0; j < i / 2; j++) {
//             char temp = buf[j];
//             buf[j] = buf[i - j - 1];
//             buf[i - j - 1] = temp;
//         }
//     }
//     write(1, buf, i);
//     write(1, "\n", 1);
// }
// int main(int argc, char **argv) {
//     int count[MAX_CHAR] = {0};  
//     for (int i = 1; i < argc; i++) {
//         char *str = argv[i];
//         while (*str) {
//             count[(unsigned char)*str]++;
//             str++;
//         }
//     }
//     for (int i = 0; i < MAX_CHAR; i++) {
//         if (count[i] > 0) {
//             my_ngram((char)i, count[i]);
//         }
//     }

//     return 0;
// }




#include <unistd.h>
#define ASCII_SIZE 256
void my_ngram(char character, int count)
{
    char buffer[20];
    int i = 0; int j = 0;

    if(count == 0) return;
    write(1, &character, 1);
    write(1, ":", 1);

    if(count == 0){buffer[i++] = '0';}
    else {        
            while(count > 0)
            {
                buffer[i++] = '0' + (count % 10);
                count /= 10;
            }
            for(j = 0; j < i / 2; j++){
                char temporary = buffer[j];
                buffer[j] = buffer[i - j - 1];
                buffer[ i - j - 1] = temporary;
            }
         }
         write(1, buffer, i);
         write(1, "\n", 1);

}


int main(int argc, char **argv)
{
    int count[ASCII_SIZE] = {0};
    for ( int i = 1; i < argc; i++){
        char *string = argv[i];
        while(*string){ 
            count[(unsigned char)*string]++;
            string++;
        }
    }
    for(int i = 0; i < ASCII_SIZE; i++){
        if(count[i] > 0){
            my_ngram((char)i, count[i]);
        }
    }
    return 0;

}