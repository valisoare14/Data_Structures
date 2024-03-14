//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<string.h>
//#include<stdlib.h>
//#include<malloc.h>
//
//struct Neighbour
//{
//	int value;
//	struct Neighbour* next;
//};
//typedef struct Neighbour Neighbour;
//
//struct Vertex
//{
//	int value;
//	Neighbour* neighbours;
//	struct Vertex* next;
//};
//typedef struct Vertex Vertex;
//typedef struct Vertex* Graph;
//
////memory
//void addEdge(Vertex** , int, int);
//Vertex* createVertex(int);
//void printGraph(Vertex* );
//Neighbour* createNeighbour(int);
//void allocateMatrix(int***, int);
//void populateMatrix(int***, Graph);
//void printMatrix(int**,int);
//
//
//void main()
//{
//	Graph graph = NULL;
//	addEdge(&graph, 1, 4);
//	addEdge(&graph, 1, 5);
//	addEdge(&graph, 2, 4);
//	addEdge(&graph, 2, 5);
//	addEdge(&graph, 3, 1);
//	addEdge(&graph, 6, 1);
//	addEdge(&graph, 2, 3);
//	printGraph(graph);
//	int** matrix = NULL;
//	allocateMatrix(&matrix, 6);
//	populateMatrix(&matrix, graph);
//	printMatrix(matrix , 6);
//}
//void addEdge(Vertex** graph, int source, int destination)
//{
//	if (*graph == NULL)
//	{
//		*graph = createVertex(source);
//		(*graph)->neighbours = createNeighbour(destination);
//		Vertex* destinationVertex = createVertex(destination);
//		destinationVertex->next = *graph;
//		destinationVertex->neighbours = createNeighbour(source);
//		*graph = destinationVertex;
//	}
//	else
//	{
//		Vertex* aux = *graph;
//		while (aux != NULL && aux->value != source) {
//			aux = aux->next;
//		}
//		if (aux)
//		{
//			Neighbour* vecin = createNeighbour(destination);
//			vecin->next = aux->neighbours;
//			aux->neighbours = vecin;
//		}
//		else
//		{
//			aux = createVertex(source);
//			aux->next = *graph;
//			*graph = aux;
//			Neighbour* vecin = createNeighbour(destination);
//			vecin->next = (*graph)->neighbours;
//			(*graph)->neighbours = vecin;
//		}
//		Vertex* res = *graph;
//		while (res != NULL && res->value != destination) {
//			res = res->next;
//		}
//		if (res)
//		{
//			Neighbour* vecin = createNeighbour(source);
//			vecin->next = res->neighbours;
//			res->neighbours = vecin;
//		}
//		else
//		{
//			res = createVertex(destination);
//			res->next = *graph;
//			*graph = res;
//			Neighbour* vecin = createNeighbour(source);
//			vecin->next=(*graph)->neighbours;
//			(*graph)->neighbours = vecin;
//		}
//	}
//}
//Vertex* createVertex(int source)
//{
//	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
//	vertex->value = source;
//	vertex->next = NULL;
//	vertex->neighbours = NULL;
//	return vertex;
//}
//Neighbour* createNeighbour(int destination)
//{
//	Neighbour* res = (Neighbour*)malloc(sizeof(Neighbour));
//	res->value = destination;
//	res->next = NULL;
//	return res;
//}
//void printGraph(Vertex* graph)
//{
//	if (graph)
//	{
//		do
//		{
//			printf("Sursa: %d\nDestinatii : " , graph->value);
//			do
//			{
//				printf("%d ", graph->neighbours->value);
//				graph->neighbours = graph->neighbours->next;
//			} while (graph->neighbours);
//			printf("\n");
//			graph = graph->next;
//		} while (graph);
//	}
//}
//void allocateMatrix(int*** matrix, int n)
//{
//	*matrix = (int**)malloc(sizeof(int*) * n);
//	for (int i = 0; i < n; i++)
//	{
//		(*matrix)[i] = (int*)malloc(n * sizeof(int));
//		memset((*matrix)[i], 0, n * sizeof(int));
//	}
//}
//void populateMatrix(int*** matrix, Graph graph)
//{
//	while (graph)
//	{
//		while (graph->neighbours)
//		{
//			(*matrix)[graph->value - 1][graph->neighbours->value - 1] = 1;
//			graph->neighbours = graph->neighbours->next;
//		}
//		graph = graph->next;
//	}
//}
//void printMatrix(int** matrix,int size)
//{
//	printf("Matrix:\n----------------\n");
//	for (int i = 0; i< size; i++)
//	{
//		for (int j = 0; j < size; j++)
//		{
//			printf("%d ", matrix[i][j]);
//		}
//		printf("\n");
//	}
//}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Neighbour
{
	unsigned char value;
	struct Neighbour* next;
};
typedef struct Neighbour Neighbour;

struct Vertex
{
	unsigned char value;
	struct Vertex* next;
	Neighbour* neighbours;
};
typedef struct Vertex Vertex;
typedef struct Vertex* Graph;

//usefull info memory
void addEdge(Graph*, unsigned char, unsigned char);
Vertex* createVertex(unsigned char);
Neighbour* createNeighbour(unsigned char);
Vertex* searchVertex(Vertex*, unsigned char);
void printGraph(Graph);
void refineMatrix(int***, unsigned char);
void convertGraphToMatrix(int***, Graph);
void printMatrix(int**, unsigned char);

void main()
{
	int noNodes = 6;
	Graph graph = NULL;
	addEdge(&graph, 1, 2);
	addEdge(&graph, 1, 4);
	addEdge(&graph, 1, 5);
	addEdge(&graph, 2, 5);
	addEdge(&graph, 3, 5);
	addEdge(&graph, 3, 6);
	addEdge(&graph, 4, 6);
	addEdge(&graph, 5, 6);
	printGraph(graph);
	int** matrix = NULL;
	refineMatrix(&matrix, 6);
	convertGraphToMatrix(&matrix, graph);
	printMatrix(matrix, 6);
}
Vertex* createVertex(unsigned char value)
{
	Vertex* result = (Vertex*)malloc(sizeof(Vertex));
	result->value = value;
	result->next = NULL;
	result->neighbours = NULL;
	return result;
}
Neighbour* createNeighbour(unsigned char value)
{
	Neighbour* result = (Neighbour*)malloc(sizeof(Neighbour));
	result->value = value;
	result->next = NULL;
}
Vertex* searchVertex(Vertex* graph, unsigned char value)
{
	while (graph && graph->value != value)
	{
		graph = graph->next;
	}
	return graph;
}
void printGraph(Graph graph)
{
	while (graph)
	{
		printf("Nod %d :\n", graph->value);
		Neighbour* aux = graph->neighbours;
		while (aux)
		{
			printf("%d  ", aux->value);
			aux = aux->next;
		}
		printf("\n");
		graph = graph->next;
	}
}
void refineMatrix(int*** matrix, unsigned char noEl)
{
	*matrix = (int**)malloc(noEl * sizeof(int*));
	for (int i = 0; i < noEl; i++)
	{
		(*matrix)[i] = (int*)malloc(noEl * sizeof(int));
		memset((*matrix)[i], 0, noEl * sizeof(int));
	}
}


void addEdge(Graph* graph, unsigned char sursa, unsigned char destinatie)
{
	if (*graph == NULL)
	{
		*graph = createVertex(sursa);
		(*graph)->neighbours = createNeighbour(destinatie);
		Vertex* aux = *graph;
		*graph = createVertex(destinatie);
		(*graph)->next = aux;
		(*graph)->neighbours = createNeighbour(sursa);
	}
	else
	{
		Vertex* vertex = searchVertex(*graph, sursa);
		if (vertex == NULL)
		{
			vertex = createVertex(sursa);
			vertex->next = *graph;
			*graph = vertex;
			(*graph)->neighbours = createNeighbour(destinatie);
		}
		else
		{
			Neighbour* neighbour = createNeighbour(destinatie);
			neighbour->next = vertex->neighbours;
			vertex->neighbours = neighbour;
		}
		//
		vertex = searchVertex(*graph, destinatie);
		if (vertex == NULL)
		{
			vertex = createVertex(destinatie);
			vertex->next = *graph;
			*graph = vertex;
			(*graph)->neighbours = createNeighbour(sursa);
		}
		else
		{
			Neighbour* neighbour = createNeighbour(sursa);
			neighbour->next = vertex->neighbours;
			vertex->neighbours = neighbour;
		}
	}
}
void convertGraphToMatrix(int*** matrix, Graph graph)
{
	while (graph)
	{
		while (graph->neighbours)
		{
			(*matrix)[graph->value - 1][graph->neighbours->value - 1] = 1;
			graph->neighbours = graph->neighbours->next;
		}
		graph = graph->next;
	}
}
void printMatrix(int** matrix, unsigned char noEl)
{
	printf("\n\nMatrix:\n\n");
	for (int i = 0; i < noEl; i++)
	{
		for (int j = 0; j < noEl; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

