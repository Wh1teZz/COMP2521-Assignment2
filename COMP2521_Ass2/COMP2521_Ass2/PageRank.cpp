#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "graph.h"
#include "pagerank.h"

int main(int argc, char **argv) {

	// check usage of function
	if (argc != 4) {
		fprintf(stderr, "Usage: ./pagerank dampener diffPR maxIterations\n");
		return EXIT_FAILURE;
	}

	// retrieve arguments
	double dampener = atof(argv[1]),
		diffPR = atof(argv[2]);
	int maxIterations = atoi(argv[3]);

	// read from collection.txt and put in char **urls
	char **urls = malloc(sizeof(char*));
	FILE *fp = fopen("collection.txt", "r");
	if (!fp) { fprintf(stderr, "failed to open collection.txt\n"); return EXIT_FAILURE; }
	char* temp;
	int count = 0;
	while (fscanf(fp, "%s", temp) != EOF) {
		urls[count++] = strdup(temp);
		urls = realloc(urls, (count + 1) * sizeof(char*));
	}
	fclose(fp);

	// setup graph and perform calculations
	Out **out = malloc(count * sizeof(struct outputLine*)); //free later
	Graph g = newGraph(count);
	fillGraph(g, urls);
	double *pageRank = calculatePageRank(g, dampener, diffPR, maxIterations);

	// put into a single struct for sorting
	int i;
	for (i = 0; i < count; i++) {
		out[i] = malloc(sizeof(Out));
		out[i]->rank = pageRank[i];
		out[i]->url = strdup(urls[i]);
		free(urls[i]);
		out[i]->outLinks = numOutLinks(g, i);
	}

	free(pageRank);

	sort(out, count);

	writeToText(out, count);

	freeOut(out, count);

	free(out);

	return EXIT_SUCCESS;
}

double *calculatePageRank(Graph g, double dampener, double diffPR, int maxIterations){

	/* Quick and dirty translation of algo to english/python/shit pseudocode

	N = number urls in collection
	pagerank = array of url weights

	for i in range(N)
		pagerank[i] = 1/N
	
	i = 0
	diff = diffPR

				given by argv
					|
	while i < maxIteration && diff > diffPR

		This was added after I realised I needed to keep prev version of PageRank
		>>
		for i in range(N)
			OldPageRank[i] = NewPageRank[i]
		<<

		NewPageRank[i] = ((((1-d)/Ndouble)+(d*sum);
										  |
										  |
								can be broken down as (see below)
	
	NOTE:
	W-in is defined in
	https://www.cse.unsw.edu.au/~cs2521/18s2/assignments/ass02/page1725348.pdf
	as "inlinks / inlinks across all reference pages"
	An inlink is a webpage leading into the current webpage
	W-out is defined using outlinks instead of inlinks

		Therefore, sum is:

			for j in range(N)

				for k in range(N) <-- we are traversing a 2d array

					if a link exists between k and j

						apply formula for W-In and W-Out\

						sum += OldPageRank[k] * W-In * W-Out
		

	Now it's time to calculate diff

		diff = 0
		for j in range(N)
			diff += fabs(NewPageRank - OldPageRank)

		finally, increment i for while loop
	*/

	int N = getnV(g);
	double NDouble = (double)N,
		*currPR = malloc(sizeof(double) * N),
		*oldPR = malloc(sizeof(double) * N);
	int i, j, k;

	// set PR(Pj;t)
	for (i = 0; i < N; i++) {
		currPR[i] = 1 / NDouble;
	}

	//set up var for main case
	double diff = diffPR,
		wIn = 0,
		wOut = 0,
		inOutSum = 0;

	i = 0;
	while (i < maxIterations && diff >= diffPR) {

		// set PR(P-i;t)
		for (j = 0; j < N; j++) {
			oldPR[j] = currPR[j];
		}

		// for each PR(P-j;t) that is an element of PR(P-i;t)
		// calculate PR(P-i;t + 1)
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				if (isLinked(g, j, k)) {
					wIn = numInLinks(g, k) / sumInLinks(g, j);
					wOut = numOutLinks(g, k) / sumOutLinks(g, j);
					inOutSum += oldPR[k] * wIn * wOut;
					//printf("wIn[%d][%d]: %f, wOut[%d][%d]: %f\n", j, k, wIn, j, k, wOut);
				}
			}
			currPR[j] = ((1 - dampener) / NDouble) + (dampener * inOutSum);
			inOutSum = 0;
		}

		// find new diff
		diff = 0;
		for (j = 0; j < N; j++) {
			diff += fabs(currPR[j] - oldPR[j]);
		}

		i++;
	}
	free(oldPR);
	return currPR; //remember to free currPR
}

void sort(Out **out, int length) {
	int i, j;
	Out* temp;
	for (i = 0; i < length; i++) {
		for (j = 1; j < length; j++) {
			if (out[j - 1]->rank < out[j]->rank) {
				temp = out[j - 1];
				out[j - 1] = out[j];
				out[j] = temp;
			}
		}
	}
}

void freeOut(Out **out, int length) {
	int i;
	for (i = 0; i < length; i++) {
		free(out[i]->url);
		free(out[i]);
	}
}

void writeToText(Out **out, int length) {
	int i;
	FILE *fp = fopen("pagerankList.txt", "w");
	for (i = 0; i < length; i++) {
		fprintf(fp, "%s, %d, %.7f\n", out[i]->url, out[i]->outLinks, out[i]->rank);
	}
	fclose(fp);
}

