#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#define LINE_SIZE 128

struct Rezervare
{
	unsigned int id_rezervare;
	char* denumire_hotel;
	unsigned char numar_camere;
	char* nume_client;
	float suma_de_plata;
};
typedef struct Rezervare RezervareInfo;

struct BST
{
	struct BST* left;
	struct BST* right;
	RezervareInfo* info;
};
typedef struct BST BinarySearchTree;

//usefull info mem
RezervareInfo* createRezervare(unsigned char , char* , unsigned char , char* , float );
void insertNode(BinarySearchTree** , RezervareInfo* );
BinarySearchTree* createNode(RezervareInfo*);
void preordine(BinarySearchTree*);
void displayInfo(RezervareInfo*);
float contravaloare(BinarySearchTree*, char*);
void deleteMin(BinarySearchTree**);
int* NumberOfNodesPerLevel(BinarySearchTree*);
int  searchNumberOfLevels(BinarySearchTree*);
void populateVector(BinarySearchTree* , int* , int );
void displayVector(int*, int);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		unsigned int id_rezervare;
		char denumire_hotel[LINE_SIZE];
		unsigned char numar_camere;
		char nume_client[LINE_SIZE];
		float suma_de_plata;
		char* token;
		char delimiter[] = ",\n";
		char lineBuffer[LINE_SIZE];
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile))
		{
			token = strtok(lineBuffer, delimiter);
			id_rezervare = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(denumire_hotel, token);
			token = strtok(NULL, delimiter);
			numar_camere = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(nume_client, token);
			token = strtok(NULL, delimiter);
			suma_de_plata = atof(token);
			RezervareInfo* rez = createRezervare(id_rezervare,denumire_hotel,numar_camere,nume_client,suma_de_plata);
			insertNode(&BST, rez);
		}
		//afisare preordine
		preordine(BST);

		//contravaloare
		printf("\n\nContravaloarea rezervarilor :%f\n" , contravaloare(BST , " Popescu Razvan "));
	
		//.4
		int* vector = NumberOfNodesPerLevel(BST);
		displayVector(vector, searchNumberOfLevels(BST));
		deleteMin(&BST);
		printf("Dupa stergere minim:\n");
		preordine(BST);

		//.5
		int* vector1 = NumberOfNodesPerLevel(BST);
		displayVector(vector1, searchNumberOfLevels(BST));
	}
}
RezervareInfo* createRezervare(unsigned char id_rezervare, char* denumire_hotel, unsigned char numar_camere, char* nume_client, float suma_de_plata)
{
	RezervareInfo* rez = (RezervareInfo*)malloc(sizeof(RezervareInfo));
	rez->denumire_hotel = (char*)malloc(strlen(denumire_hotel) + 1);
	rez->nume_client = (char*)malloc(strlen(nume_client) + 1);
	strcpy(rez->denumire_hotel, denumire_hotel);
	strcpy(rez->nume_client, nume_client);
	rez->id_rezervare = id_rezervare;
	rez->numar_camere = numar_camere;
	rez->suma_de_plata = suma_de_plata;
	return rez;
}
void insertNode(BinarySearchTree** BST, RezervareInfo* rez)
{
	if (*BST == NULL)
	{
		*BST = createNode(rez);
	}
	else
	{
		if ((*BST)->info->id_rezervare > rez->id_rezervare)
		{
			insertNode(&(*BST)->left, rez);
		}
		else if ((*BST)->info->id_rezervare < rez->id_rezervare)
		{
			insertNode(&(*BST)->right, rez);
		}
		else
		{
			RezervareInfo* aux = (*BST)->info;
			(*BST)->info = rez;
			free(aux->denumire_hotel);
			free(aux->nume_client);
			free(aux);
		}
	}
}
BinarySearchTree* createNode(RezervareInfo* rez)
{
	BinarySearchTree* aux = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	aux->info = rez;
	aux->left = aux->right = NULL;
	return aux;
}
void preordine(BinarySearchTree* BST)
{
	if (BST)
	{
		preordine(BST->left);
		displayInfo(BST->info);
		preordine(BST->right);
	}
}
void displayInfo(RezervareInfo* rez)
{
	printf("ID :%d -> Nume:%s\n", rez->id_rezervare, rez->nume_client);
}
float contravaloare(BinarySearchTree* BST, char* nume)
{
	if (BST)
	{
		if (strcmp(BST->info->nume_client, nume) == 0)
		{
			return BST->info->suma_de_plata + contravaloare(BST->left,nume) + contravaloare(BST->right,nume);
		}
		else
		{
			return contravaloare(BST->left, nume) + contravaloare(BST->right, nume);
		}
	}
	else
	{
		return 0;
	}
}
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
				free(aux->info->denumire_hotel);
				free(aux->info->nume_client);
				free(aux->info);
				free(aux);
			}
			else
			{
				BinarySearchTree* BSTe = *BST;
				free(BSTe->info->denumire_hotel);
				free(BSTe->info->nume_client);
				free(BSTe->info);
				free(BSTe);
				*BST = NULL;
			}
		}
	}
}
int* NumberOfNodesPerLevel(BinarySearchTree* BST)
{
	int numberOfLevels = searchNumberOfLevels(BST);
	int* vector = (int*)malloc(numberOfLevels * sizeof(int));
	memset(vector, 0, numberOfLevels * sizeof(int));
	int currentLevel = 1;
	populateVector(BST, vector , currentLevel);
	return vector;
}
int searchNumberOfLevels(BinarySearchTree* BST){
	if (BST)
	{
		return 1 + max(searchNumberOfLevels(BST->left), searchNumberOfLevels(BST->right));
	}
	else
	{
		return 0;
	}
}
void populateVector(BinarySearchTree* BST,int* vector,int currentLevel)
{
	if (BST)
	{
		vector[currentLevel - 1]++;
		populateVector(BST->left, vector, currentLevel+1);
		populateVector(BST->right, vector, currentLevel+1);
	}
}
void  displayVector(int* vector, int n)
{
	printf("Nivele:\n");
	for (int i = 0; i < n; i++)
	{
		printf("Nivel %d -> noduri: %d\n" , i+1 , vector[i]);
	}
}
