// Graph ADT
// Skeleton for this Graph ADT copied from Week 8 Lab Graph.c
// Functions and function names modified to interact with an 
// adjacency matrix.
// Functions used for graphical representation of graph changed
// for debugging purposes.
// Some functions added for the purpose of this assignement.
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

// graph representation using adjacency matrix
struct GraphRep {
	int    nV;
	int    nE;
	int  **edges; //edges are weighted
};


// representation of an edge between vertex v and w
struct Edge {
	Vertex v;
	Vertex w;
};

//  static declarations
//
//

static Vertex findOutLink(char *string, int length, char **str);

//  helper functions
//  
//  

// get number of edges
int getnE(Graph g) { return g->nE; }

// get number of vertices
int getnV(Graph g) { return g->nV; }

// find weight between 2 vertices
int isLinked(Graph g, Vertex v, Vertex w) { return (g->edges[v][w]); }

// check if graph and vertex exists
int isValidV(Graph g, Vertex v) { return (g != NULL && v >= 0 && v < g->nV) ? 1 : 0; }


//  Actual Functions
//
//

// create an empty graph
Graph newGraph(int nV)
{
	if (nV <= 0) return NULL;
	int v, w;
	Graph g = malloc(sizeof(GraphRep));
	assert(g != 0);
	g->nV = nV;
	g->nE = 0;
	g->edges = malloc(nV * sizeof(int *));
	assert(g->edges != 0);
	for (v = 0; v < nV; v++) {
		g->edges[v] = malloc(nV * sizeof(int));
		assert(g->edges[v] != 0);
		for (w = 0; w < nV; w++)
			g->edges[v][w] = 0;
	}
	return g;
}

// check each url and fill matrix
void fillGraph(Graph g, char **urls) {

	FILE *fp;
	int i, urlCount = getnV(g);
	char temp[3000];
	char *urlName;
	Vertex v;

	for (i = 0; i < urlCount; i++) {

		//setup for each iter
		urlName = strdup(urls[i]);
		strcat(urlName, ".txt");
		fp = fopen(urlName, "r");
		if (!fp) { fprintf(stderr, "failed to open a url.txt file\n"); return; }

		//make sure that only the outLinks of a URL are read
		while (fscanf(fp, "%s", temp) != EOF) {
			v = findOutLink(temp, urlCount, urls);
			if (v != -1) insertEdge(g, i, v, 1);
		}

		// cleanup after each iter
		fclose(fp);
		free(urlName);
	}
}

// return position if str is an element of urls, otherwise -1
static int findOutLink(char *str, int urlCount, char **urls) {
	int i;
	for (i = 0; i < urlCount; i++) {
		if (strcmp(str, urls[i]) == 0) return i;
	}
	return -1;
}

Edge makeEdge(Graph g, Vertex v, Vertex w) {
	Edge e;
	if (!g || !isValidV(g, v) || !isValidV(g, w)) {
		fprintf(stderr, "Failed to make edge\n");
		e.v = 0; e.w = 0;
		return e;
	}
	e.v = v; e.w = w;;
	return e;
}


// - sets g->[v][w]
void insertEdge(Graph g, Vertex v, Vertex w, int weight)
{
	if (v == w) return;
	else if (!g || !isValidV(g, v) || !isValidV(g, w)) {
		fprintf(stderr, "Failed to add edge\n");
	}
	else if (!g->edges[v][w]) {
		g->edges[v][w] = weight;
		g->nE++;
	}
	else fprintf(stderr, "Something went wrong with insertEdge\n");
}


// - sets g->[v][w] to 0
void removeEdge(Graph g, Vertex v, Vertex w)
{
	if (!g || !isValidV(g, v) || !isValidV(g, w)) {
		fprintf(stderr, "Failed to remove edge\n");
		return;
	}
	if (g->edges[v][w]) {
		g->edges[v][w] = 0;
		g->nE--;
	}
}


// display graph, using names for vertices
void showGraph(Graph g)
{
	if (!g) return;
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		for (w = 0; w < g->nV; w++) {
			printf("%d ", g->edges[v][w]);
		}
		printf("\n");
	}
}


//return number of inLinks for vertex v
double numInLinks(Graph g, Vertex v) {
	int i;
	double links = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[i][v]) {
			links++;
		}
	}
	return links;
}


//return number of outLinks for vertex v
double numOutLinks(Graph g, Vertex v) {
	int i;
	double links = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i]) {
			links++;
		}
	}
	if (links == 0) return 0.5;
	return links;
}

//return the sum of inLinks for all vertices connected to v
double sumInLinks(Graph g, Vertex v) {
	int i = 0;
	double sum = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i]) { //fdsfadfasfsafas
			sum += numInLinks(g, i);
		}
	}
	return sum;
}


//return the sum of outLinks for all vertices connected to v
double sumOutLinks(Graph g, Vertex v) {
	int i = 0;
	double sum = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i]) {
			sum += numOutLinks(g, i);
		}
	}
	return sum;
}


//free memory associated with graph 
void freeGraph(Graph g) {
	if (!g) {
		fprintf(stderr, "graph does not exist\n");
		return;
	}
	int i;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[i]) free(g->edges[i]);
	}
	if (g->edges[i]) free(g->edges);
	free(g);
}
