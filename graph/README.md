# Implementation of an Undirected Graph in C

This C file implements an **undirected graph** data structure using **adjacency lists** and then converts this structure into an **adjacency matrix**.

## Main Components and Their Functions:

### Data Structures:
- **Neighbour**: Represents the neighbors of a node in the graph (adjacency list).
- **Vertex**: Represents the nodes of the graph and contains a list of neighbors (**Neighbour**).
- **Graph**: A pointer to the first **Vertex** in the graph, representing the graph itself.

### Memory Management Functions:
- `createVertex()`: Creates a new **Vertex** in the graph.
- `createNeighbour()`: Creates a new **Neighbour** for a given node.

### Graph Manipulation Functions:
- `addEdge()`: Adds a new edge to the graph by updating the adjacency lists for both nodes involved in the edge.
- `searchVertex()`: Searches for a node in the graph based on its value.

## Display Functions:

- `printGraph()`: Displays the graph – nodes and their neighbors.
- `printMatrix()`: Displays the adjacency matrix of the graph.

## Graph to Adjacency Matrix Conversion:

- `refineMatrix()`: Initializes an adjacency matrix with all elements set to **0**.
- `convertGraphToMatrix()`: Converts the **graph structure** based on adjacency lists into an **adjacency matrix**, where each element `matrix[i][j]` is set to **1** if an edge exists between **node (i+1)** and **node (j+1)**.
