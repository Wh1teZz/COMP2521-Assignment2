#pragma once
// Graph ADT
// Ctrl+C and Ctrl+V from Week 8 lab Graph.c
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

//  typedefs
//
//

typedef struct GraphRep GraphRep;
typedef struct GraphRep *Graph;
typedef struct Edge Edge;
typedef int Vertex;

//  helper functions
//  
//  

// get number of edges
int getnE(Graph g);

// get number of vertices
int getnV(Graph g);

// find weight between 2 vertices
int isLinked(Graph g, Vertex v, Vertex w);

// check if graph and vertex exists
int isValidV(Graph g, Vertex v);

//  Actual Functions
//
//

// create an empty graph
Graph newGraph(int nV);

// fill a graph using collection.txt
void fillGraph(Graph g, char **urls);

// create an edge between v and w
Edge makeEdge(Graph g, Vertex v, Vertex w);

// - sets g->[v][w]
void insertEdge(Graph g, Vertex v, Vertex w, int weight);

// - sets g->[v][w] to 0
void removeEdge(Graph g, Vertex v, Vertex w);

// display graph, using names for vertices
void showGraph(Graph g);

//return number of inLinks for vertex v
double numInLinks(Graph g, Vertex v);

//return number of outLinks for vertex v
double numOutLinks(Graph g, Vertex v);

//return the sum of inLinks for all vertices connected to v
double sumInLinks(Graph g, Vertex v);

//return the sum of outLinks for all vertices connected to v
double sumOutLinks(Graph g, Vertex v);

//free memory associated with graph 
void freeGraph(Graph g);
