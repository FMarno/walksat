#include "walksat.h"

void nextToken(char buffer[BUFFER_SIZE], FILE* fp){
  int c = fgetc(fp);
  int i = 0;
  while(c == ' ' || c == '\n'){ c = fgetc(fp); }
  while(c != ' ' && c != '\n'){ buffer[i] = c; i++; c = fgetc(fp); } 
}

void clearBuffer(char buffer[BUFFER_SIZE]){
  for (int i = 0; i< BUFFER_SIZE; i++){
    buffer[i] = 0;
  }
}


int** parseData(FILE* fp, int* vars, int* clauses){
 char* line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
 size_t len = 0;
  while(1){
    getline(&line, &len, fp);
    if (line[0] != 'c'){ break; }
  }

  sscanf(line, "p cnf %d  %d", vars, clauses);
  int** clausesContainingLiteral = (int**) calloc((*vars)*2, sizeof(int*));
  for (int i =0; i< (*vars)*2; i++){
  	clausesContainingLiteral[i] = calloc(*clauses, sizeof(int));
  }

  int i = 0;
  int token = 0;
  while (i < *clauses){
    clearBuffer(line);
    nextToken(line, fp);

    if (line[0] == '0'){
    	//end of line
      i++;
    } else if (line[0] == '-'){
    	//negated atoms
      line++;
      sscanf(line, "%d", &token);
      line--;
      token--;
      token = token*2 + 1;
      clausesContainingLiteral[token][i] = 1;
    } else {
    	//positive atoms
      sscanf(line, "%d", &token);
      token--;
      token *= 2;
      clausesContainingLiteral[token][i] = 1;
    }
  }
  free(line);

  return clausesContainingLiteral;

}