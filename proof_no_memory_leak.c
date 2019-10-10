#include "turtle_interpreter.h"
#define CURRENT_WD p->wds[p->cw]
#define NEXT_WD p->cw++;

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
  assert(b<MAX_Y_WORD);
  fclose(fp);
  strcpy(prog.wds[total_word], end);
  check_bracket(&prog, total_word);
  initialize (&prog);
  MAIN_F(&prog, sw);
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

void MAIN_F (program *p, SDL_Simplewin sw){
  NEXT_WD;
  INSTRCTLST (p, sw);
  check_again (p, sw);
  printf("################ Finished, safe to quit###############\n");
}

void initialize (program *p){
  p->cw = 0;
  p->x = 450;
  p->y = 450;
  p->angle = 0;
  p->pol_sym_count = 0;
  p->pol_var_count = 0;
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
  if (strcmp(CURRENT_WD,"RT")== 0){c = 'R';}
  if (strcmp(CURRENT_WD,"DO")== 0){c = 'D';}
  if (strcmp(CURRENT_WD,"SET")== 0){c = 'S';}
  switch(c){
    case 'F':
      FD(p, sw);
      break;
    case 'L':
      LT(p);
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
    default:
      printf("\n##### Invalid INSTRCTION [ %s ]  #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
  }
}

void draw_line (program *p, double value, SDL_Simplewin sw){
  p->x2 = p->x + value*cos(p->angle*(M_PI/180));
  p->y2 = p->y + value*sin(p->angle*(M_PI/180));
  delay(2500);
  p->x = p->x2;
  p->y = p->y2;
}

void FD(program *p, SDL_Simplewin sw){
  int var;
  double value;
  if(strcmp(CURRENT_WD,"FD")== 0){
    NEXT_WD;
    VARNUM(p);
    if (is_number (p) == true){
      sscanf(CURRENT_WD, "%lf", &value);
      draw_line (p, value, sw);
    }
    else if (is_number (p) == false){
      var = p->wds[p->cw][0]-'A';
      value = p->var[var].value;
      if (p->var[var].occupancy == true){
        draw_line (p, value, sw);
      }
      else {
        printf("\n##### ERROR : Variable [ %s ] is unassigned  #####\n\n", p->wds[p->cw]);
        exit(EXIT_FAILURE);
      }
    }
    NEXT_WD;
  }
}


void LT(program *p){
  int var;
  double temp;
  if(strcmp(CURRENT_WD,"LT")== 0){
    NEXT_WD;
    VARNUM(p);
    if (is_number (p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->angle += (360 - temp);
    }
    else {
      var = p->wds[p->cw][0]-'A';
      if (p->var[var].occupancy == true){
        p->angle += (360 - p->var[var].value);
      }
      else {
        printf("\n##### ERROR : Variable [ %s ] is unassigned  #####\n\n", p->wds[p->cw]);
        exit(EXIT_FAILURE);
      }
    }
    NEXT_WD;
  }
}


void RT(program *p){
  int var;
  double temp;
  if(strcmp(CURRENT_WD,"RT")== 0){
    NEXT_WD;
    VARNUM(p);
    if (is_number (p) == true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->angle += temp;
    }
    else if (is_number (p) == false){
      var = p->wds[p->cw][0]-'A';
      if (p->var[var].occupancy == true){
        p->angle += p->var[var].value;
      }
      else {
        printf("\n##### ERROR : Variable [ %s ] is unassigned  #####\n\n", p->wds[p->cw]);
        exit(EXIT_FAILURE);
      }
    }
    NEXT_WD;
  }
}

void POLISH(program *p, stack_t *stack){
  char c;
  double a, b, e;
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
    p->pol_sym_count++;
    c = CURRENT_WD[0];
    if (stack->size < 2){
      ON_ERROR("\n##### Expected 2 operands for SET before OP #####\n\n");
    }
    b = pop(stack);
    a = pop(stack);
    push(stack, calculation(c, a, b));
    NEXT_WD;
    POLISH(p, stack);
  }
  else{
    VARNUM(p);
    p->pol_var_count++;
    if (is_number (p)==false){
      push(stack, p->var[(p->wds[p->cw][0]-'A')].value);
    }
    else if (is_number (p)==true){
      sscanf(CURRENT_WD, "%lf", &e);
      push(stack, e);
    }
    NEXT_WD;
    POLISH(p, stack);
  }
}

void SET(program *p){
  int position;
  if(strcmp(CURRENT_WD,"SET")== 0){
    stack_t *stack = stack_initial();
    NEXT_WD;
    VAR(p);
    position = (p->wds[p->cw][0] - 'A');
    NEXT_WD;
    if(strcmp(CURRENT_WD,":=")== 0){
      NEXT_WD;
      POLISH(p, stack);
      while (isempty(stack)==false){
        p->var[position].value = pop(stack);
        p->var[position].occupancy = true;
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
  double begining, start_loop, end_loop;
  int var;
  if(strcmp(CURRENT_WD,"DO")== 0){
    NEXT_WD;
    VAR(p);
    var = p->wds[p->cw][0] - 'A';
    NEXT_WD;
    if(strcmp(CURRENT_WD,"FROM")== 0){
      NEXT_WD;
      VARNUM(p);
      start_loop = extract_number (p);
      NEXT_WD;
      if(strcmp(CURRENT_WD,"TO")== 0){
        NEXT_WD;
        VARNUM(p);
        end_loop = extract_number (p);
        NEXT_WD;
        if(strcmp(CURRENT_WD,"{")== 0){
          NEXT_WD;
          begining = p->cw;
          while(start_loop <= end_loop){
            p->var[var].value = start_loop;
            p->var[var].occupancy = true;
            INSTRCTLST(p, sw);
            p->cw = begining;
            start_loop ++;
          }
          jump_block (p);
        }
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


void VAR(program *p){
  if ((int)strlen(CURRENT_WD)>1){
    printf("\n##### Missing VAR or Invalid VAR naming [ %s ]!!_1 upper caser letter only_!! #####\n\n", p->wds[p->cw]);
    exit(EXIT_FAILURE);
  }
  if ((int)strlen(CURRENT_WD)==1 && (p->wds[p->cw][0]<'0' || p->wds[p->cw][0]>'9')){
    if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
      return;
    }
    else{
      printf("\n##### Invalid naming VAR [ %s ] !!_1 upper caser letter only_!! #####\n\n", p->wds[p->cw]);
      exit(EXIT_FAILURE);
    }
  }
}

void VARNUM(program *p){
  int i;
  if ((int)strlen(CURRENT_WD)==1 && (p->wds[p->cw][0]<'0' || p->wds[p->cw][0]>'9')){
    if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
      return;
    }
    else{
      printf("\n##### Invalid VAR [ %s ], 1 upper caser letter only #####\n\n", p->wds[p->cw]);
      exit(EXIT_FAILURE);
    }
  }
  else{
    check_decimal(p);
    for (i=0; i< (int)strlen(CURRENT_WD); i++){
      if(p->wds[p->cw][i]!='.' && (p->wds[p->cw][i]<'0' || p->wds[p->cw][i]>'9')){
        printf("\n##### Invalid NUM or VAR, [ %s ] cause error #####\n\n", p->wds[p->cw]);
        exit(EXIT_FAILURE);
      }
    }
    return;
  }
}

double extract_number (program *p){
  double value;
  int position;
  if (is_number (p) == true){
    sscanf(CURRENT_WD, "%lf", &value);
  }
  else if(is_number (p) == false){
    position = p->wds[p->cw][0]-'A';
    if (p->var[position].occupancy == true){
      value = p->var[position].value;
    }
    else {
      printf("\n##### ERROR : Variable [ %s ] is unassigned  #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
    }
  }
return value;
}


void check_decimal(program *p){
  int j=0, k;
  for (k=0; k< (int)strlen(CURRENT_WD); k++){
    if (p->wds[p->cw][k]=='.'){
      j++;
    }
    if (j>1){
      printf("\n##### Too much . in your NUM [ %s ], ERROR #####\n\n", p->wds[p->cw]);
      exit(EXIT_FAILURE);
    }
  }
  if (p->wds[p->cw][0]=='.' || p->wds[p->cw][(int)strlen(CURRENT_WD)-1]=='.'){
    printf("\n##### '.' in wrong position [ %s ], ERROR #####\n\n", p->wds[p->cw]);
    exit(EXIT_FAILURE);
  }
}

bool is_number (program *p){
  if (p->wds[p->cw][0]<'A' || p->wds[p->cw][0]>'Z'){
    return true;
  }
  else if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
    return false;
  }
  return false;
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

void delay(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/*################## Stacks for SET reverse polish calculation, stacks functions ################################*/
/*###############################################################################################################*/
node_t *make_new_node(double value){
  node_t *node = calloc(1, sizeof(node_t));
  node->value = value;
  node->next = NULL;
  return node;
}

stack_t *stack_initial() {
  stack_t *stack = calloc(1, sizeof(stack_t));
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
/*################## Stacks for SET reverse polish calculation, stacks functions ################################*/
