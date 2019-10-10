#include "test_turtle_parser.h"

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
    ON_ERROR("cannot oopen file");
  }
  prog.cw = 0;
  while(fscanf(fp, "%s",prog.wds[b++])==1 && b <MAX_Y_WORD){
    total_word++;
  }
  assert(b<MAX_Y_WORD);
  fclose(fp);
  strcpy(prog.wds[total_word], end);

  prog.varnum_test=0;
  assert(check_bracket (&prog, total_word) == 3);
  assert(strcmp(prog.wds[prog.cw], "{")==0);
  prog.cw++;
  assert(strcmp(prog.wds[prog.cw], "}")==0);
  prog.cw++;
  assert(strcmp(prog.wds[prog.cw], "}")==0);
  prog.cw++;
  printf("%s\n", prog.wds[prog.cw]);
  printf("\n");

  assert(INSTRUCTION (&prog)==1);
  assert(prog.varnum_test == 3);
  prog.varnum_test=0;
  printf("\n");

  printf("%s\n", prog.wds[prog.cw]);
  printf("%s\n", prog.wds[prog.cw+1]);
  assert(INSTRUCTION (&prog)==1);
  assert(prog.varnum_test==1);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==1);
  assert(prog.varnum_test==0);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==1);
  assert(prog.varnum_test==2);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==2);
  assert(prog.varnum_test==1);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==2);
  assert(prog.varnum_test==0);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==3);
  assert(prog.varnum_test==1);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==3);
  assert(prog.varnum_test==0);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==3);
  assert(prog.varnum_test==3);
  prog.varnum_test=0;
  printf("\n");

  assert(INSTRUCTION (&prog)==3);
  assert(prog.varnum_test==2);
  prog.varnum_test=0;
  printf("\n");

  assert(check_decimal(&prog)==2);
  prog.cw++;
  assert(check_decimal(&prog)==3);
  prog.cw++;
  assert(check_decimal(&prog)==2);
  prog.cw++;
  assert(check_decimal(&prog)==1);
  prog.cw++;

  assert(INSTRUCTION (&prog)==5);
  assert(prog.polish_test_1==1);
  assert(prog.polish_test_2==2);
  assert(prog.polish_test_3==3);
  assert(prog.var_test==2);
  printf("\n");

  assert(INSTRUCTION (&prog)==5);
  assert(prog.set_test==2);
  printf("\n");

  assert(INSTRUCTION (&prog)==5);
  assert(prog.var_test==1);
  printf("\n");

  prog.varnum_test=0;
  assert(INSTRUCTION (&prog)==4);
  assert(prog.do_test_1==1);
  assert(prog.do_test_2==1);
  assert(prog.do_test_3==1);
  assert(prog.var_test==2);
  assert(prog.varnum_test==0);
  prog.do_test_1==0;
  prog.do_test_2==0;
  prog.do_test_3==0;
  prog.cw++;
  printf("\n");

  prog.varnum_test=0;
  assert(INSTRUCTION (&prog)==4);
  assert(prog.do_test_1==2);
  prog.do_test_1==0;
  prog.do_test_2==0;
  prog.do_test_3==0;
  printf("\n");

  assert(INSTRUCTION (&prog)==4);
  assert(prog.do_test_2==2);
  prog.do_test_1==0;
  prog.do_test_2==0;
  prog.do_test_3==0;
  prog.cw++;
  printf("\n");

  assert(INSTRUCTION (&prog)==4);
  assert(prog.do_test_1==1);
  assert(prog.do_test_2==1);
  assert(prog.do_test_3==1);
  assert(prog.var_test==2);
  assert(prog.varnum_test==1);
  printf("\n");

  prog.cw++;
  assert(INSTRUCTION (&prog)==6);
  prog.cw++;
  assert(strcmp(prog.wds[prog.cw],"END_OF_CODE")==0);
}

int check_bracket (program *p, int total_word){
  int open_bracket = 0, close_bracket = 0, i;
  if (strcmp(p->wds[0],"{")!= 0){
    return (1);
  }
  for (i=0; i < total_word; i++){
    if(strcmp(p->wds[i],"{") == 0 ){
      open_bracket++;
      //printf("open bracket\n");
    }
    if(strcmp(p->wds[i],"}")==0){
      close_bracket++;
      //printf("close bracket\n");
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
}

void check_again (program *p){
  if (strcmp(p->wds[p->cw+1], "END_OF_CODE") == 0){
    return;
  }
  NEXT_WD;
  INSTRCTLST (p);
  check_again (p);
}

int INSTRCTLST (program *p){
  printf("Called Instrctlst \n");
  if(strcmp(CURRENT_WD,"}")== 0){
    return (2);
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


int VAR(program *p){
  printf("called VAR\n");
  if ((int)strlen(CURRENT_WD)>1){
    printf("\n##### Missing VAR or Invalid VAR naming [ %s ] !!_1 upper caser letter only_!! #####\n\n", p->wds[p->cw]);
    p->var_test=1;
  }
  if ((int)strlen(CURRENT_WD)==1 && (p->wds[p->cw][0]<'0' || p->wds[p->cw][0]>'9')){
    if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
      p->var_test=2;
    }
    else{
      printf("\n##### Invalid naming VAR [ %s ] !!_1 upper caser letter only_!! #####\n\n", p->wds[p->cw]);
      p->var_test=3;
    }
  }
}

int check_decimal(program *p){
  int j=0, k;
  for (k=0; k< (int)strlen(CURRENT_WD); k++){
    if (p->wds[p->cw][k]=='.'){
      j++;
    }
    if (j>1){
      printf("\n##### Too much . in your NUM [ %s ], ERROR #####\n\n", p->wds[p->cw]);
      return(1);
    }
  }
  if (p->wds[p->cw][0]=='.' || p->wds[p->cw][(int)strlen(CURRENT_WD)-1]=='.'){
    printf("\n##### '.' in wrong position [ %s ], ERROR #####\n\n", p->wds[p->cw]);
    return(2);
  }
  return(3);
}

void VARNUM(program *p){
  int i, j=0, k;
  printf ("called VARNUM\n");
  if ((int)strlen(CURRENT_WD)==1 && (p->wds[p->cw][0]<'0' || p->wds[p->cw][0]>'9')){
    if (p->wds[p->cw][0]>='A' && p->wds[p->cw][0]<='Z'){
      p->varnum_test=1;
    }
    else{
      printf("\n##### Invalid VAR [ %s ], 1 upper caser letter only #####\n\n", p->wds[p->cw]);
      p->varnum_test=2;
    }
  }
  else{
    //check_decimal(p);
    for (i=0; i< (int)strlen(CURRENT_WD); i++){
      if(p->wds[p->cw][i]!='.' && (p->wds[p->cw][i]<'0' || p->wds[p->cw][i]>'9')){
        printf("\n##### Invalid NUM or VAR, [ %s ] cause error #####\n\n", p->wds[p->cw]);
        p->varnum_test = 3;
        return;
      }
    }
    return;
  }
}

void POLISH(program *p){
  /*printf("%d\n",p->cw);*/
  if(strcmp(CURRENT_WD,";")== 0){
    printf("found ;, end of polish \n");
    p->polish_test_1 = 1;
    return;
  }
  else if(strcmp(CURRENT_WD,"+")== 0 || strcmp(CURRENT_WD,"-")== 0 || strcmp(CURRENT_WD,"*")== 0 || strcmp(CURRENT_WD,"/")== 0){
    NEXT_WD;
    printf("checked sign + - * / \n");
    p->polish_test_2 = 2;
    POLISH(p);
  }
  else{
    VARNUM(p);
    NEXT_WD;
    printf("called varnum\n");
    p->polish_test_3 = 3;
    POLISH(p);
  }
}

void FD(program *p){
  printf("called FD\n");
  if(strcmp(CURRENT_WD,"FD")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void LT(program *p){
  printf("called LT\n");
  if(strcmp(CURRENT_WD,"LT")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void RT(program *p){
  printf("called RT\n");
  if(strcmp(CURRENT_WD,"RT")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void SET(program *p){
  printf("called SET\n");
  if(strcmp(CURRENT_WD,"SET")== 0){
    NEXT_WD;
    VAR(p);
    NEXT_WD;
    if(strcmp(CURRENT_WD,":=")== 0){
      NEXT_WD;
      POLISH(p);
      NEXT_WD;
    }
    else{
      p->set_test=2;
      printf("Missing := for SET fuction\n");
      return;
    }
  }
}

void DO(program *p){
  printf("called DO \n");
  if(strcmp(CURRENT_WD,"DO")== 0){
    NEXT_WD;
    VAR(p);
    NEXT_WD;
    if(strcmp(CURRENT_WD,"FROM")== 0){
      NEXT_WD;
      p->do_test_1 =1;
      VARNUM(p);
      NEXT_WD;
      if(strcmp(CURRENT_WD,"TO")== 0){
        NEXT_WD;
        p->do_test_2 =1;
        VARNUM(p);
        NEXT_WD;
        if(strcmp(CURRENT_WD,"{")== 0){
          NEXT_WD;
          p->do_test_3 =1;
          INSTRCTLST(p);
        }
      }
      else{
        p->do_test_2 =2;
        printf("Missing 'TO' for DO\n");
      }
    }
    else{
      p->do_test_1 =2;
      printf ("Missing 'FROM' for DO\n");
    }
  }
}
