#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#define HT_INITIAL_SIZE 8

struct Garantie
{
	char* denumire;
	unsigned int nr_luni;
	unsigned char esteElectronica;
	char* service;
};
typedef struct Garantie GarantieInfo;

struct Node
{
	struct Node* next;
	GarantieInfo* info;
};
typedef struct Node StructNode;

struct DoubleLinkedList
{
	GarantieInfo* info;
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
};
typedef struct DoubleLinkedList DoubleLinkedList;

struct HashTable
{
	StructNode** buckets;
	int size;
};
typedef struct HashTable HashTable;

//memory
GarantieInfo* createGarantie(char* , unsigned int , unsigned char , char* );
void populateHT(HashTable*, GarantieInfo*);
int hashFunction(char* , int );
StructNode* createNode(GarantieInfo* );
void putNode(StructNode** , StructNode* );
int searchForNumberOfGarantiiOnSpecifiedService(HashTable, char*);
void extindereGarantii(HashTable*, int);
void printHT(HashTable );
void displayGarantie(GarantieInfo* );
DoubleLinkedList* createDoubleLinkedListNode(GarantieInfo*);
DoubleLinkedList* convertHTToDoubleLinkedList(HashTable, int);
DoubleLinkedList* insertNodeIntoDoubleLinkedList(DoubleLinkedList*, DoubleLinkedList*);
void displayDoubleLinkedList(DoubleLinkedList*);
void modificareService(HashTable*, char*, char*);
void moveNode(StructNode**, StructNode*);
void dezlipireDeStructura(StructNode**, StructNode*);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	HashTable HT = { .buckets = NULL , .size = 0 };
	DoubleLinkedList* listaDubla = NULL;
	if (pFile)
	{
		char denumire[128];
		unsigned int nr_luni;
		unsigned char esteElectronica;
		char service[128];
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			strcpy(denumire, token);
			token = strtok(NULL, delimiter);
			nr_luni = atoi(token);
			token = strtok(NULL, delimiter);
			esteElectronica = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(service, token);
			GarantieInfo* garantie = createGarantie(denumire, nr_luni, esteElectronica, service);
			populateHT(&HT, garantie);
		}
		//.2
		int nrGarantii = searchForNumberOfGarantiiOnSpecifiedService(HT, "SecureGuard Insurance");
		printf("Numar garantii : %d\n", nrGarantii);

		//.3
		//printf("Inainte:\n\n");
		//printHT(HT);
		extindereGarantii(&HT, 7);
		//printf("Dupa:\n\n");
		//printHT(HT);

		//.4
		printHT(HT);
		printf("Dupa modificare service:\n\n");
		modificareService(&HT, "TurboSpark Electric Blender", "SecureGuard Insurance");
		printHT(HT);

		//.5
		/*listaDubla = convertHTToDoubleLinkedList(HT, 20);
		displayDoubleLinkedList(listaDubla);
		printHT(HT);*/
	}
}
GarantieInfo* createGarantie(char* denumire,unsigned int nr_luni,unsigned char esteElectronica, char* service)
{
	GarantieInfo* garantie = (GarantieInfo*)malloc(sizeof(GarantieInfo));
	garantie->denumire = (char*)malloc(strlen(denumire) + 1);
	garantie->service = (char*)malloc(strlen(service) + 1);
	strcpy(garantie->denumire, denumire);
	strcpy(garantie->service, service);
	garantie->esteElectronica = esteElectronica;
	garantie->nr_luni = nr_luni;
	return garantie;
}
void populateHT(HashTable* HT, GarantieInfo* garantie)
{
	if (HT->buckets == NULL)
	{
		HT->buckets = (StructNode**)malloc(sizeof(StructNode*) * HT_INITIAL_SIZE);
		memset(HT->buckets, 0, sizeof(StructNode*) * HT_INITIAL_SIZE);
		HT->size = HT_INITIAL_SIZE;
	}
	StructNode* node = createNode(garantie);
	int key = hashFunction(node->info->service, HT->size);
	putNode(&(HT->buckets[key]), node);
}
StructNode* createNode(GarantieInfo* garantie)
{
	StructNode* node = (StructNode*)malloc(sizeof(StructNode));
	node->info = garantie;
	node->next = NULL;
	return node;
}
int hashFunction(char* service, int size)
{
	return service[0] % size;
}
void putNode(StructNode** bucket, StructNode* node)
{
	if (*bucket == NULL)
	{
		node->next = node;
		*bucket = node;
	}
	else
	{
		node->next = (*bucket)->next;
		(*bucket)->next = node;
		*bucket = node;
	}
}
int searchForNumberOfGarantiiOnSpecifiedService(HashTable HT, char* service)
{
	int nrGarantii = 0;
	int key = hashFunction(service, HT.size);
	StructNode* aux = HT.buckets[key];
	do
	{
		if (strcmp(aux->info->service, service)==0)
		{
			nrGarantii++;
		}
		aux = aux->next;
	} while (aux != HT.buckets[key]);
	return nrGarantii;
}
void extindereGarantii(HashTable* HT, int luniAdaos)
{
	for (int i = 0; i < HT->size; i++)
	{
		if (HT->buckets[i])
		{
			StructNode* aux = HT->buckets[i];
			do
			{
				if (aux->info->esteElectronica == 1)
				{
					aux->info->nr_luni += luniAdaos;
				}
				aux = aux->next;
			} while (aux != HT->buckets[i]);
		}
	}
}
void printHT(HashTable HT)
{
	if (HT.buckets)
	{
		printf("HashTable\n\n");
		for (int i = 0; i < HT.size; i++)
		{
			if (HT.buckets[i])
			{
				StructNode* aux = HT.buckets[i];
				do
				{
					displayGarantie(aux->info);
					aux = aux->next;
				} while (aux != HT.buckets[i]);
			}
		}
	}
}
void displayGarantie(GarantieInfo* garantie)
{
	if (garantie)
	{
		printf("%s -> %d -> %d -> %s\n", garantie->denumire, garantie->nr_luni, garantie->esteElectronica, garantie->service);
	}
}
DoubleLinkedList* createDoubleLinkedListNode(GarantieInfo* garantie)
{
	DoubleLinkedList* node = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
	node->info = garantie;
	node->next = node->prev = NULL;
	return node;
}
DoubleLinkedList* convertHTToDoubleLinkedList(HashTable HT, int prag)
{
	DoubleLinkedList* listaDubla = NULL;
	for (int i = 0; i < HT.size; i++)
	{
		if (HT.buckets[i])
		{
			StructNode* aux = HT.buckets[i];
			do
			{
				if (aux->info->nr_luni < prag)
				{
					listaDubla = insertNodeIntoDoubleLinkedList(listaDubla, createDoubleLinkedListNode(createGarantie(aux->info->denumire, aux->info->nr_luni, aux->info->esteElectronica, aux->info->service)));
				}
				aux = aux->next;
			} while (aux != HT.buckets[i]);
		}
	}
	return listaDubla;
}
DoubleLinkedList* insertNodeIntoDoubleLinkedList(DoubleLinkedList* listaDubla, DoubleLinkedList* node)
{
	if (listaDubla == NULL)
	{
		node->next = node->prev = node;
	}
	else
	{
		node->next = listaDubla;
		node->prev = listaDubla->prev;
		listaDubla->prev= node;
		node->prev->next = node;
	}
	return node;
}
void displayDoubleLinkedList(DoubleLinkedList* lista)
{
	if (lista)
	{
		printf("Lista dublu inlantuita:\n\n");
		DoubleLinkedList* aux = lista;
		do
		{
			displayGarantie(aux->info);
			aux = aux->next;
		} while (aux != lista);
	}
}
void modificareService(HashTable* HT, char* denumire, char* serviceNou)
{
	if (HT->buckets)
	{
		for (int i = 0; i < HT->size; i++)
		{
			if (HT->buckets[i]) {
				StructNode* aux = HT->buckets[i];
				int a = 1;
				do
				{
					if ((a = strcmp(aux->info->denumire, denumire)) == 0)
					{
						int hash = hashFunction(serviceNou, HT->size);
						dezlipireDeStructura(&HT->buckets[i], aux);
						//----------------------------------------------------------
						free(aux->info->service);
						aux->info->service = (char*)malloc(strlen(serviceNou) + 1);
						strcpy(aux->info->service, serviceNou);
						//----------------------------------------------------------
						moveNode(&(HT->buckets[hash]), aux);
					}
					aux = aux->next;
				} while (aux != HT->buckets[i] && a != 0);
			}
		}	
	}
}
void moveNode(StructNode** bucket, StructNode* garantie)
{
	if (*bucket == NULL)
	{
		garantie->next = garantie;
		*bucket = garantie;
	}
	else
	{
		garantie->next = (*bucket)->next;
		(*bucket)->next = garantie;
	}
	*bucket = garantie;
}
void dezlipireDeStructura(StructNode** bucket, StructNode* node)
{
	if (*bucket == (*bucket)->next)
	{
		*bucket = NULL;
	}
	else
	{
		StructNode* prev = *bucket;
		do
		{
			prev = prev->next;
		} while (prev->next != *bucket);
		while (strcmp((*bucket)->info->denumire, node->info->denumire) != 0)
		{
			*bucket = (*bucket)->next;
			prev = prev->next;
		}
		StructNode* aux = *bucket;
		*bucket = prev;
		(*bucket)->next = aux->next;
	}
}
