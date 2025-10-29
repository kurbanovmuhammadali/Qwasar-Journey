#ifndef CODEGEN_H
#define CODEGEN_H

#define SECRET_LENGTH 4
#define MAX_ROUNDS 10
#define ALLOWED_CHARS "012345678"

void create_secret(char *secret);
int check_input(const char *guess);
void assess_guess(const char *secret, const char *attempt, int *correct_pos, int *wrong_pos);

#endif // CODEGEN_H
