#include "turtle_parser.h"

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
    assert(strlen(prog.wds[total_word]) < MAX_X_WORD);
    total_word++;
  }
  assert(b<MAX_Y_WORD);
  fclose(fp);
  strcpy(prog.wds[total_word], end);
  check_bracket(&prog, total_word);
  MAIN_F(&prog);
  printf("NO error in code (uwu) \n");
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

void INSTRCTLST (program *p){
  if(strcmp(CURRENT_WD,"}")== 0){
    return;
  }
  else{
    INSTRUCTION (p);
    INSTRCTLST (p);
  }
}

void INSTRUCTION (program *p){
  char c;
  if (strcmp(CURRENT_WD,"FD")== 0){c = 'F';}
  if (strcmp(CURRENT_WD,"LT")== 0){c = 'L';}
  if (strcmp(CURRENT_WD,"RT")== 0){c = 'R';}
  if (strcmp(CURRENT_WD,"DO")== 0){c = 'D';}
  if (strcmp(CURRENT_WD,"SET")== 0){c = 'S';}
  switch(c){
    case 'F':
      FD(p);
      break;
    case 'L':
      LT(p);
      break;
    case 'R':
      RT(p);
      break;
    case 'D':
      DO(p);
      break;
    case 'S':
      SET(p);
      break;
    default:
      printf("\n##### Invalid INSTRCTION [ %s ]  #####\n\n", CURRENT_WD);
      exit(EXIT_FAILURE);
  }
}


void VAR(program *p){
  if ((int)strlen(CURRENT_WD)>1){
    printf("\n##### Missing VAR or Invalid VAR naming [ %s ] !!_1 upper caser letter only_!! #####\n\n", p->wds[p->cw]);
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

void POLISH(program *p){
  if(strcmp(CURRENT_WD,";")== 0){
    return;
  }
  else if(strcmp(CURRENT_WD,"+")== 0 || strcmp(CURRENT_WD,"-")== 0
  || strcmp(CURRENT_WD,"*")== 0 || strcmp(CURRENT_WD,"/")== 0){
    NEXT_WD;
    POLISH(p);
  }
  else{
    VARNUM(p);
    NEXT_WD;
    POLISH(p);
  }
}

void FD(program *p){
  if(strcmp(CURRENT_WD,"FD")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void LT(program *p){
  if(strcmp(CURRENT_WD,"LT")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void RT(program *p){
  if(strcmp(CURRENT_WD,"RT")== 0){
    NEXT_WD;
    VARNUM(p);
    NEXT_WD;
  }
}


void SET(program *p){
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
      ON_ERROR("Missing := for SET fuction\n");
    }
  }
}

void DO(program *p){
  if(strcmp(CURRENT_WD,"DO")== 0){
    NEXT_WD;
    VAR(p);
    NEXT_WD;
    if(strcmp(CURRENT_WD,"FROM")== 0){
      NEXT_WD;
      VARNUM(p);
      NEXT_WD;
      if(strcmp(CURRENT_WD,"TO")== 0){
        NEXT_WD;
        VARNUM(p);
        NEXT_WD;
        if(strcmp(CURRENT_WD,"{")== 0){
          NEXT_WD;
          INSTRCTLST(p);
        }
      }
      else{
        ON_ERROR("Missing 'TO' for DO\n");
      }
    }
    else{
      ON_ERROR("Missing 'FROM' for DO\n");
    }
  }
}
