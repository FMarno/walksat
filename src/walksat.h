#ifndef HEADER
#define HEADER

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 20
#define THRESHOLD 1000000
#define P 0.5

void parseHeader(FILE* fp, int* vars, int* clauses);
void parseData(FILE* fp, int vars, int clauses, int clausesContainingLiteral[vars*2][clauses]);
#endif