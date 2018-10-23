#ifndef PAGE_H
#define PAGE_H

#include "pagerank.h"

typedef struct outputLine {
	char *url;
	int outLinks;
	double rank;
} Out;

double *calculatePageRank(Graph g, double dampener, double diffPR, int maxIterations);
void sort(Out **out, int length);
void freeOut(Out **out, int length);
void writeToText(Out **out, int length);

#endif