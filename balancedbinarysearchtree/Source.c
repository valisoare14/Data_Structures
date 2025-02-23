#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

struct Deposit
{
	char* name;
	char* location;
	int numEmployees;
	float salaries;
	int seniority;
};
typedef struct Deposit DepositInfo;

struct BinarySearchTree
{
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
	DepositInfo* info;
	int bFactor;
};
typedef struct BinarySearchTree BinarySearchTree;

//memory
DepositInfo* createDeposit(char*, char*, int, float, int);
BinarySearchTree* createNode(DepositInfo*);
void insertNode(BinarySearchTree**, DepositInfo*);
void rebalance(BinarySearchTree**);
int getHeight(BinarySearchTree*);
void leftRotation(BinarySearchTree**);
void rightRotation(BinarySearchTree**);
void inorder(BinarySearchTree*);
void displayDeposit(DepositInfo*);
DepositInfo** vectorOfDepositsOnSpecifiedLevel(BinarySearchTree*, int*, int);
int searchNumberOfLevels(BinarySearchTree*);
int searchNumberOfNodesOnSpecifiedLevel(BinarySearchTree*, int, int);
void populateVector(BinarySearchTree*, DepositInfo**, int, int, int*);
void displayVector(DepositInfo**, int*);
DepositInfo** roadVector(BinarySearchTree*, int*, int);
int searchNumberOfNodesOnRoad(BinarySearchTree*, int);
void populateRoadVector(BinarySearchTree*, DepositInfo**, int, int*);
void deleteLeavesNodes(BinarySearchTree**);
int maximumNumberOfNodesLevel(BinarySearchTree*);
void generalDeallocation(BinarySearchTree**, DepositInfo**, DepositInfo**, int*, int*);
void deallocateBST(BinarySearchTree**);
void deallocateVector(DepositInfo**, int*);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		char name[128];
		char location[128];
		int numEmployees;
		float salaries;
		int seniority;
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			strcpy(name, token);
			token = strtok(NULL, delimiter);
			strcpy(location, token);
			token = strtok(NULL, delimiter);
			numEmployees = atoi(token);
			token = strtok(NULL, delimiter);
			salaries = atof(token);
			token = strtok(NULL, delimiter);
			seniority = atoi(token);
			DepositInfo* dep = createDeposit(name, location, numEmployees, salaries, seniority);
			insertNode(&BST, dep);
		}
		inorder(BST);
		{
			int* noEL = (int*)malloc(sizeof(int));
			noEL[0] = 0;
			DepositInfo** vector = vectorOfDepositsOnSpecifiedLevel(BST, noEL, 2);
			displayVector(vector, noEL);
			{
				int* noEl1 = (int*)malloc(sizeof(int));
				noEl1[0] = 0;
				DepositInfo** vector1 = roadVector(BST, noEl1, 9);
				displayVector(vector1, noEl1);
				{
					int maxLevel = maximumNumberOfNodesLevel(BST);
					printf("Level with the maximum number of nodes : %d\n", maxLevel);

					deleteLeavesNodes(&BST);
					printf("After deleting leaves:\n");
					inorder(BST);

					generalDeallocation(&BST, vector, vector1, noEL, noEl1);
					printf("After deleting structures:\n------------------\n");
					inorder(BST);
					displayVector(vector1, noEl1);
					displayVector(vector, noEL);
				}
			}
		}
	}
}
DepositInfo* createDeposit(char* name, char* location, int numEmployees, float salaries, int seniority)
{
	DepositInfo* dep = (DepositInfo*)malloc(sizeof(DepositInfo));
	dep->name = (char*)malloc(strlen(name) + 1);
	dep->location = (char*)malloc(strlen(location) + 1);
	strcpy(dep->name, name);
	strcpy(dep->location, location);
	dep->numEmployees = numEmployees;
	dep->salaries = salaries;
	dep->seniority = seniority;
	return dep;
}
BinarySearchTree* createNode(DepositInfo* dep)
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	node->info = dep;
	node->left = node->right = NULL;
	node->bFactor = 0;
	return node;
}
void insertNode(BinarySearchTree** BST, DepositInfo* dep)
{
	if (*BST == NULL)
	{
		*BST = createNode(dep);
	}
	else
	{
		if ((*BST)->info->numEmployees > dep->numEmployees)
		{
			insertNode(&(*BST)->left, dep);
		}
		else if ((*BST)->info->numEmployees < dep->numEmployees)
		{
			insertNode(&(*BST)->right, dep);
		}
		else
		{
			DepositInfo* aux = (*BST)->info;
			(*BST)->info = dep;
			free(aux->name);
			free(aux->location);
			free(aux);
		}
	}
	rebalance(BST);
}
void rebalance(BinarySearchTree** BST)
{
	(*BST)->bFactor = getHeight((*BST)->right) - getHeight((*BST)->left);
	if ((*BST)->bFactor == 2)
	{
		if (getHeight((*BST)->right) == -1)
		{
			rightRotation(&(*BST)->right);
			leftRotation(BST);
		}
		else
		{
			leftRotation(BST);
		}
	}
	if ((*BST)->bFactor == -2)
	{
		if (getHeight((*BST)->left) == 1)
		{
			leftRotation(&(*BST)->left);
			rightRotation(BST);
		}
		else
		{
			rightRotation(BST);
		}
	}
}
int getHeight(BinarySearchTree* root)
{
	if (root)
	{
		return 1 + max(getHeight(root->right), getHeight(root->left));
	}
	else
	{
		return 0;
	}
}
void rightRotation(BinarySearchTree** pivot)
{
	BinarySearchTree* descendant = (*pivot)->left;
	BinarySearchTree* aux = descendant->right;
	(*pivot)->left = aux;
	descendant->right = *pivot;
	*pivot = descendant;
}
void leftRotation(BinarySearchTree** pivot)
{
	BinarySearchTree* descendant = (*pivot)->right;
	BinarySearchTree* aux = descendant->left;
	(*pivot)->right = aux;
	descendant->left = *pivot;
	*pivot = descendant;
}
void inorder(BinarySearchTree* BST)
{
	if (BST)
	{
		inorder(BST->left);
		displayDeposit(BST->info);
		inorder(BST->right);
	}
}
void displayDeposit(DepositInfo* dep)
{
	printf("Name : %s -> Number of employees : %d\n", dep->name, dep->numEmployees);
}
DepositInfo** vectorOfDepositsOnSpecifiedLevel(BinarySearchTree* BST, int* noEL, int level)
{
	int nrLevels = searchNumberOfLevels(BST);
	if (level <= nrLevels)
	{
		noEL[0] = searchNumberOfNodesOnSpecifiedLevel(BST, level, 1);
		DepositInfo** vector = (DepositInfo**)malloc(noEL[0] * sizeof(DepositInfo*));
		memset(vector, 0, noEL[0] * sizeof(DepositInfo*));
		int* currentPosition = (int*)malloc(sizeof(int));
		currentPosition[0] = 0;
		populateVector(BST, vector, level, 1, currentPosition);
		return vector;
	}
	else
	{
		return NULL;
	}
}
int searchNumberOfLevels(BinarySearchTree* BST)
{
	if (BST)
	{
		return 1 + max(getHeight(BST->right), getHeight(BST->left));
	}
	else
	{
		return 0;
	}
}
int searchNumberOfNodesOnSpecifiedLevel(BinarySearchTree* BST, int level, int currentLevel)
{
	if (BST)
	{
		if (currentLevel == level)
		{
			return 1;
		}
		else
		{
			return searchNumberOfNodesOnSpecifiedLevel(BST->left, level, currentLevel + 1) + searchNumberOfNodesOnSpecifiedLevel(BST->right, level, currentLevel + 1);
		}
	}
	else
	{
		return 0;
	}
}
void populateVector(BinarySearchTree* BST, DepositInfo** vector, int level, int currentLevel, int* currentPosition)
{
	if (BST)
	{
		if (currentLevel == level)
		{
			vector[currentPosition[0]] = createDeposit(BST->info->name, BST->info->location, BST->info->numEmployees, BST->info->salaries, BST->info->seniority);
			currentPosition[0]++;
		}
		else
		{
			populateVector(BST->left, vector, level, currentLevel + 1, currentPosition);
			populateVector(BST->right, vector, level, currentLevel + 1, currentPosition);
		}
	}
}
void displayVector(DepositInfo** vector, int* noEL)
{
	if (vector)
	{
		printf("Vector:\n----------------\n");
		for (int i = 0; i < noEL[0]; i++)
		{
			displayDeposit(vector[i]);
		}
	}
}
DepositInfo** roadVector(BinarySearchTree* BST, int* noEl, int key)
{
	noEl[0] = searchNumberOfNodesOnRoad(BST, key);
	if (noEl[0] < 10)
	{
		DepositInfo** vector = (DepositInfo**)malloc(noEl[0] * sizeof(DepositInfo*));
		memset(vector, 0, noEl[0] * sizeof(DepositInfo*));
		int* currentPosition = (int*)malloc(sizeof(int));
		currentPosition[0] = 0;
		populateRoadVector(BST, vector, key, currentPosition);
		return vector;
	}
	else
	{
		return NULL;
	}
}
int searchNumberOfNodesOnRoad(BinarySearchTree* BST, int key)
{
	if (BST)
	{
		if (BST->info->numEmployees == key)
		{
			return 1;
		}
		else
		{
			if (BST->info->numEmployees < key)
			{
				return 1 + searchNumberOfNodesOnRoad(BST->right, key);
			}
			else
			{
				return 1 + searchNumberOfNodesOnRoad(BST->left, key);
			}
		}
	}
	else
	{
		return 10;
	}
}
void populateRoadVector(BinarySearchTree* BST, DepositInfo** vector, int key, int* currentPosition)
{
	vector[currentPosition[0]] = createDeposit(BST->info->name, BST->info->location, BST->info->numEmployees, BST->info->salaries, BST->info->seniority);
	currentPosition[0]++;
	if (BST->info->numEmployees != key)
	{
		if (key < BST->info->numEmployees)
		{
			populateRoadVector(BST->left, vector, key, currentPosition);
		}
		else
		{
			populateRoadVector(BST->right, vector, key, currentPosition);
		}
	}
}
int maximumNumberOfNodesLevel(BinarySearchTree* BST)
{
	int nrOfLevels = searchNumberOfLevels(BST);
	int* vector = (int*)malloc(sizeof(int) * nrOfLevels);
	for (int i = 0; i < nrOfLevels; i++)
	{
		vector[i] = searchNumberOfNodesOnSpecifiedLevel(BST, i + 1, 1);
	}
	int maxim = 0;
	for (int i = 1; i < nrOfLevels; i++)
	{
		if (vector[i] > vector[maxim])
		{
			maxim = i;
		}
	}
	return maxim + 1;
}
void deleteLeavesNodes(BinarySearchTree** BST)
{
	if (*BST)
	{
		if ((*BST)->left == NULL && (*BST)->right == NULL)
		{
			BinarySearchTree* aux = *BST;
			*BST = NULL;
			free(aux->info->name);
			free(aux->info->location);
			free(aux->info);
			free(aux);
		}
		else
		{
			deleteLeavesNodes(&(*BST)->left);
			deleteLeavesNodes(&(*BST)->right);
		}
	}
}
void generalDeallocation(BinarySearchTree** BST, DepositInfo** vector1, DepositInfo** vector2, int* n1, int* n2)
{
	deallocateBST(BST);
	deallocateVector(vector1, n1);
	deallocateVector(vector2, n2);
}
void deallocateBST(BinarySearchTree** BST)
{
	if (*BST)
	{
		deallocateBST(&(*BST)->left);
		deallocateBST(&(*BST)->right);
		BinarySearchTree* aux = *BST;
		*BST = NULL;
		free(aux->info->name);
		free(aux->info->location);
		free(aux->info);
		free(aux);
	}
}
void deallocateVector(DepositInfo** vector, int* n)
{
	for (int i = 0; i < n[0]; i++)
	{
		free(vector[i]->name);
		free(vector[i]->location);
		free(vector[i]);
	}
	free(vector);
	free(n);
}
