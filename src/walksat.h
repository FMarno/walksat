#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"


#define BUFFER_SIZE 20

void flip(int varIndex, int vars, int clauses, int* variables,List* clausesContainingLiteral[vars*2],int satisfiedLiterals[clauses],List* unsatisfiedClauses);
void fillVariables(int* variables, int vars);
int getRandomUnsatisfiedClauseIndex(List* unsatisfiedClauses);
int walksat(FILE* , int THRESHOLD, float P);
int pickVar(List* lits, int vars, List* clausesContainingLiteral[vars*2], int* satisfiedLiterals);
int pickRandomVar(List* lits);
int made(List* clausesContainingLiteral,int* satisfiedLiterals);
int broken(List* clausesContainingLiteral, int* satisfiedLiterals);

#endif
