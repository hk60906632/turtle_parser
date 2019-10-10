/*Every words in the code need to be separate by a space, like { } and all other symbol
  Remember this while coding */


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
};
typedef struct prog program;

void MAIN_F (program *p);
void check_again (program *p);
void check_bracket (program *p, int total_word);
void INSTRCTLST (program *p);
void INSTRUCTION (program *p);
void VAR(program *p);
void check_decimal(program *p);
void VARNUM(program *p);
void POLISH(program *p);
void FD(program *p);
void LT(program *p);
void RT(program *p);
void SET(program *p);
void DO(program *p);
