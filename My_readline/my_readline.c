#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int READLINE_READ_SIZE = 512;  // can be changed externally

char *g_buffer = NULL;          // leftover buffer between calls

//this function will help us to find the new line 
static int find_newline(char *s)//input as a string
{
    int i = 0;//temp variable
    if (!s)//if strung == NULL return -1
        return -1;
    while (s[i])//while we are going threw each char in the stiring
    {
        if (s[i] == '\n')//if the string ends then return the number of character in the line
            return i;//return its position 
        i++;//i dont know why 
    }
    return -1;//else return -1
}

//function for finding length of the string or implemention of strlen()
static int str_len(char *s)//input
{
    int i = 0;//temp variable
    if (!s)//if the sstring is empty then return 0
        return 0;
    while (s[i])//while we are goind threw each char in the string
        i++;//coiunt it 
    return i;//return number of char

}

//in this function i join two string for what ?
static char *str_join_free(char *s1, char *s2)//two strings input
{
    int len1 = str_len(s1);//by already made function finds its length
    int len2 = str_len(s2);//same thing
    char *new_str = malloc(len1 + len2 + 1);//new string is addition of first and second +1 because '\0' also takes memory
    int i = 0, j = 0;//temp variables

    if (!new_str)//if new_str == NULL
    {
        free(s1);
        return NULL;
    }
    while (i < len1)//while i < length if s1
    {
        new_str[i] = s1[i];//copies s1[i] character to new_string[i] char
        i++;//moves to next char
    }
    while (j < len2)// same thing with second string
    {
        new_str[i + j] = s2[j];
        j++;
    }
    new_str[i + j] = '\0';//adding '\0' to new_string
    free(s1);//making free space cause we dont need s1
    return new_str;
}


void init_my_readline(void)
{
    if (g_buffer)//if variable is okay then 
    {
        free(g_buffer);//making free space
        g_buffer = NULL;//for preventing invalid use of other pointer in memory
    }
}

//main function 
char *my_readline(int fd)
{
    char read_buf[READLINE_READ_SIZE + 1];//memory for 512 bytes + '\0'
    ssize_t bytes_read;
    int nl_index;
    char *line;

    if (fd < 0 || READLINE_READ_SIZE <= 0)
        return NULL;//error input (noting in input)

    while (1)
    {
        nl_index = find_newline(g_buffer);
        if (nl_index >= 0) // We found a newline in buffer
        {
            line = malloc(nl_index + 1);
            if (!line)
                return NULL;
            // Copy up to newline (not including)
            for (int i = 0; i < nl_index; i++)
                line[i] = g_buffer[i];
            line[nl_index] = '\0';

            // Remove line + newline from g_buffer
            char *tmp = g_buffer;
            g_buffer = malloc(str_len(g_buffer + nl_index + 1) + 1);
            if (!g_buffer)
            {
                free(line);
                return NULL;
            }
            int i = 0;
            while (tmp[nl_index + 1 + i])
            {
                g_buffer[i] = tmp[nl_index + 1 + i];
                i++;
            }
            g_buffer[i] = '\0';
            free(tmp);

            return line;
        }

        // No newline found, read more from fd
        bytes_read = read(fd, read_buf, READLINE_READ_SIZE);
        if (bytes_read <= 0) // EOF or error
        {
            if (bytes_read == 0 && g_buffer && g_buffer[0] != '\0')
            {
                // Return remaining data as last line (no newline)
                line = g_buffer;
                g_buffer = NULL;
                return line;
            }
            // Nothing left or error
            if (g_buffer)
            {
                free(g_buffer);
                g_buffer = NULL;
            }
            return NULL;
        }
        read_buf[bytes_read] = '\0';
        g_buffer = str_join_free(g_buffer, read_buf);
        if (!g_buffer)
            return NULL;
    }
}//gcc -Wall -Wextra -Werror -c my_readline.c


// int main(void)
// {
//     char *line;
//     int fd = open("file.txt", O_RDONLY);
//     if (fd < 0)
//         return 1;

//     init_my_readline();
//     while ((line = my_readline(fd)) != NULL)
//     {
//         printf("%s\n", line);
//         free(line);
//     }
//     close(fd);
//     return 0;
// }//gcc -Wall -Wextra -Werror -o test_reader my_readline.c main.c
// //./test_reader