#include "test_turtle_interpreter.h"
#define CURRENT_WD p->wds[p->cw]
#define NEXT_WD p->cw++;

int main(int argc, char* argv[]){
  int b=0, total_word=0;
  program prog;
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
  /*var and varnum function are exactly the same as turtle_parser and tested
  on test_turtle_parser so they wont be test here */
  initialize (&prog);
  assert(prog.x==450);
  assert(prog.y==450);
  assert(prog.angle==0);
  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  assert(INSTRUCTION (&prog)==3);
  assert(prog.angle==45);
  assert(INSTRUCTION (&prog)==3);
  assert(prog.angle==135);
  assert(INSTRUCTION (&prog)==2);
  assert(prog.angle==468);
  assert(INSTRUCTION (&prog)==2);
  assert(prog.angle==648);
  printf("\n");
  printf("test drawline here \n");
  draw_line(&prog, 100);
  assert(prog.x== (450+100*cos(648*(M_PI/180))));
  assert(prog.y== (450+100*sin(648*(M_PI/180))));
  printf("\n");
  printf("test stacks here \n");
  stack_t *stack = stack_initial();
  assert(stack->size == 0);
  push(stack, 30);
  assert(stack->head->value == 30);
  assert(stack->size == 1);
  push(stack, 50);
  assert(stack->size == 2);
  assert(stack->head->value == 50);
  assert(stack->head->next->value == 30);
  double x = pop(stack);
  double y = pop(stack);
  assert(stack->size == 0);
  assert(x == 50);
  assert(y == 30);
  double z = calculation('+', x, y);
  assert(z == 80);
  z = calculation('*', x, y);
  assert(z == 1500);
  z = calculation('-', x, y);
  assert(z == 20);
  free(stack);
  printf("\n");
  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  assert(INSTRUCTION (&prog)==5);
  assert(prog.var['B'-'A'].value==12);
  assert(prog.var['B'-'A'].occupancy==true);
  printf("\n");
  assert(INSTRUCTION (&prog)==5); /* which is 9*(3+3) */
  assert(prog.var['C'-'A'].value==54);
  assert(prog.var['C'-'A'].occupancy==true);
  printf("\n");
  assert(INSTRUCTION (&prog)==5); /* which is 12 + 54 */
  assert(prog.var['D'-'A'].value==66);
  assert(prog.var['D'-'A'].occupancy==true);
  printf("\n");
  stack_t *stack2 = stack_initial();
  assert(POLISH(&prog, stack2) == 1);
  pop(stack2);
  pop(stack2);
  prog.cw ++;
  printf("\n");
  assert(POLISH(&prog, stack2) == 3);
  pop(stack2);
  free(stack2);
  prog.angle=0;
  prog.cw += 3;
  printf("\n");
  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  INSTRCTLST (&prog);
  assert(prog.angle==150);
  assert(prog.var['H'-'A'].value==5);
  assert(prog.var['H'-'A'].occupancy==true);
  prog.angle=0;
  prog.cw += 2;
  printf("\n");
  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  INSTRCTLST (&prog);
  assert(prog.angle==15);
  assert(prog.var['Z'-'A'].value==5);
  assert(prog.var['Z'-'A'].occupancy==true);
  prog.angle=0;
  prog.cw += 2;
  printf("\n");
  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  INSTRCTLST (&prog);
  assert(prog.angle==0);
  assert(prog.var['E'-'A'].value==0);
}

int check_bracket (program *p, int total_word){
  int open_bracket = 0, close_bracket = 0, i;
  if (strcmp(p->wds[0],"{")!= 0){
    return (1);
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
    return (2);
  }
  else if (open_bracket < close_bracket){
    return (3);
  }
}

void MAIN_F (program *p){
  NEXT_WD;
  INSTRCTLST (p);
  check_again (p);
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

void check_again (program *p){
  if (strcmp(p->wds[p->cw+1], "END_OF_CODE") == 0){
    return;
  }
  NEXT_WD;
  INSTRCTLST (p);
  check_again (p);
}

void INSTRCTLST (program *p){
  printf("Called INSTRCTLST\n");
  if(strcmp(CURRENT_WD,"}")== 0){
    printf("end of INSTRCTLST\n");
    return;
  }
  else{
    INSTRUCTION (p);
    INSTRCTLST (p);
  }
}

int INSTRUCTION (program *p){
  printf("called Instruction\n");
  char c;
  if (strcmp(CURRENT_WD,"FD")== 0){c = 'F';}
  if (strcmp(CURRENT_WD,"LT")== 0){c = 'L';}
  if (strcmp(CURRENT_WD,"RT")== 0){c = 'R';}
  if (strcmp(CURRENT_WD,"DO")== 0){c = 'D';}
  if (strcmp(CURRENT_WD,"SET")== 0){c = 'S';}
  switch(c){
    case 'F':
      FD(p);
      return(1);
      break;
    case 'L':
      LT(p);
      return(2);
      break;
    case 'R':
      RT(p);
      return(3);
      break;
    case 'D':
      DO(p);
      return(4);
      break;
    case 'S':
      SET(p);
      return(5);
      break;
    default:
      printf("\n##### Invalid INSTRCTION [ %s ]  #####\n\n", CURRENT_WD);
      return (6);
  }
}

void draw_line (program *p, double value){
  p->x2 = p->x + value*cos(p->angle*(M_PI/180));
  p->y2 = p->y + value*sin(p->angle*(M_PI/180));
  /*delay(2500);
  SDL_RenderDrawLine(sw.renderer,p->x,p->y,p->x2,p->y2);
  Neill_SDL_UpdateScreen(&sw);*/
  p->x = p->x2;
  p->y = p->y2;
}

double FD(program *p){
  int var;
  double value;
  if(strcmp(CURRENT_WD,"FD")== 0){
    NEXT_WD;
    VARNUM(p);
    if (is_number (p) == true){
      sscanf(CURRENT_WD, "%lf", &value);
      /*draw_line (p, value, sw);*/
      return value;
    }
    else {
      var = p->wds[p->cw][0]-'A';
      value = p->var[var].value;
      return value;
      /*if (p->var[var].occupancy == true){
        draw_line (p, value, sw);
      }
      else {
        printf("\n##### ERROR : Variable [ %s ] is unassigned  #####\n\n", p->wds[p->cw]);
        exit(EXIT_FAILURE);
      }*/
    }
    NEXT_WD;
  }
}


int LT(program *p){
  int var;
  double temp;
  if(strcmp(CURRENT_WD,"LT")== 0){
    NEXT_WD;
    VARNUM(p);
    if (is_number (p)==true){
      sscanf(CURRENT_WD, "%lf", &temp);
      p->angle += (360 - temp);
    }
    else if (is_number (p)==false){
      var = p->wds[p->cw][0]-'A';
      if (p->var[var].occupancy == true){
        p->angle += (360 - p->var[var].value);
      }
      else {
        return (2);
      }
    }
    NEXT_WD;
  }
}


int RT(program *p){
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
        return (2);
      }
    }
    NEXT_WD;
  }
}

int POLISH(program *p, stack_t *stack){
  char c;
  double a, b, e;
  if(strcmp(CURRENT_WD,";")== 0){
    if (p->pol_sym_count == 0 && p->pol_var_count != 1 ){
      return(1);
    }
    p->pol_var_count = 0;
    p->pol_sym_count = 0;
    return(2);
  }
  else if(strcmp(CURRENT_WD,"+")== 0 || strcmp(CURRENT_WD,"-")== 0
  || strcmp(CURRENT_WD,"*")== 0 || strcmp(CURRENT_WD,"/")== 0){
    p->pol_sym_count++;
    c = CURRENT_WD[0];
    if (stack->size < 2){
      return(3);
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
    else {
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
      printf("stack size %d\n", stack->size);
      while (isempty(stack)==false){
        p->var[position].value = pop(stack);
        printf("value of calculation %lf\n", p->var[position].value);
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

int DO(program *p){
  char *ptr;
  double begining, start_loop, end_loop;
  int var, start, end;
  if(strcmp(CURRENT_WD,"DO")== 0){
    NEXT_WD;
    VAR(p);
    var = p->wds[p->cw][0] - 'A';
    p->var[var].value = 0;/*####for test only####*/
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
          printf("Now in the while loop:\n");
          printf("end_loop is : %lf\n", end_loop);
          while(start_loop <= end_loop){
            printf("start_loop is : %lf \n", start_loop);
            p->var[var].value = start_loop;
            p->var[var].occupancy = true;
            INSTRCTLST(p);
            p->cw = begining;
            start_loop ++;
          }
          jump_block (p);
        }
      }
    }
    else{
      return (1);
    }
  }
  else{
    return(2);
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

void jump_block (program *p){
  int open_bracket=1, close_bracket=0, jump_step=0, i;
  i = p->cw;
  while(open_bracket!= close_bracket){
    if(strcmp(p->wds[i],"{") == 0 ){
      open_bracket++;
      printf("open %d\n", open_bracket);
    }
    if(strcmp(p->wds[i],"}")==0){
      close_bracket++;
      printf("close %d\n", close_bracket);
    }
    i++;
    jump_step++;
  }
p->cw += jump_step;
}

bool is_number (program *p){
  int i, is_number=0;
  if (p->wds[p->cw][0]<'A' || p->wds[p->cw][0]>'Z'){
    return true;
  }
  else if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
    return false;
  }
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
  printf("push variable[%lf], stack size is:%d\n", value, stack->size);
}

double pop(stack_t *stack) {

  if (isempty(stack)==true){
    ON_ERROR("\n##### Stack underflow #####\n\n");
  }
  node_t *head = stack->head;
  double value = head->value;
  stack->head = head->next;
  free(head);
  stack->size--;
  printf("pop value [%lf] out, stack size: %d\n", value,stack->size );
  return value;
}

double calculation(char c, double a, double b){
  printf("called calculation\n");
  switch (c) {
    case '+':
      return (a + b);
    case '-':
      return (a - b);
    case '*':
      return (a * b);
    case '/':
      return (a / b);
  }
}
/*################## Stacks for SET reverse polish calculation, stacks functions ################################*/
