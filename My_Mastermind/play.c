#include "codegen.h"
#include "helpers.h"
#include <unistd.h>
#include <stdio.h>

void get_user_input(char *buffer) {
    char ch;
    int pos = 0;
    int read_bytes;

    while ((read_bytes = read(0, &ch, 1)) > 0) {
        if (ch == '\n' || pos >= SECRET_LENGTH) break;
        buffer[pos++] = ch;
    }
    buffer[pos] = '\0';

    if (read_bytes <= 0 && pos == 0) buffer[0] = '\0';
}

int main(int argc, char **argv) {
    char secret_code[SECRET_LENGTH + 1] = {0};
    int remaining_attempts = MAX_ROUNDS;

    create_secret(secret_code);

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (i + 1 < argc && str_compare(argv[i], "-c") == 0) {
                if (check_input(argv[i + 1])) {
                    for (int k = 0; k < SECRET_LENGTH; k++) {
                        secret_code[k] = argv[i + 1][k];
                    }
                    i++;
                }
            } else if (i + 1 < argc && str_compare(argv[i], "-t") == 0) {
                int tmp = 0;
                for (int k = 0; argv[i + 1][k]; k++) {
                    if (argv[i + 1][k] >= '0' && argv[i + 1][k] <= '9')
                        tmp = tmp * 10 + (argv[i + 1][k] - '0');
                }
                if (tmp > 0) remaining_attempts = tmp;
                i++;
            }
        }
    }

    printf("Will you find the secret code?\nPlease enter a valid guess\n");

    char user_guess[SECRET_LENGTH + 1];
    int round = 0;

    while (round < remaining_attempts) {
        printf("---\nRound %d\n>", round);
        fflush(stdout);

        get_user_input(user_guess);

        if (user_guess[0] == '\0') break;

        if (!check_input(user_guess)) {
            printf("Wrong input!\n");
            continue;
        }

        int correct, misplaced;
        assess_guess(secret_code, user_guess, &correct, &misplaced);

        if (correct == SECRET_LENGTH) {
            printf("Congratz! You did it!\n");
            return 0;
        }
        printf("Well placed pieces: %d\nMisplaced pieces: %d\n", correct, misplaced);
        round++;
    }

    return 0;
}
