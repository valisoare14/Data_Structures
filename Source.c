#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#define V_INIT_SIZE 8

struct Fotografie
{
	unsigned int idFotografie;
	char* oras;
	char* data;
	float rezolutie;
};
typedef struct Fotografie FotografieInfo;

struct BinarySearchTree
{
	FotografieInfo* info;
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
};
typedef struct BinarySearchTree BinarySearchTree;

//memory
FotografieInfo* createFotoGrafie(unsigned int , char* , char* , float );
void insertBST(BinarySearchTree**, FotografieInfo*);
BinarySearchTree* createNode(FotografieInfo* );
void inordine(BinarySearchTree*);
void displayFoto(FotografieInfo*);
int searchForFotosInSameTown(BinarySearchTree*, char*);
void modificareData(BinarySearchTree**, unsigned int, char*);
void dezalocareRadacina(BinarySearchTree**);
FotografieInfo** fromBSTToVector(BinarySearchTree*, float, int*);
int searchNumberOfFoto(BinarySearchTree*, float);
void populateVector(BinarySearchTree*, FotografieInfo**, int*, float);
void printVector(FotografieInfo**, int*);
void displayFotoOnlyAboveSpecifiedDate(BinarySearchTree*, char*);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		unsigned int idFotografie;
		char oras[128];
		char data[128];
		float rezolutie;
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			idFotografie = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(oras, token);
			token = strtok(NULL, delimiter);
			strcpy(data, token);
			token = strtok(NULL, delimiter);
			rezolutie = atof(token);
			FotografieInfo* fotografie = createFotoGrafie(idFotografie, oras, data, rezolutie);
			insertBST(&BST, fotografie);
		}
		//.1
		inordine(BST);

		//.2
		int nrFotografii = searchForFotosInSameTown(BST, "Giurgiu");
		printf("Numar fotografii in orasul Giurgiu : %d\n", nrFotografii);

		//.3
		modificareData(&BST, 23, "2023-07-07");
		printf("Dupa modificare data:\n\n");
		inordine(BST);

		//.4
		printf("Dupa dezalocare radacina:\n\n");
		dezalocareRadacina(&BST);
		inordine(BST);

		//.5
		int* size=(int*)malloc(sizeof(int));
		size[0] = 0;
		FotografieInfo** vector = fromBSTToVector(BST, 120,size);
		printVector(vector, size);

		//.6 Sa se afiseze doar fotografiile executate dupa data 2024-07-05
		printf("Fotografii peste data 2024-07-05:\n\n");
		displayFotoOnlyAboveSpecifiedDate(BST, "2024-07-05");

	}
}
FotografieInfo* createFotoGrafie(unsigned int idFotografie,char* oras,char* data,float rezolutie)
{
	FotografieInfo* foto = (FotografieInfo*)malloc(sizeof(FotografieInfo));
	foto->data = (char*)malloc(strlen(data) + 1);
	foto->oras = (char*)malloc(strlen(oras) + 1);
	strcpy(foto->data, data);
	strcpy(foto->oras, oras);
	foto->idFotografie = idFotografie;
	foto->rezolutie = rezolutie;
	return foto;
}
void insertBST(BinarySearchTree** BST, FotografieInfo* foto)
{
	if (*BST == NULL)
	{
		*BST = createNode(foto);
	}
	else
	{
		if ((*BST)->info->idFotografie > foto->idFotografie)
		{
			insertBST(&(*BST)->left, foto);
		}
		else if ((*BST)->info->idFotografie < foto->idFotografie)
		{
			insertBST(&(*BST)->right, foto);
		}
		else
		{
			FotografieInfo* aux = (*BST)->info;
			(*BST)->info = foto;
			free(aux->oras);
			free(aux->data);
			free(aux);
		}
	}
}
BinarySearchTree* createNode(FotografieInfo* foto)
{
	BinarySearchTree* bst = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	bst->info = foto;
	bst->left = bst->right = NULL;
	return bst;
}
void inordine(BinarySearchTree* BST)
{
	if (BST)
	{
		inordine(BST->left);
		displayFoto(BST->info);
		inordine(BST->right);
	}
}
void displayFoto(FotografieInfo* foto)
{
	printf("%d -> %s -> %s -> %.2f\n", foto->idFotografie, foto->oras, foto->data, foto->rezolutie);
}
int searchForFotosInSameTown(BinarySearchTree* BST, char* oras)
{
	if (BST)
	{
		if (strcmp(BST->info->oras, oras) == 0)
		{
			return 1 + searchForFotosInSameTown(BST->left, oras) + searchForFotosInSameTown(BST->right, oras);
		}
		else
		{
			return searchForFotosInSameTown(BST->left, oras) + searchForFotosInSameTown(BST->right, oras);
		}
	}
	else
	{
		return 0;
	}
}
void modificareData(BinarySearchTree** BST, unsigned int idFotografie, char* nouaData)
{
	if (*BST)
	{
		if ((*BST)->info->idFotografie == idFotografie)
		{
			free((*BST)->info->data);
			(*BST)->info->data = (char*)malloc(strlen(nouaData) + 1);
			strcpy((*BST)->info->data, nouaData);
		}
		else
		{
			modificareData(&(*BST)->left, idFotografie, nouaData);
			modificareData(&(*BST)->right, idFotografie, nouaData);
		}
	}
}
void dezalocareRadacina(BinarySearchTree** BST)
{
	BinarySearchTree* oldRoot = *BST;
	BinarySearchTree* descDreapta = (*BST)->right;
	BinarySearchTree* descDreaptaStanga = (*BST)->right->left;
	if ((*BST)->left != NULL)
	{
		BinarySearchTree* aux = (*BST)->left->right;
		while (aux)
		{
			aux = aux->right;
		}
		aux = descDreapta;
		descDreapta->left = (*BST)->left;
	}
	*BST = descDreapta;
	free(oldRoot->info->data);
	free(oldRoot->info->oras);
	free(oldRoot->info);
	free(oldRoot);
}
FotografieInfo** fromBSTToVector(BinarySearchTree* BST, float rezolutie, int* size)
{
	int n =searchNumberOfFoto(BST, rezolutie);
	FotografieInfo** vector =(FotografieInfo**)malloc(sizeof(FotografieInfo*)*n);
	memset(vector, 0, sizeof(FotografieInfo*) * n);
	populateVector(BST, vector, size, rezolutie);
	return vector;
}
int searchNumberOfFoto(BinarySearchTree* BST, float rezolutie)
{
	if (BST)
	{
		if (BST->info->rezolutie == rezolutie)
		{
			return 1 + searchNumberOfFoto(BST->left, rezolutie) + searchNumberOfFoto(BST->right, rezolutie);
		}
		else
		{
			return searchNumberOfFoto(BST->left, rezolutie) + searchNumberOfFoto(BST->right, rezolutie);
		}
	}
	else
	{
		return 0;
	}
}
void populateVector(BinarySearchTree* BST, FotografieInfo** vector, int* size, float rezolutie)
{
	if (BST)
	{
		if (BST->info->rezolutie == rezolutie)
		{
			vector[size[0]] = createFotoGrafie(BST->info->idFotografie, BST->info->oras, BST->info->data, BST->info->rezolutie);
			size[0]++;
		}
		populateVector(BST->left, vector, size, rezolutie);
		populateVector(BST->right, vector, size, rezolutie);
	}
}
void printVector(FotografieInfo** vector, int* size)
{
	if (vector)
	{
		printf("Vector:\n\n");
		for (int i = 0; i < size[0]; i++)
		{
			displayFoto(vector[i]);
		}
	}
}
void displayFotoOnlyAboveSpecifiedDate(BinarySearchTree* BST, char* prag)
{
	if (BST)
	{
		char delimiter[] = "-\0";
		char* token;
		char buffer1[128];
		strcpy(buffer1, prag);
		//-------------------------------
		token = strtok(buffer1, delimiter);
		int yp = atoi(token);
		token = strtok(NULL, delimiter);
		int mp = atoi(token);
		token = strtok(NULL, delimiter);
		int dp = atoi(token);
		//-------------------------------
		memset(buffer1, 0, 128);
		strcpy(buffer1, BST->info->data);
		token = strtok(buffer1, delimiter);
		int y = atoi(token);
		token = strtok(NULL, delimiter);
		int m = atoi(token);
		token = strtok(NULL, delimiter);
		int d = atoi(token);
		//--------------------------------
		if (y >= yp)
		{
			if (y > yp)
			{
				displayFoto(BST->info);
			}
			else
			{
				if (m >= mp)
				{
					if (m > mp)
					{
						displayFoto(BST->info);
					}
					else
					{
						if (d > dp)
						{
							displayFoto(BST->info);
						}
					}
				}
			}
		}
		displayFotoOnlyAboveSpecifiedDate(BST->left, prag);
		displayFotoOnlyAboveSpecifiedDate(BST->right, prag);
	}
}