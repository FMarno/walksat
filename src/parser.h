#ifndef PARSER_H
#define PARSER_H
#include "list.h"
#include "walksat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void parseHeader(FILE* fp, int* vars, int* clauses);
void parseData(FILE* fp, int vars, int clauses, List* clausesContainingLiteral[vars*2], List* litsInClause[clauses]);
#endif