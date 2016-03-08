#include "walksat.h"
#include "list.h"
#include "parser.h"



int walksat(FILE* fp, int THRESHOLD, float P){
  //start a clock to record the time
  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);

  //read the number of vars and clauses
  int vars; int clauses;
  parseHeader(fp, &vars, &clauses);

  //alloc all the space for the clausesContainingLiteral
  List* clausesContainingLiteral[vars*2];
  for (int i = 0; i < vars*2; ++i){
  clausesContainingLiteral[i] = createList();
  }

  //also alloc a list of lits in clause as an attemp at optimisation
  List* litsInClause[clauses];
  for (int i = 0; i < clauses; i++){
    litsInClause[i] = createList();
  }

  //read in the clauses
  parseData(fp, vars, clauses, clausesContainingLiteral, litsInClause);

  //zero an array for satusfiedLiterals per clause
  int* satisfiedLiterals = (int*)calloc(clauses, sizeof(int));

  int variables[vars];

  //randomly fill the varisables
  fillVariables(variables, vars);



  //satisfied vars per clause
  for (int i = 0; i < (vars*2); i++){
    ListNode* node = clausesContainingLiteral[i]->root;
    for (int x = 0; x < lengthOfList(clausesContainingLiteral[i]); x++){
      int clause = node->value;
      if ((i % 2) == 0){
        //positive variables
        if (variables[i/2]){
        satisfiedLiterals[clause]++;
        }
      } else {
        //negated variables
        if (variables[(i-1)/2] == 0){
        satisfiedLiterals[clause]++;
        }
      }
      node = node->next;
    }
  }

  //create a list of the unsatisfiedClauses
  List* unsatisfiedClauses = createList();
  for (int i = 0; i<clauses; i++){
    if (satisfiedLiterals[i] == 0){
      add(unsatisfiedClauses, i);
    }
  }

  float r;
  int timeout = 0;
  while (lengthOfList(unsatisfiedClauses) && timeout < THRESHOLD){
    //get a random unsatisfied clause
    int randomClause = getRandomUnsatisfiedClauseIndex(unsatisfiedClauses);
    //create a random float between 0 and 1
    r = drand48();

    int varIndex;
    if (r > P){
      //select the variable from the clause that will cause the most clauses to be satisfied
      varIndex = pickVar(litsInClause[randomClause], vars, clausesContainingLiteral, satisfiedLiterals);
    } else {
      //select a random variable from the clause
      varIndex = pickRandomVar(litsInClause[randomClause]);
    }

    //flip the selected variable and update structures
    flip(varIndex, vars, clauses, variables, clausesContainingLiteral, satisfiedLiterals, unsatisfiedClauses);

    timeout++;
  }
  //print info
  if (timeout == THRESHOLD){
    printf("UNKNOWN\n");
  } else {
    printf("SATISFIABLE\n");
  }

  printf("Variables: %d\tClauses: %d\n",vars, clauses);


  for (int i = 0; i < vars; ++i){
    printf("%d", variables[i]);
  }
  printf("\n");

  printf("Loops: %d\n", timeout);

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("Time ~= %.9f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));


  //free everything up
  for (int i = 0; i < vars*2; ++i){
    freeList(clausesContainingLiteral[i]);
  }

  for (int i = 0; i < clauses; i++){
    freeList(litsInClause[i]);
  }

  free(satisfiedLiterals);

  if (timeout == THRESHOLD){
  return 0;
  } 
  return 1;
}

void flip(int varIndex, int vars, int clauses, int* variables, List* clausesContainingLiteral[vars*2], int satisfiedLiterals[clauses],List* unsatisfiedClauses){
  variables[varIndex] = !variables[varIndex];
  if (variables[varIndex]){
    //flipped to true
    ListNode* node = clausesContainingLiteral[varIndex*2]->root;
    //for each clause containing the positive literal
    for (int i = 0; i < lengthOfList(clausesContainingLiteral[2*varIndex]); i++){
      int clause = node->value;
      //increase the number of satisfied literals in the clause
      satisfiedLiterals[clause]++;
      //if that is the first one, remove from list
      if (satisfiedLiterals[clause] == 1){
        removeValue(unsatisfiedClauses, clause);
      }
      node = node->next;
    }
    node = clausesContainingLiteral[(varIndex*2) + 1]->root;
    for (int i = 0; i<lengthOfList(clausesContainingLiteral[(2*varIndex)+1]); i++){
      int clause = node->value;
      satisfiedLiterals[clause]--;
      if (satisfiedLiterals[clause] == 0){
        add(unsatisfiedClauses, clause);
      }
      node = node->next;
    }
  } else {
    ListNode* node = clausesContainingLiteral[varIndex*2]->root;
    for (int i = 0; i < lengthOfList(clausesContainingLiteral[2*varIndex]); i++){
      int clause = node->value;
      satisfiedLiterals[clause]--;
      if (satisfiedLiterals[clause] == 0){
        add(unsatisfiedClauses, clause);
      }
      node = node->next;
    }
    node = clausesContainingLiteral[(varIndex*2) + 1]->root;
    for (int i = 0; i<lengthOfList(clausesContainingLiteral[(2*varIndex)+1]); i++){
      int clause = node->value;
      satisfiedLiterals[clause]++;
      if (satisfiedLiterals[clause] == 1){
        removeValue(unsatisfiedClauses, clause);
      }
      node = node->next;
    }
  }
}

void fillVariables(int* variables, int vars){
  for (int i = 0; i<vars; i++){
    variables[i] = rand()% 2;
  }
}

int getRandomUnsatisfiedClauseIndex(List* unsatisfiedClauses){
  return get(unsatisfiedClauses, rand() % lengthOfList(unsatisfiedClauses));
}

int pickVar(List* lits, int vars, List* clausesContainingLiteral[vars*2], int* satisfiedLiterals){
  ListNode* node = lits->root;
  int best = node->value;

  int var = best/2;

  int bestProfit = made(clausesContainingLiteral[var*2], satisfiedLiterals) - broken(clausesContainingLiteral[(var*2)+1], satisfiedLiterals);
  node = node->next;
  for (int i = 1; i<lengthOfList(lits); i++){
    var = (node->value)/2;
    int profit = made(clausesContainingLiteral[var*2], satisfiedLiterals) - broken(clausesContainingLiteral[(var*2)+1], satisfiedLiterals);
    if (profit > bestProfit){
      bestProfit = profit;
      best = node->value;
    }
    node = node->next;
  }
  return best/2;
}

int made(List* clausesContainingLiteral,int* satisfiedLiterals){
  int sum = 0;
  if (lengthOfList(clausesContainingLiteral) != 0){
    ListNode* node = clausesContainingLiteral->root;
    for (int i = 0; i< lengthOfList(clausesContainingLiteral); i++){
      if (satisfiedLiterals[node->value] == 0){
        sum++;
      }
      node = node->next;
    }
  }
  return sum;
}

int broken(List* clausesContainingLiteral, int* satisfiedLiterals){
  int sum = 0;
  if (lengthOfList(clausesContainingLiteral) != 0){
    ListNode* node = clausesContainingLiteral->root;
    for (int i = 0; i< lengthOfList(clausesContainingLiteral); i++){
      if (satisfiedLiterals[node->value] == 1){
        sum++;
      }
      node = node->next;
    }
  }
  return sum;

}

int pickRandomVar(List* lits){
  int varIndex = get(lits, rand() % lengthOfList(lits)) /2;
  return varIndex;
}