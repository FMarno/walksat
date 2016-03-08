#include "walksat.h"

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

//TODO fucked
int made(int variableIndex,int clauses,int vars, int clausesContainingLiteral[vars*2][clauses],int satisfiedLiterals[clauses]){
  int made = 0;
  for (int i = 0; i< clauses; i++){
    if (clausesContainingLiteral[(variableIndex)+1][i] && satisfiedLiterals[i] == 0){
      made++;
    }
  }
  return made;
}

//TODO fucked
int broken(int variableIndex, int clauses, int vars, int clausesContainingLiteral[vars*2][clauses], int satisfiedLiterals[clauses]){
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
        break;
    } else if (clausesContainingLiteral[i+1][clause]){
    	best = i+1;
    	break;
    }
  }
  int bestProfit = made(best, clauses, vars, clausesContainingLiteral, satisfiedLiterals) - broken(best, clauses, vars, clausesContainingLiteral, satisfiedLiterals);
  for (int i = best+1; i<vars*2; i+=2){
    if (clausesContainingLiteral[i][clause]){
    	int m =  made(i, clauses, vars, clausesContainingLiteral, satisfiedLiterals);
    	int b = broken(i, clauses, vars, clausesContainingLiteral, satisfiedLiterals);
    	//printf("%d\tm: %d\tb: %d\n",i,m,b );
      int profit = m-b; 
      if (profit > bestProfit){
        bestProfit = profit;
        best = i;
      }
    }

  }
  //printf("%d\n",best/2);
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

  int vars; int clauses; FILE* fp;
  fp = fopen("cnffiles/uf20-01.cnf", "r");
  if (fp == NULL){
    printf("file not found\n");
    exit(1);
  }


  parseHeader(fp, &vars, &clauses);

  int clausesContainingLiteral[vars*2][clauses];
  for (int i = 0; i < vars*2; ++i)
  {
  	for (int  a = 0; a < clauses; ++a)
  	{
  		clausesContainingLiteral[i][a] = 0;
  	}
  }
  parseData(fp, vars, clauses, clausesContainingLiteral);
  int* satisfiedLiterals = (int*)calloc(clauses, sizeof(int));
//define initial variable values
  int variables[vars];

  fillVariables(variables, vars);
 

//satisfied vars per clause
//TODO untested
  for (int i = 0; i < (vars*2); i++){
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
  for (int i = 0; i < clauses; i++){
  	if (satisfiedLiterals[i] == 0){
  		unsatisfiedClauses[i] = 1;
  		num_unsatisfiedClauses++;
  	} else {
  		unsatisfiedClauses[i] = 0;
  	}
  }

   printf("\033c");
   for ( int i = 0; i < vars; ++i)
  {
    printf("%d", variables[i]);
  }
  printf("\n");

  for ( int i = 0; i <clauses; i++){
    printf("%s ",unsatisfiedClauses[i] ? "Unsatisfied" : "satisfied");
  }
  printf("\n");

  float r;

  int timeout = 0;
  while(num_unsatisfiedClauses && timeout < THRESHOLD){
    //chose random unsatisfied clause
    int randomClause = getRandomUnsatisfiedClauseIndex(unsatisfiedClauses, num_unsatisfiedClauses);

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
      for (int i = 0; i< clauses; i++){
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
      for (int i = 0; i< clauses; i++){
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

   //printf("\r");
    for (int i = 0; i < vars; ++i) {
    //printf("%d", variables[i]);
   }
   timeout++;
  }
  printf("\n");
  

  for ( int i = 0; i <clauses; i++){
    printf("%s ",unsatisfiedClauses[i] ? "Unsatisfied" : "satisfied");
  }
  printf("\n%d\n", timeout);

    free(satisfiedLiterals);


}
