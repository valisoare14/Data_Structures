#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

struct Depozit
{
	char* denumire;
	char* locatie;
	int nrAngajati;
	float salarii;
	int vechime;
};
typedef struct Depozit DepozitInfo;

struct BinarySearchTree
{
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
	DepozitInfo* info;
	int bFactor;
};
typedef struct BinarySearchTree BinarySearchTree;

//memory
DepozitInfo* createDepozit(char* , char* , int , float , int );
BinarySearchTree* createNode(DepozitInfo*);
void insertNode(BinarySearchTree**, DepozitInfo*);
void rebalance(BinarySearchTree**);
int getHeight(BinarySearchTree*);
void leftRotation(BinarySearchTree**);
void rightRotation(BinarySearchTree**);
void inordine(BinarySearchTree*);
void displayDepozit(DepozitInfo*);
DepozitInfo** vectorOfDepositsOnSpecifiedLevel(BinarySearchTree*, int*, int);
int searchNumberOfLevels(BinarySearchTree*);
int  searchNumberOfNodesOnSpecifiedLevel(BinarySearchTree* , int , int );
void populateVector(BinarySearchTree* , DepozitInfo** , int , int ,int*);
void displayVector(DepozitInfo** , int* );
DepozitInfo** roadVector(BinarySearchTree* , int* , int );
int searchNumberOfNodesOnRoad(BinarySearchTree*, int);
void populateRoadVector(BinarySearchTree*, DepozitInfo** , int , int* );
void deleteLeavesNodes(BinarySearchTree**);
int maximumNumberOfNodesLevel(BinarySearchTree*);
void dezalocareGenerala(BinarySearchTree** BST, DepozitInfo** , DepozitInfo** , int* , int* );
void dezalocareBST(BinarySearchTree**);
void dezalocareVector(DepozitInfo**, int*);


void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		char denumire[128];
		char locatie[128];
		int nrAngajati;
		float salarii;
		int vechime;
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			strcpy(denumire, token);
			token = strtok(NULL, delimiter);
			strcpy(locatie, token);
			token = strtok(NULL, delimiter);
			nrAngajati = atoi(token);
			token = strtok(NULL, delimiter);
			salarii = atof(token);
			token = strtok(NULL, delimiter);
			vechime = atoi(token);
			DepozitInfo* dep = createDepozit(denumire,locatie,nrAngajati,salarii,vechime);
			insertNode(&BST, dep);
		}
		inordine(BST);
		//.2
		int* noEL = (int*)malloc(sizeof(int));
		noEL[0] = 0;
		DepozitInfo** vector = vectorOfDepositsOnSpecifiedLevel(BST, noEL, 2);
		displayVector(vector, noEL);

		//.3
		int* noEl1 = (int*)malloc(sizeof(int));
		noEl1[0] = 0;
		DepozitInfo** vector1 = roadVector(BST, noEl1, 9);
		displayVector(vector1, noEl1);

		//.4
		int maxLevel = maximumNumberOfNodesLevel(BST);
		printf("Nivelul cu numarul maxim de noduri : %d\n", maxLevel);

		//.5
		deleteLeavesNodes(&BST);
		printf("Dupa stergere frunze:\n");
		inordine(BST);

		//.6
		dezalocareGenerala(&BST, vector, vector1, noEL, noEl1);
		printf("Dupa stergere structuri:\n------------------\n");
		inordine(BST);
		displayVector(vector1, noEl1);
		displayVector(vector, noEL);
	}
}
DepozitInfo* createDepozit(char* denumire, char* locatie, int nrAngajati, float salarii, int vechime)
{
	DepozitInfo* dep = (DepozitInfo*)malloc(sizeof(DepozitInfo));
	dep->denumire = (char*)malloc(strlen(denumire) + 1);
	dep->locatie = (char*)malloc(strlen(locatie) + 1);
	strcpy(dep->denumire, denumire);
	strcpy(dep->locatie, locatie);
	dep->nrAngajati = nrAngajati;
	dep->salarii = salarii;
	dep->vechime = vechime;
	return dep;
}
BinarySearchTree* createNode(DepozitInfo* dep)
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	node->info = dep;
	node->left = node->right = NULL;
	node->bFactor = 0;
	return node;
}
void insertNode(BinarySearchTree** BST, DepozitInfo* dep)
{
	if (*BST == NULL)
	{
		*BST = createNode(dep);
	}
	else
	{
		if ((*BST)->info->nrAngajati > dep->nrAngajati)
		{
			insertNode(&(*BST)->left, dep);
		}
		else if ((*BST)->info->nrAngajati < dep->nrAngajati)
		{
			insertNode(&(*BST)->right, dep);
		}
		else
		{
			DepozitInfo* aux = (*BST)->info;
			(*BST)->info = dep;
			free(aux->denumire);
			free(aux->locatie);
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
		return 1 + max(getHeight(root->right) , getHeight(root->left));
	}
	else
	{
		return 0;
	}
}
void rightRotation(BinarySearchTree** pivot)
{
	BinarySearchTree* descendent = (*pivot)->left;
	BinarySearchTree* aux = descendent->right;
	(*pivot)->left = aux;
	descendent->right = *pivot;
	*pivot = descendent;
}
void leftRotation(BinarySearchTree** pivot)
{
	BinarySearchTree* descendent = (*pivot)->right;
	BinarySearchTree* aux = descendent->left;
	(*pivot)->right = aux;
	descendent->left = *pivot;
	*pivot = descendent;
}
void inordine(BinarySearchTree* BST)
{
	if (BST)
	{
		inordine(BST->left);
		displayDepozit(BST->info);
		inordine(BST->right);
	}
}
void displayDepozit(DepozitInfo* dep)
{
	printf("Nume : %s -> Nr. angajati : %d\n", dep->denumire, dep->nrAngajati);
}
DepozitInfo** vectorOfDepositsOnSpecifiedLevel(BinarySearchTree* BST,int* noEL,int level)
{
	int nrNivele = searchNumberOfLevels(BST);
	if (level <= nrNivele)
	{
		noEL[0] = searchNumberOfNodesOnSpecifiedLevel(BST, level , 1);
		DepozitInfo** vector = (DepozitInfo**)malloc(noEL[0] * sizeof(DepozitInfo*));
		memset(vector, 0, noEL[0] * sizeof(DepozitInfo*));
		int* currentPosition = (int*)malloc(sizeof(int));
		currentPosition[0] = 0;
		populateVector(BST, vector, level,1, currentPosition);
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
int  searchNumberOfNodesOnSpecifiedLevel(BinarySearchTree* BST,int level , int currentLevel)
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
void populateVector(BinarySearchTree* BST,DepozitInfo** vector,int level,int currentLevel,int* currentPosition)
{
	if (BST)
	{
		if (currentLevel == level)
		{
			vector[currentPosition[0]] = createDepozit(BST->info->denumire, BST->info->locatie,BST->info->nrAngajati ,BST->info->salarii, BST->info->vechime);
			currentPosition[0]++;
		}
		else
		{
			populateVector(BST->left, vector, level, currentLevel + 1, currentPosition);
			populateVector(BST->right, vector, level, currentLevel + 1, currentPosition);
		}
	}
}
void displayVector(DepozitInfo** vector,int* noEL)
{
	if (vector)
	{
		printf("Vector:\n----------------\n");
		for (int i = 0; i < noEL[0]; i++)
		{
			displayDepozit(vector[i]);
		}
	}
}
DepozitInfo** roadVector(BinarySearchTree* BST, int* noEl, int key)
{
	noEl[0] = searchNumberOfNodesOnRoad(BST, key);
	if(noEl[0] < 10)
	{
		DepozitInfo** vector = (DepozitInfo**)malloc(noEl[0] * sizeof(DepozitInfo*));
		memset(vector, 0, noEl[0] * sizeof(DepozitInfo*));
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
		if (BST->info->nrAngajati == key)
		{
			return 1;
		}
		else
		{
			if (BST->info->nrAngajati < key)
			{
				return 1+ searchNumberOfNodesOnRoad(BST->right, key);
			}
			else
			{
				return 1+ searchNumberOfNodesOnRoad(BST->left, key);
			}
		}
	}
	else
	{
		return 10;
	}
}
void populateRoadVector(BinarySearchTree* BST,DepozitInfo** vector,int key,int* currentPosition)
{
	vector[currentPosition[0]] = createDepozit(BST->info->denumire, BST->info->locatie, BST->info->nrAngajati, BST->info->salarii, BST->info->vechime);
	currentPosition[0]++;
	if (BST->info->nrAngajati != key)
	{
		if (key < BST->info->nrAngajati)
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
		if ((*BST)->left == NULL&&(*BST)->right ==NULL)
		{
			BinarySearchTree* aux = *BST;
			*BST = NULL;
			free(aux->info->denumire);
			free(aux->info->locatie);
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
void dezalocareGenerala(BinarySearchTree** BST, DepozitInfo** vector1, DepozitInfo** vector2, int* n1, int* n2)
{
	dezalocareBST(BST);
	dezalocareVector(vector1, n1);
	dezalocareVector(vector2, n2);
}
void dezalocareBST(BinarySearchTree** BST)
{
	if (*BST)
	{
		dezalocareBST(&(*BST)->left);
		dezalocareBST(&(*BST)->right);
		BinarySearchTree* aux = *BST;
		*BST = NULL;
		free(aux->info->denumire);
		free(aux->info->locatie);
		free(aux->info);
		free(aux);
	}
}
void dezalocareVector(DepozitInfo** vector, int* n)
{
	for (int i = 0; i < n[0]; i++)
	{
		free(vector[i]->denumire);
		free(vector[i]->locatie);
		free(vector[i]);
	}
	free(vector);
	free(n);
}
