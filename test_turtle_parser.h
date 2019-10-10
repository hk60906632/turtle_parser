#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_Y_WORD 100
#define MAX_X_WORD 100
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)


struct word{
  char words[MAX_X_WORD];
};
typedef struct word word;

struct prog{
  char wds[MAX_Y_WORD][MAX_X_WORD];
  int cw;
  int polish_test_1;
  int polish_test_2;
  int polish_test_3;
  int varnum_test;
  int check_decimal_test;
  int var_test;
  int set_test;
  int do_test_1;
  int do_test_2;
  int do_test_3;
};
typedef struct prog program;

void MAIN_F (program *p);
void check_again (program *p);
int check_bracket (program *p, int total_word);
int INSTRCTLST (program *p);
int INSTRUCTION (program *p);
int VAR(program *p);
int check_decimal(program *p);
void VARNUM(program *p);
void POLISH(program *p);
void FD(program *p);
void LT(program *p);
void RT(program *p);
void SET(program *p);
void DO(program *p);
