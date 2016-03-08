#include "walksat.h"

int main(){
	//FILE* fp = fopen("sat.cnf","r"); 
  FILE* fp = fopen("UF150.645.100/uf150-01.cnf", "r");
  if (fp == NULL){
    printf("file not found\n");
    exit(1);
  }
  	srand(time(NULL));
  	srand48(time(NULL));
  	
  walksat(fp, 10000000, 0.9);
}
