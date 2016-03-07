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


void parseHeader(FILE* fp, int* vars, int* clauses){
 char* line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
 size_t len = 0;
 do {
 	getline(&line, &len, fp);
 } while(line[0] == 'c');

  sscanf(line, "p cnf %d  %d", vars, clauses);
  free(line);
}

void parseData(FILE* fp, int vars, int clauses, int clausesContainingLiteral[vars*2][clauses]){
	char* line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	int i = 0;
    int token = 0;
	while (i < clauses){
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
}