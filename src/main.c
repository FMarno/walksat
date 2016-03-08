#include "walksat.h"

int main(int argc, char** argv){
	if (argc != 4){
		printf("Wrong number of arguments\n");
		printf("useage: ./WalkSAT [file name] [threshold] [probability]\n");
	} else {
		//FILE* fp = fopen("sat.cnf","r"); 
		FILE* fp = fopen(argv[1], "r");
		if (fp == NULL){
			printf("file not found\n");
			exit(1);
		}
		srand(time(NULL));
		srand48(time(NULL));

		int threshold; float probability;
		sscanf(argv[2], "%d", &threshold);
		
		sscanf(argv[3], "%f", &probability);
		if (probability > 0 && probability < 1){
			walksat(fp, threshold, probability);
		} else {
			printf("Probability in inappriopriate range.\n\
				\rPlease select a number between (0,1) exclusive\n");
		}
	}
}
