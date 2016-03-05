#include "walksat.h"

void nextToken(char buffer[BUFFER_SIZE]){
  int c = getchar();
  int i = 0;
  while(c == ' ' || c == '\n'){ c = getchar(); }
  while(c != ' ' && c != '\n'){ buffer[i] = c; i++; c = getchar(); } 
}

void clearBuffer(char buffer[BUFFER_SIZE]){
  for (int i = 0; i< BUFFER_SIZE; i++){
    buffer[i] = 0;
  }
}

void fillVariables(int* variables, int vars){
	for (int i = 0; i<vars; i++){
		variables[i] = rand()% 2;
	}
}

int getRandomUnsatisfiedClauseIndex(int* unsatisfiedClauses, int num){
  if (!num){
    return -1;
  }
  int index = rand() % num;
  int clause = 0;
  int i = 0;
  while (1){
    if (unsatisfiedClauses[i]){
      clause++;
      if (clause > index){
        return i;
      }
    }
    i++;
  }

}

int made(int variableIndex,int clauses, int clausesContainingLiteral[][clauses],int satisfiedLiterals[clauses]){
  int made = 0;
  for (int i = 0; i< clauses; i++){
    if (clausesContainingLiteral[variableIndex+1][i] && satisfiedLiterals[i] == 0){
      made++;
    }
  }
  return made;
}

int broken(int variableIndex, int clauses, int clausesContainingLiteral[][clauses], int satisfiedLiterals[clauses]){
  int broken = 0;
  for (int i = 0; i< clauses; i++){
    if (clausesContainingLiteral[variableIndex][i] && satisfiedLiterals[i] == 1){
      broken++;
    }
  }
  return broken;
}


int pickVar(int clause, int vars, int clauses, int clausesContainingLiteral[vars*2][clauses], int satisfiedLiterals[clauses]){
  int best = -1;
  for (int i = 0; i<vars*2; i+=2){
    if (clausesContainingLiteral[i][clause]){
        best = i;
    }
  }
  int bestProfit = made(best, clauses, clausesContainingLiteral, satisfiedLiterals) - broken(best, clauses, clausesContainingLiteral, satisfiedLiterals);
  for (int i = best+1; i<vars*2; i+=2){
    if (clausesContainingLiteral[i][clause]){
      int profit = made(i, clauses, clausesContainingLiteral, satisfiedLiterals) - broken(i, clauses, clausesContainingLiteral, satisfiedLiterals);
      if (profit > bestProfit){
        bestProfit = profit;
        best = i;
      }
    }

  }
  return best/2;
}

int pickRandomVar(int clause, int vars, int clauses, int clausesContainingLiteral[vars*2][clauses]){
  int choices[clauses];
  int num_of_choices = 0; 
  for (int i = 0; i<vars*2; i+=2){
    if (clausesContainingLiteral[i][clause] || clausesContainingLiteral[i+1][clause]){
        choices[num_of_choices] = i;
        num_of_choices++;
    }
  }
  int r = rand() % num_of_choices;
  return (choices[r])/2;
}

int main(){
	srand(time(NULL));
  srand48(time(NULL));

  char* line = malloc(sizeof(char)*BUFFER_SIZE);
  size_t len = 0;
  while(1){
    getline(&line, &len, stdin);
    if (line[0] != 'c'){ break; }
  }
  int vars;
  int clauses;
  sscanf(line, "p cnf %d  %d", &vars, &clauses);
  int clausesContainingLiteral[vars*2][clauses];
  int satisfiedLiterals[clauses];
  
  for (int v = 0; v<vars*2; v++) {
  	for (int c = 0; c < clauses; c++) {
  		clausesContainingLiteral[v][c] = 0;
  	}
  }
  for (int c = 0; c < clauses; c++) {
  	satisfiedLiterals[c] = 0;
  }

  int i = 0;
  int token = 0;
  while (i < clauses){
    clearBuffer(line);
    nextToken(line);
    if (line[0] == '0'){
    	//end of line
      i++;
    } else if (line[0] == '-'){
    	//negated atoms
      line++;
      sscanf(line, "%d", &token);
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
//define initial variable values
  int variables[vars];

  fillVariables(variables, vars);
 

//satisfied vars per clause
//TODO untested
  for (i = 0; i < (vars*2); i++){
  	for (int c = 0; c < clauses; c++){
  		if (clausesContainingLiteral[i][c]){
  			if ((i & 1) == 0){
  				//positive variables
  				if (variables[i/2]){
  					satisfiedLiterals[c]++;
  				}
  			} else {
  				//negated variables
  				if (variables[(i-1)/2] == 0){
  					satisfiedLiterals[c]++;
  				}
  			}
  		}
  	}
  }

  int unsatisfiedClauses[clauses];
  int num_unsatisfiedClauses = 0;
  for (i = 0; i < clauses; i++){
  	if (satisfiedLiterals[i] == 0){
  		unsatisfiedClauses[i] = 1;
  		num_unsatisfiedClauses++;
  	} else {
  		unsatisfiedClauses[i] = 0;
  	}
  }

  printf("\033c");

   for ( i = 0; i < vars; ++i)
  {
    printf("%d", variables[i]);
  }
  printf("\n");

  for ( i = 0; i <clauses; i++){
    printf("%s ",unsatisfiedClauses[i] ? "Unsatisfied" : "satisfied");
  }
  printf("\n");

  float r;

  int timeout = 0;
  while(num_unsatisfiedClauses && timeout < THRESHOLD){
    //chose random unsatisfied clause
    int randomClause = getRandomUnsatisfiedClauseIndex(unsatisfiedClauses, num_unsatisfiedClauses);
    //printf("%d\n", randomclause);

    r = drand48();

    int varIndex;
    if (r > P){
      varIndex = pickVar(randomClause, vars, clauses, clausesContainingLiteral, satisfiedLiterals);
    } else {
      varIndex = pickRandomVar(randomClause , vars, clauses, clausesContainingLiteral);
    }

    //flip
    variables[varIndex] = !variables[varIndex];
    if (variables[varIndex]){
      for (i = 0; i< clauses; i++){
        if (clausesContainingLiteral[varIndex*2][i]){
          satisfiedLiterals[i]++;
          if (satisfiedLiterals[i] == 1){
            num_unsatisfiedClauses--;
            unsatisfiedClauses[i] = 0;
          }
        }
        if (clausesContainingLiteral[(varIndex*2)+1][i]){
          satisfiedLiterals[i]--;
          if (satisfiedLiterals[i] == 0){
            num_unsatisfiedClauses++;
            unsatisfiedClauses[i] = 1;
          }
        }
      }
    } else {
      for (i = 0; i< clauses; i++){
        if (clausesContainingLiteral[varIndex*2][i]){
          satisfiedLiterals[i]--;
          if (satisfiedLiterals[i] == 0){
            num_unsatisfiedClauses++;
            unsatisfiedClauses[i] = 1;
          }
        }
        if (clausesContainingLiteral[(varIndex*2)+1][i]){
          satisfiedLiterals[i]++;
          if (satisfiedLiterals[i] == 1){
            num_unsatisfiedClauses--;
            unsatisfiedClauses[i] = 0;
          }
        }
      }
    }

   printf("\r");
    for ( i = 0; i < vars; ++i) {
    printf("%d", variables[i]);
   }
   timeout++;
  }
  printf("\n");
  

  for ( i = 0; i <clauses; i++){
    printf("%s ",unsatisfiedClauses[i] ? "Unsatisfied" : "satisfied");
  }
  printf("\n%d\n", timeout);

}
