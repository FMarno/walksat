#ifndef HEADER
#define HEADER

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 20
#define THRESHOLD 1000000
#define P 0.5

int** parseData(FILE* fp, int* vars, int* clauses);
#endif