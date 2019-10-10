/*Every words in the code need to be separate by a space, like { } and all other symbol
  Remember this while coding */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "neillsdl2.h"

#define MAX_Y_WORD 200
#define MAX_X_WORD 200
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct node_t {
  double value;
  struct node_t *next;
};
typedef struct node_t node_t;

struct stack_t {
  node_t *head;
  int size;
};
typedef struct stack_t stack_t;

struct variable{
  double value;
  bool occupancy;
};
typedef struct variable variable;

struct prog{
  char wds[MAX_Y_WORD][MAX_X_WORD];
  variable var[26];
  stack_t stack;
  int cw;
  double x;
  double y;
  double x2;
  double y2;
  double angle;
  int pol_var_count;
  int pol_sym_count;
};
typedef struct prog program;

void MAIN_F (program *p, SDL_Simplewin sw);
void check_bracket (program *p, int total_word);
void initialize (program *p);
void check_again (program *p, SDL_Simplewin sw);
void INSTRCTLST (program *p, SDL_Simplewin sw);
void INSTRUCTION (program *p, SDL_Simplewin sw);
void VAR(program *p);
void check_decimal(program *p);
void VARNUM(program *p);
void POLISH(program *p, stack_t *stack);
void FD(program *p, SDL_Simplewin sw);
void LT(program *p);
void RT(program *p);
void SET(program *p);
void DO(program *p, SDL_Simplewin sw);
void draw_line (program *p, double value, SDL_Simplewin sw);
double extract_number (program *p);
bool is_number (program *p);
void jump_block (program *p);
void delay(unsigned int mseconds);
/*#### Stacks for SET reverse polish calculation, stacks functions ###########*/
node_t *make_new_node(double value);
stack_t *stack_initial();
bool isempty(stack_t *stack);
void push(stack_t *stack, double value);
double pop(stack_t *stack);
double calculation(char c, double a, double b);
double parse_postfix(stack_t stack);
