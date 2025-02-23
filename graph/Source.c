#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

// A neighbor node for adjacency list
struct Neighbor
{
    unsigned char value;
    struct Neighbor* next;
};
typedef struct Neighbor Neighbor;

// A vertex node, holding a value and its adjacency list
struct Vertex
{
    unsigned char value;
    struct Vertex* next;
    Neighbor* neighbors;
};
typedef struct Vertex Vertex;
typedef Vertex* Graph;

// Prototypes
Vertex* createVertex(unsigned char);
Neighbor* createNeighbor(unsigned char);
Vertex* searchVertex(Vertex*, unsigned char);
void addEdge(Graph*, unsigned char, unsigned char);
void printGraph(Graph);
void refineMatrix(int***, unsigned char);
void convertGraphToMatrix(int***, Graph);
void printMatrix(int**, unsigned char);

// Main
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
    refineMatrix(&matrix, noNodes);
    convertGraphToMatrix(&matrix, graph);
    printMatrix(matrix, noNodes);
}

// Creates a new vertex
Vertex* createVertex(unsigned char value)
{
    Vertex* result = (Vertex*)malloc(sizeof(Vertex));
    result->value = value;
    result->next = NULL;
    result->neighbors = NULL;
    return result;
}

// Creates a new neighbor node
Neighbor* createNeighbor(unsigned char value)
{
    Neighbor* result = (Neighbor*)malloc(sizeof(Neighbor));
    result->value = value;
    result->next = NULL;
    return result;
}

// Searches for a vertex in the graph by its value
Vertex* searchVertex(Vertex* graph, unsigned char value)
{
    while (graph && graph->value != value)
    {
        graph = graph->next;
    }
    return graph;
}

// Prints the adjacency list representation of the graph
void printGraph(Graph graph)
{
    while (graph)
    {
        printf("Vertex %d:\n", graph->value);
        Neighbor* aux = graph->neighbors;
        while (aux)
        {
            printf("%d ", aux->value);
            aux = aux->next;
        }
        printf("\n");
        graph = graph->next;
    }
}

// Initializes a square adjacency matrix, filled with 0
void refineMatrix(int*** matrix, unsigned char noEl)
{
    *matrix = (int**)malloc(noEl * sizeof(int*));
    for (int i = 0; i < noEl; i++)
    {
        (*matrix)[i] = (int*)malloc(noEl * sizeof(int));
        memset((*matrix)[i], 0, noEl * sizeof(int));
    }
}

// Adds an undirected edge between source and destination
void addEdge(Graph* graph, unsigned char source, unsigned char destination)
{
    if (*graph == NULL)
    {
        // If graph is empty, create 2 vertices (source & destination)
        *graph = createVertex(source);
        (*graph)->neighbors = createNeighbor(destination);

        Vertex* tmp = *graph;
        *graph = createVertex(destination);
        (*graph)->next = tmp;
        (*graph)->neighbors = createNeighbor(source);
    }
    else
    {
        // Handle the source vertex
        Vertex* vertex = searchVertex(*graph, source);
        if (vertex == NULL)
        {
            vertex = createVertex(source);
            vertex->next = *graph;
            *graph = vertex;
            (*graph)->neighbors = createNeighbor(destination);
        }
        else
        {
            Neighbor* neighbor = createNeighbor(destination);
            neighbor->next = vertex->neighbors;
            vertex->neighbors = neighbor;
        }

        // Handle the destination vertex
        vertex = searchVertex(*graph, destination);
        if (vertex == NULL)
        {
            vertex = createVertex(destination);
            vertex->next = *graph;
            *graph = vertex;
            (*graph)->neighbors = createNeighbor(source);
        }
        else
        {
            Neighbor* neighbor = createNeighbor(source);
            neighbor->next = vertex->neighbors;
            vertex->neighbors = neighbor;
        }
    }
}

// Converts adjacency list to adjacency matrix
void convertGraphToMatrix(int*** matrix, Graph graph)
{
    while (graph)
    {
        Neighbor* nbr = graph->neighbors;
        while (nbr)
        {
            (*matrix)[graph->value - 1][nbr->value - 1] = 1;
            nbr = nbr->next;
        }
        graph = graph->next;
    }
}

// Prints the adjacency matrix
void printMatrix(int** matrix, unsigned char noEl)
{
    printf("\nAdjacency Matrix:\n\n");
    for (int i = 0; i < noEl; i++)
    {
        for (int j = 0; j < noEl; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
