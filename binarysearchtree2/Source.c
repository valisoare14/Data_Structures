#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#define LINE_SIZE 128

// Structure describing a reservation
struct Reservation
{
	unsigned int reservation_id;
	char* hotelName;
	unsigned char roomCount;
	char* clientName;
	float paymentAmount;
};
typedef struct Reservation ReservationInfo;

// Classic BST structure (keyed by reservation_id)
struct BST
{
	struct BST* left;
	struct BST* right;
	ReservationInfo* info;
};
typedef struct BST BinarySearchTree;

// Function prototypes
ReservationInfo* createReservation(unsigned char, char*, unsigned char, char*, float);
void insertNode(BinarySearchTree**, ReservationInfo*);
BinarySearchTree* createNode(ReservationInfo*);
void preOrder(BinarySearchTree*);
void displayReservation(ReservationInfo*);
float totalPayment(BinarySearchTree*, char*);
void deleteMin(BinarySearchTree**);
int* numberOfNodesPerLevel(BinarySearchTree*);
int searchNumberOfLevels(BinarySearchTree*);
void populateVector(BinarySearchTree*, int*, int);
void displayVector(int*, int);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		unsigned int reservation_id;
		char hotelName[LINE_SIZE];
		unsigned char roomCount;
		char clientName[LINE_SIZE];
		float paymentAmount;
		char* token;
		char delimiter[] = ",\n";
		char lineBuffer[LINE_SIZE];
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile))
		{
			token = strtok(lineBuffer, delimiter);
			reservation_id = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(hotelName, token);
			token = strtok(NULL, delimiter);
			roomCount = (unsigned char)atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(clientName, token);
			token = strtok(NULL, delimiter);
			paymentAmount = (float)atof(token);
			ReservationInfo* reservation = createReservation(reservation_id, hotelName, roomCount, clientName, paymentAmount);
			insertNode(&BST, reservation);
		}
		preOrder(BST);

		printf("\n\nTotal payment: %f\n", totalPayment(BST, " Popescu Razvan "));

		{
			int* levelVector = numberOfNodesPerLevel(BST);
			displayVector(levelVector, searchNumberOfLevels(BST));
			deleteMin(&BST);
			printf("After deleting the minimum:\n");
			preOrder(BST);
		}
		{
			int* levelVector2 = numberOfNodesPerLevel(BST);
			displayVector(levelVector2, searchNumberOfLevels(BST));
		}
	}
}

// Creates a reservation
ReservationInfo* createReservation(unsigned char reservation_id, char* hotelName, unsigned char roomCount, char* clientName, float paymentAmount)
{
	ReservationInfo* r = (ReservationInfo*)malloc(sizeof(ReservationInfo));
	r->hotelName = (char*)malloc(strlen(hotelName) + 1);
	r->clientName = (char*)malloc(strlen(clientName) + 1);
	strcpy(r->hotelName, hotelName);
	strcpy(r->clientName, clientName);
	r->reservation_id = reservation_id;
	r->roomCount = roomCount;
	r->paymentAmount = paymentAmount;
	return r;
}

// Inserts a node into the BST
void insertNode(BinarySearchTree** BST, ReservationInfo* r)
{
	if (*BST == NULL)
	{
		*BST = createNode(r);
	}
	else
	{
		if ((*BST)->info->reservation_id > r->reservation_id)
		{
			insertNode(&(*BST)->left, r);
		}
		else if ((*BST)->info->reservation_id < r->reservation_id)
		{
			insertNode(&(*BST)->right, r);
		}
		else
		{
			ReservationInfo* aux = (*BST)->info;
			(*BST)->info = r;
			free(aux->hotelName);
			free(aux->clientName);
			free(aux);
		}
	}
}

// Creates a BST node
BinarySearchTree* createNode(ReservationInfo* r)
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	node->info = r;
	node->left = node->right = NULL;
	return node;
}

// Pre-order traversal
void preOrder(BinarySearchTree* BST)
{
	if (BST)
	{
		preOrder(BST->left);
		displayReservation(BST->info);
		preOrder(BST->right);
	}
}

// Displays one reservation
void displayReservation(ReservationInfo* r)
{
	printf("ID: %d -> Client Name: %s\n", r->reservation_id, r->clientName);
}

// Computes the total payment for a given client name
float totalPayment(BinarySearchTree* BST, char* client)
{
	if (BST)
	{
		if (strcmp(BST->info->clientName, client) == 0)
		{
			return BST->info->paymentAmount + totalPayment(BST->left, client) + totalPayment(BST->right, client);
		}
		else
		{
			return totalPayment(BST->left, client) + totalPayment(BST->right, client);
		}
	}
	else
	{
		return 0;
	}
}

// Deletes the minimum node
void deleteMin(BinarySearchTree** BST)
{
	if (*BST)
	{
		if ((*BST)->left)
		{
			deleteMin(&(*BST)->left);
		}
		else
		{
			if ((*BST)->right)
			{
				BinarySearchTree* aux = *BST;
				*BST = (*BST)->right;
				free(aux->info->hotelName);
				free(aux->info->clientName);
				free(aux->info);
				free(aux);
			}
			else
			{
				BinarySearchTree* temp = *BST;
				free(temp->info->hotelName);
				free(temp->info->clientName);
				free(temp->info);
				free(temp);
				*BST = NULL;
			}
		}
	}
}

// Returns an array with the number of nodes per level
int* numberOfNodesPerLevel(BinarySearchTree* BST)
{
	int numberOfLvls = searchNumberOfLevels(BST);
	int* vector = (int*)malloc(numberOfLvls * sizeof(int));
	memset(vector, 0, numberOfLvls * sizeof(int));
	int currentLevel = 1;
	populateVector(BST, vector, currentLevel);
	return vector;
}

// Determines how many levels the BST has
int searchNumberOfLevels(BinarySearchTree* BST)
{
	if (BST)
	{
		return 1 + max(searchNumberOfLevels(BST->left), searchNumberOfLevels(BST->right));
	}
	else
	{
		return 0;
	}
}

// Fills an array with the number of nodes at each level
void populateVector(BinarySearchTree* BST, int* vector, int currentLevel)
{
	if (BST)
	{
		vector[currentLevel - 1]++;
		populateVector(BST->left, vector, currentLevel + 1);
		populateVector(BST->right, vector, currentLevel + 1);
	}
}

// Displays an array of the number of nodes per level
void displayVector(int* vector, int n)
{
	printf("Levels:\n");
	for (int i = 0; i < n; i++)
	{
		printf("Level %d -> nodes: %d\n", i + 1, vector[i]);
	}
}
