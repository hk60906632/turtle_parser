#include "extension_interpreter.h"
#define CURRENT_WD p->wds[p->cw]
#define NEXT_WD p->cw++;
#define DOUBLE_HASH (index1 + i*index2)%(50)
#define PRIME 13
#define DELAY_TIME 2500
#define START_X 450
#define START_Y 450

/*Everytime i called the SDL functions, it causes memory leak
  NO memory leak when none of the SDL function called*/

int main(int argc, char* argv[]){
  int b=0, total_word=0;
  program prog;
  SDL_Simplewin sw;
  char end[13] = "END_OF_CODE";
  FILE *fp;
  if (argc != 2){
    ON_ERROR("Missing file name \n");
  }
  if(!(fp = fopen(argv[1], "r"))){
    ON_ERROR("cannot oopen file\n");
  }
  while(fscanf(fp, "%s",prog.wds[b++])==1 && b <MAX_Y_WORD){
    assert(strlen(prog.wds[total_word]) < MAX_X_WORD);
    total_word++;
  }
  printf("%d\n", total_word);
  assert(b<MAX_Y_WORD);
  fclose(fp);
  strcpy(prog.wds[total_word], end);
  check_bracket(&prog, total_word);
  initialize (&prog);
  create_var_tab(&prog);
  Neill_SDL_Init(&sw);
  SDL_SetRenderDrawColor(sw.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sw.renderer);
  SDL_SetRenderDrawColor(sw.renderer, 255, 180, 0, SDL_ALPHA_OPAQUE);
  MAIN_F(&prog, sw);
  SDL_RenderPresent(sw.renderer);
  do{
     Neill_SDL_Events(&sw);
  }while(!sw.finished);
  table_free(&prog);
  SDL_Quit();
  atexit(SDL_Quit);
  return(0);
}

void check_bracket (program *p, int total_word){
  int open_bracket = 0, close_bracket = 0, i;
  if (strcmp(p->wds[0],"{")!= 0){
    ON_ERROR("\n##### Missing the very first '{' #####\n\n");
  }
  for (i=0; i < total_word; i++){
    if(strcmp(p->wds[i],"{") == 0 ){
      open_bracket++;
    }
    if(strcmp(p->wds[i],"}")==0){
      close_bracket++;
    }
  }
  if (open_bracket > close_bracket){
    ON_ERROR("\n##### Missing }, please check #####\n\n");
  }
  else if (open_bracket < close_bracket){
    ON_ERROR("\n##### Missing {, please check #####\n\n");
  }
}

void initialize (program *p){
  p->cw = 0;
  p->x = START_X;
  p->y = START_Y;
  p->angle = 0;
  p->pol_var_count=0;
  p->pol_sym_count = 0;
}

void MAIN_F (program *p, SDL_Simplewin sw){
  NEXT_WD;
  INSTRCTLST (p, sw);
  check_again (p, sw);
  printf("################ Finished, safe to quit###############\n");
}

void check_again (program *p, SDL_Simplewin sw){
  if (strcmp(p->wds[p->cw+1], "END_OF_CODE") == 0){
    return;
  }
  NEXT_WD;
  INSTRCTLST (p, sw);
  check_again (p, sw);
}

void INSTRCTLST (program *p, SDL_Simplewin sw){
  if(strcmp(CURRENT_WD,"}")== 0){
    return;
  }
  else{
    INSTRUCTION (p, sw);
    INSTRCTLST (p, sw);
  }
}

void INSTRUCTION (program *p, SDL_Simplewin sw){
  char c;
  if (strcmp(CURRENT_WD,"FD")== 0){c = 'F';}
  if (strcmp(CURRENT_WD,"LT")== 0){c = 'L';}
  if (strcmp(CURRENT_WD,"MOVE_X")== 0){c = 'X';}
  if (strcmp(CURRENT_WD,"RT")== 0){c = 'R';}
  if (strcmp(CURRENT_WD,"DO")== 0){c = 'D';}
  if (strcmp(CURRENT_WD,"SET")== 0){c = 'S';}
  if (strcmp(CURRENT_WD,"MOVE_Y")== 0){c = 'Y';}
  switch(c){
    case 'F':
      FD(p, sw);
      break;
    case 'L':
      LT(p);
      break;
    case 'X':
      MOVE_X(p);
      break;
    case 'R':
      RT(p);
      break;
    case 'D':
      DO(p, sw);
      break;
    case 'S':
      SET(p);
      break;
    case 'Y':
      MOVE_Y(p);
      break;
    default:
      printf("\n##### Invalid INSTRCTION [ %s ]  #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
  }
}

void VAR(program *p){
  int k;
  if (is_number (CURRENT_WD)==true || check_decimal(p)==true){
    printf("\n##### Require variable , NOT number [ %s ] !!_ERROR_!! #####\n\n", CURRENT_WD);
    exit(EXIT_FAILURE);
  }
  for (k=0; k< (int)strlen(CURRENT_WD); k++){
    if (CURRENT_WD[k]=='.' || CURRENT_WD[k]=='+' || CURRENT_WD[k]=='-'
        || CURRENT_WD[k]=='*' || CURRENT_WD[k]=='/'){
      printf("\n##### No + - * / . in variable naming [ %s ], ERROR #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
    }
  }
}

void FD(program *p, SDL_Simplewin sw){
  stack_t *stack;
  double value;
  if(strcmp(CURRENT_WD,"FD")== 0){
    NEXT_WD;
    if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &value);
      draw_line (p, value, sw);
    }
    if (is_number(CURRENT_WD) == false && check_decimal(p)==false
    && strcmp(CURRENT_WD,":")!= 0){
      value = get_value(p, CURRENT_WD);
      draw_line (p, value, sw);
    }
    if (strcmp(CURRENT_WD,":")== 0){
      NEXT_WD;
      stack = stack_initial();
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
      }
      draw_line (p, value, sw);
      free(stack);
    }
    NEXT_WD;
  }
}

void LT(program *p){
  double temp, value;
  stack_t *stack;
  if(strcmp(CURRENT_WD,"LT")== 0){
    NEXT_WD;
    if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->angle += (360 - temp);
    }
    if (is_number(CURRENT_WD) == false && check_decimal(p)==false
    && strcmp(CURRENT_WD,":")!= 0){
      value = get_value(p, CURRENT_WD);
      p->angle += (360 - value);
    }
    if (strcmp(CURRENT_WD,":")== 0){
      NEXT_WD;
      stack = stack_initial();
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
      }
      p->angle += (360 - value);
      free(stack);
    }
    NEXT_WD;
  }
}

void RT(program *p){
  double temp, value;
  stack_t *stack;
  if(strcmp(CURRENT_WD,"RT")== 0){
    NEXT_WD;
    if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->angle += temp;
    }
    if (is_number(CURRENT_WD) == false && check_decimal(p)==false
    && strcmp(CURRENT_WD,":")!= 0){
      value = get_value(p, CURRENT_WD);
      p->angle += value;
    }
    if (strcmp(CURRENT_WD,":")== 0){
      NEXT_WD;
      stack = stack_initial();
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
      }
      p->angle += value;
      free(stack);
    }
    NEXT_WD;
  }
}

void POLISH(program *p, stack_t *stack){
  char c;
  char variable[25];
  double a, b, e, value;
  if(strcmp(CURRENT_WD,";")== 0){
    if (p->pol_sym_count == 0 && p->pol_var_count != 1 ){
      ON_ERROR("\n##### Missing + - * / for set #####\n\n");
    }
    p->pol_var_count = 0;
    p->pol_sym_count = 0;
    return;
  }
  else if(strcmp(CURRENT_WD,"+")== 0 || strcmp(CURRENT_WD,"-")== 0
  || strcmp(CURRENT_WD,"*")== 0 || strcmp(CURRENT_WD,"/")== 0){
    c = p->wds[p->cw][0];
    p->pol_sym_count++;
    if (stack->size < 2){
      ON_ERROR("\n##### Expected 2 operands for SET #####\n\n");
    }
    b = pop(stack);
    a = pop(stack);
    push(stack, calculation(c, a, b));
    NEXT_WD;
    POLISH(p, stack);
  }
  else{
    p->pol_var_count++;
    strcpy(variable, CURRENT_WD);
    if (is_number (CURRENT_WD) == false && check_decimal(p)==false){
      value = get_value(p, variable);
      push(stack, value);
    }
    else if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &e);
      push(stack, e);
    }

    NEXT_WD;
    POLISH(p, stack);
  }
}

void SET(program *p){
  int  value;
  char variable[MAX_X_WORD];
  if(strcmp(CURRENT_WD,"SET")== 0){
    stack_t *stack = stack_initial();
    NEXT_WD;
    VAR(p);
    strcpy(variable, CURRENT_WD);
    if (var_isin(p, CURRENT_WD)==false){
      add_var(p, CURRENT_WD);
    }
    NEXT_WD;
    if(strcmp(CURRENT_WD,":=")== 0){
      NEXT_WD;
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
        var_value(p, variable, value);
      }
      free(stack);
      NEXT_WD;
    }
    else{
      ON_ERROR("\n##### Missing := for SET fuction #####\n\n");
    }
  }
}

void DO(program *p, SDL_Simplewin sw){
  char variable[MAX_X_WORD];
  double begining, start_loop, end_loop, every;
  if(strcmp(CURRENT_WD,"DO")== 0){
    NEXT_WD;
    VAR(p);
    strcpy(variable, CURRENT_WD);
    if (var_isin(p, CURRENT_WD)==false){
      add_var(p, CURRENT_WD);
    }
    NEXT_WD;
    if(strcmp(CURRENT_WD,"FROM")== 0){
      NEXT_WD;
      start_loop = extract_number (p);
      NEXT_WD;
      if(strcmp(CURRENT_WD,"TO")== 0){
        NEXT_WD;
        end_loop = extract_number (p);
        NEXT_WD;
        if (strcmp(CURRENT_WD,"EVERY")== 0){
          NEXT_WD;
          every = extract_number (p);
          if (every <= 0){
            ON_ERROR("\n##### EVERY can only be non zero positive number#####\n\n");
          }
          NEXT_WD;
        if(strcmp(CURRENT_WD,"{")== 0){
          NEXT_WD;
          begining = p->cw;
            while(start_loop <= end_loop){
              var_value(p, variable, start_loop);
              INSTRCTLST(p, sw);
              p->cw = begining;
              start_loop +=  every;
            }
            jump_block(p);
          }
          return;
        }
       }
       else{
         ON_ERROR("\n##### Missing 'EVERY' for DO #####\n\n");
       }
      }
      else{
        ON_ERROR("\n##### Missing 'TO' for DO #####\n\n");
      }
    }
    else{
      ON_ERROR("\n##### Missing 'FROM' for DO #####\n\n");
    }
  }

void MOVE_X (program *p){
  double temp, value;
  stack_t *stack;
  if(strcmp(CURRENT_WD,"MOVE_X")== 0){
    NEXT_WD;
    if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->x = temp;
    }
    if (is_number(CURRENT_WD) == false && check_decimal(p)==false
        && strcmp(CURRENT_WD, ":")!= 0){
      value = get_value(p, CURRENT_WD);
      p->x = value;
    }
    if (strcmp(CURRENT_WD,":")== 0){
      NEXT_WD;
      stack = stack_initial();
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
      }
      p->x = value;
      free(stack);
    }
  NEXT_WD;
  }
}

void MOVE_Y (program *p){
  double temp, value;
  stack_t *stack;
  if(strcmp(CURRENT_WD,"MOVE_Y")== 0){
    NEXT_WD;
    if (is_number(CURRENT_WD) == true || check_decimal(p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->y = temp;
    }
    if (is_number(CURRENT_WD) == false && check_decimal(p)==false
        && strcmp(CURRENT_WD, ":")!= 0){
      value = get_value(p, CURRENT_WD);
      p->y = value;
    }
    if (strcmp(CURRENT_WD,":")== 0){
      NEXT_WD;
      stack = stack_initial();
      POLISH(p, stack);
      while (isempty(stack)==false){
        value = pop(stack);
      }
      p->y = value;
      free(stack);
    }
  NEXT_WD;
  }
}

void draw_line (program *p, double value, SDL_Simplewin sw){
  p->x2 = p->x + value*cos(p->angle*(M_PI/180));
  p->y2 = p->y + value*sin(p->angle*(M_PI/180));
  delay(DELAY_TIME);
  SDL_RenderDrawLine(sw.renderer,p->x,p->y,p->x2,p->y2);
  Neill_SDL_UpdateScreen(&sw);
  p->x = p->x2;
  p->y = p->y2;
}


void jump_block (program *p){
  int open_bracket=1, close_bracket=0, jump_step=0, i;
  i = p->cw;
  while(open_bracket!= close_bracket){
    if(strcmp(p->wds[i],"{") == 0 ){
      open_bracket++;
    }
    if(strcmp(p->wds[i],"}")==0){
      close_bracket++;
    }
    i++;
    jump_step++;
  }
p->cw += jump_step;
}

double extract_number (program *p){
  double value;
  if (is_number (CURRENT_WD) == true || check_decimal(p)==true){
    sscanf(CURRENT_WD, "%lf", &value);
  }
  else if(is_number (CURRENT_WD) == false && check_decimal(p)==false){
    value = get_value(p, CURRENT_WD);
  }
return value;
}

bool check_decimal(program *p){
  int j=0, k, decimal=0;
  if (p->wds[p->cw][0]=='.' || p->wds[p->cw][(int)strlen(CURRENT_WD)-1]=='.'){
    printf("\n##### '.' in wrong position [ %s ], ERROR #####\n\n", CURRENT_WD);
    exit(EXIT_FAILURE);
  }
  for (k=0; k< (int)strlen(CURRENT_WD); k++){
    if (p->wds[p->cw][k]=='.'){
      j++;
    }
    if (isdigit(p->wds[p->cw][k])!= 0){
      decimal++;
    }
    if (j>1){
      printf("\n##### Too much . in your NUM [ %s ], ERROR #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
    }
  }
  if (j == 1 && decimal == (int)strlen(CURRENT_WD)-1){
    return true;
  }
  else{
    return false;
  }
}

bool is_number (char* v){
  int i, is_number=0;
  for (i=0; i<(int)strlen(v); i++){
    if (isdigit(v[i])!=0){
      is_number++;
    }
  }
  if(is_number == (int)strlen(v)){
    return true;
  }
  else{
    return false;
  }
}

void delay(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/*################## Stacks for SET reverse polish calculation, stacks functions ################################*/
/*###############################################################################################################*/
node_t *make_new_node(double value){
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->value = value;
  node->next = NULL;
  return node;
}

stack_t *stack_initial() {
  stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
  if (stack == NULL){
    ON_ERROR("\n##### Failed to allocate memory #####\n\n");
  }
  stack->head = NULL;
  stack->size = 0;
  return stack;
}

bool isempty(stack_t *stack){
  if (stack->size == 0){
    return true;
  }
  else {
    return false;
  }
}

void push(stack_t *stack, double value){
  node_t *new_node = make_new_node(value);
  if (isempty(stack)==true){
    stack->head = new_node;
  }
  else {
    node_t *temp_head = stack->head;
    stack->head = new_node;
    new_node->next = temp_head;
  }
  stack->size++;
}

double pop(stack_t *stack) {
  node_t *head;
  double value;
  if (isempty(stack)==true){
    ON_ERROR("\n##### Stack underflow #####\n\n");
  }
  head = stack->head;
  value = head->value;
  stack->head = head->next;
  free(head);
  stack->size--;
  return value;
}

double calculation(char c, double a, double b){
  switch (c) {
    case '+':
      return (a + b);
    case '-':
      return (a - b);
    case '*':
      return (a * b);
    case '/':
      return (a / b);
    default:
      ON_ERROR("\n##### missing + - * / #####\n\n");
  }
}
/*###############################################################################################################*/
/*################## Stacks for SET reverse polish calculation, stacks functions ################################*/


/*########################### Variable Hash table functions ############################*/
/*#######################################################################################*/
void create_var_tab(program *p){
  p->var_table = (variable*) calloc(VAR_TABLE_SIZE, sizeof(variable));
  if(p->var_table == NULL){
    ON_ERROR("Creation of hash table Failed\n");
  }
}

int hash_key_1(char *word){
  unsigned long hash = 5381;
  int c;
  while((c=(*word++))){
    hash = 33 * hash ^ c;
  }
  return (int)(hash % 50);
}

int hash_key_2(char *word){
  int key2 = 0, word_size, i;
  word_size = strlen(word);
  for (i = 0; i < word_size; i++){
    key2 += word[i];
  }
  key2 = (PRIME - (key2 % PRIME));
  return key2;
}

void add_var(program *p, char* var_nam){
  int i = 0, index;
  int index1 = hash_key_1(var_nam);
  int index2 = hash_key_2(var_nam);
  if (p->filled >= VAR_TABLE_SIZE){
    ON_ERROR("ERROR!!!!!! Variable table overflow \n");
  }
  if (p->var_table[index1].name != NULL){
    i++;
    while (p->var_table[DOUBLE_HASH].name != NULL){
        i++;
      }
      index = DOUBLE_HASH;
      p->var_table[index].name = (char*) calloc(strlen(var_nam)+1, sizeof(char));
        if(p->var_table[index].name == NULL){
          ON_ERROR("Creation of Word Space 1 Failed\n");
        }
      strcpy(p->var_table[index].name, var_nam);
      p->filled++;
    }
  else{
    p->var_table[index1].name = (char*) calloc(strlen(var_nam)+1, sizeof(char));
      if(p->var_table[index1].name == NULL){
        ON_ERROR("Creation of Word Space 2 Failed\n");
      }
    strcpy(p->var_table[index1].name, var_nam);
    p->filled++;
  }
}

bool var_isin(program *p, char* var_nam){
  int i = 0;
  int index1 = hash_key_1(var_nam);
  int index2 = hash_key_2(var_nam);
  while (p->var_table[DOUBLE_HASH].name != NULL){
    if (strcmp(p->var_table[DOUBLE_HASH].name, var_nam) == 0){
      return true;
    }
    else{
      i++;
    }
  }
return false;
}

double get_value(program *p, char* var_nam){
  int i = 0;
  int index1 = hash_key_1(var_nam);
  int index2 = hash_key_2(var_nam);
  while (p->var_table[DOUBLE_HASH].name != NULL){
    if (strcmp(p->var_table[DOUBLE_HASH].name, var_nam) == 0){
      return(p->var_table[DOUBLE_HASH].value);
    }
    else{
      i++;
    }
  }
  printf("\n##### Unassigned variable [ %s ] !!_ERROR_!! #####\n\n", CURRENT_WD);
  exit(EXIT_FAILURE);
}

void var_value(program *p, char* var_nam, double value){
  int i = 0;
  int index1 = hash_key_1(var_nam);
  int index2 = hash_key_2(var_nam);
  while (p->var_table[DOUBLE_HASH].name != NULL){
    if (strcmp(p->var_table[DOUBLE_HASH].name, var_nam) == 0){
      p->var_table[DOUBLE_HASH].value = value;
      return;
    }
    else{
      i++;
    }
  }
}

void table_free(program* p){
  int i;
  for (i = 0; i < VAR_TABLE_SIZE; i++){
    if (p->var_table[i].name != NULL){
      free(p->var_table[i].name);
    }
  }
  free (p->var_table);
}

/*#######################################################################################*/
/*########################### Variable Hash table functions ############################*/
