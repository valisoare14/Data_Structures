#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define HASHTABLE_INIT_SIZE 5
#define LINE_SIZE 128

struct Proiect
{
	unsigned int cod;
	char* titlu;
	char* beneficiar;
	unsigned char numar_executanti;
	float buget;
};
typedef struct Proiect ProiectInfo;

struct Node
{
	ProiectInfo* info;
	struct Node* next;
};
typedef struct Node HashNode;

struct HashTable
{
	HashNode** buckets;
	int size;
};
typedef struct HashTable HashTable;

//usefull info
ProiectInfo* createProject(unsigned int , char* , char* , unsigned char , float );
void insertIntoHashTable(HashTable*, ProiectInfo*);
HashNode* createNode(ProiectInfo*);
int hashcode(char*, int );
void putNode(HashNode** , HashNode* );
void displayHashTable(HashTable);
void displayProject(ProiectInfo*);
float bugetTotal(HashTable , char* );
HashNode* fromHTtoList(HashTable, float );


void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	HashTable HT = { .buckets = NULL , .size = 0 };
	if (pFile)
	{
		unsigned int cod;
		char titlu[LINE_SIZE];
		char beneficiar[LINE_SIZE];
		unsigned char numar_executanti;
		float buget;
		char* token;
		char delimiter[] =",\n";
		char buffer[LINE_SIZE];
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			cod = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(titlu, token);
			token = strtok(NULL, delimiter);
			strcpy(beneficiar, token);
			token = strtok(NULL, delimiter);
			numar_executanti = atoi(token);
			token = strtok(NULL, delimiter);
			buget = atof(token);
			ProiectInfo* proj = createProject(cod,titlu,beneficiar,numar_executanti,buget);
			insertIntoHashTable(&HT, proj);
		}
		//.2
		displayHashTable(HT);

		//.3
		float bugeT = bugetTotal(HT, "ROMAC");
		printf("Buget total : %.2f\n", bugeT);

		//.4
		
		//.5
		HashNode* list = fromHTtoList(HT, 20000);
		while (list)
		{
			displayProject(list->info);
			list = list->next;
		}
	}
}
ProiectInfo* createProject(unsigned int cod,char* titlu,char* beneficiar,unsigned char numar_executanti,float buget)
{
	ProiectInfo* aux = (ProiectInfo*)malloc(sizeof(ProiectInfo));
	aux->beneficiar = (char*)malloc(strlen(beneficiar) + 1);
	aux->titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(aux->beneficiar, beneficiar);
	strcpy(aux->titlu, titlu);
	aux->buget = buget;
	aux->cod = cod;
	aux->numar_executanti = numar_executanti;
	return aux;
}
void insertIntoHashTable(HashTable* HT, ProiectInfo* proj)
{
	if (HT->buckets == NULL)
	{
		HT->buckets = (HashNode**)malloc(sizeof(HashNode*) * HASHTABLE_INIT_SIZE);
		memset(HT->buckets, 0, sizeof(HashNode*) * HASHTABLE_INIT_SIZE);
		HT->size = HASHTABLE_INIT_SIZE;
	}
	HashNode* node = createNode(proj);
	int hashCode = hashcode(node->info->beneficiar, HT->size);
	putNode(&(HT->buckets[hashCode]), node);
}
HashNode* createNode (ProiectInfo* proj)
{
	HashNode* node = (HashNode*)malloc(sizeof(HashNode));
	node->info = proj;
	node->next = NULL;
	return node;
}
int hashcode(char* beneficiar, int size)
{
	return beneficiar[0] % size;
}
void putNode(HashNode** list, HashNode* node)
{
	if (*list == NULL)
	{
		node->next = node;
		*list = node;
	}
	else
	{
		node->next = (*list)->next;
		(*list)->next = node;
		*list = node;
	}
}
void displayHashTable(HashTable HT)
{
	if (HT.buckets)
	{
		for (int i = 0; i < HT.size; i++)
		{
			if (HT.buckets[i])
			{
				HashNode* aux = HT.buckets[i];
				do
				{
					displayProject(aux->info);
					aux = aux->next;
				} while (aux != HT.buckets[i]);
			}
		}
	}
}
void displayProject(ProiectInfo* proj)
{
	if (proj)
	{
		printf("Cod proiect:%d -> Beneficiar: %s\n" , proj->cod,proj->beneficiar);
	}
}
float bugetTotal(HashTable HT, char* beneficiar)
{
	int hashCode = hashcode(beneficiar, HT.size);
	float bugetTotal = 0;
	if (HT.buckets[hashCode])
	{
		HashNode* aux = HT.buckets[hashCode];
		do
		{
			if (strcmp(aux->info->beneficiar, beneficiar) == 0)
			{
				bugetTotal += aux->info->buget;
			}
			aux = aux->next;
		} while (aux != HT.buckets[hashCode]);
	}
	return bugetTotal;
}
HashNode* fromHTtoList(HashTable HT, float buget)
{
	HashNode* list = NULL;
	for (int i = 0; i < HT.size; i++)
	{
		if (HT.buckets[i])
		{
			HashNode* aux = HT.buckets[i];
			do
			{
				if (aux->info->buget > buget)
				{
					if (list == NULL)
					{
						list = createNode(createProject(aux->info->cod, aux->info->titlu, aux->info->beneficiar, aux->info->numar_executanti, aux->info->buget));
					}
					else
					{
						HashNode* node= createNode(createProject(aux->info->cod, aux->info->titlu, aux->info->beneficiar, aux->info->numar_executanti, aux->info->buget));
						node->next = list;
						list = node;
					}
				}
				aux = aux->next;
			} while (aux != HT.buckets[i]);
		}
	}
	return list;
}
