#include "codegen.h"
#include "helpers.h"
#include <stdlib.h>
#include <time.h>

void create_secret(char *secret) {
    static int is_seeded = 0;
    if (!is_seeded) {
        srand(time(NULL));
        is_seeded = 1;
    }

    for (int idx = 0; idx < SECRET_LENGTH; idx++) {
        int is_unique = 0;
        while (!is_unique) {
            char candidate = '0' + (rand() % 9);
            is_unique = 1;
            for (int prev = 0; prev < idx; prev++) {
                if (secret[prev] == candidate) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) secret[idx] = candidate;
        }
    }
    secret[SECRET_LENGTH] = '\0';
}

int check_input(const char *guess) {
    if (str_len(guess) != SECRET_LENGTH) return 0;

    for (int i = 0; i < SECRET_LENGTH; i++) {
        if (!is_char_valid(guess[i])) return 0;
    }

    if (contains_duplicates(guess)) return 0;

    return 1;
}

void assess_guess(const char *secret, const char *attempt, int *correct_pos, int *wrong_pos) {
    *correct_pos = 0;
    *wrong_pos = 0;
    int secret_used[SECRET_LENGTH] = {0};
    int attempt_used[SECRET_LENGTH] = {0};

    for (int i = 0; i < SECRET_LENGTH; i++) {
        if (attempt[i] == secret[i]) {
            (*correct_pos)++;
            secret_used[i] = 1;
            attempt_used[i] = 1;
        }
    }

    for (int i = 0; i < SECRET_LENGTH; i++) {
        if (!attempt_used[i]) {
            for (int j = 0; j < SECRET_LENGTH; j++) {
                if (!secret_used[j] && attempt[i] == secret[j]) {
                    (*wrong_pos)++;
                    secret_used[j] = 1;
                    break;
                }
            }
        }
    }
}
