#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#define HT_INITIAL_SIZE 8

// Data structure describing a warranty
struct Warranty
{
	char* name;
	unsigned int months;
	unsigned char isElectronic;
	char* service;
};
typedef struct Warranty WarrantyInfo;

// A node in the singly circular linked list for each bucket
struct Node
{
	struct Node* next;
	WarrantyInfo* info;
};
typedef struct Node StructNode;

// A node in the doubly circular linked list
struct DoubleLinkedList
{
	WarrantyInfo* info;
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
};
typedef struct DoubleLinkedList DoubleLinkedList;

// Hash table structure
struct HashTable
{
	StructNode** buckets;
	int size;
};
typedef struct HashTable HashTable;

// Prototypes
WarrantyInfo* createWarranty(char*, unsigned int, unsigned char, char*);
void populateHT(HashTable*, WarrantyInfo*);
int hashFunction(char*, int);
StructNode* createNode(WarrantyInfo*);
void putNode(StructNode**, StructNode*);
int searchForNumberOfWarrantiesOnService(HashTable, char*);
void extendWarranties(HashTable*, int);
void printHT(HashTable);
void displayWarranty(WarrantyInfo*);
DoubleLinkedList* createDoubleLinkedListNode(WarrantyInfo*);
DoubleLinkedList* convertHTToDoubleLinkedList(HashTable, int);
DoubleLinkedList* insertNodeIntoDoubleLinkedList(DoubleLinkedList*, DoubleLinkedList*);
void displayDoubleLinkedList(DoubleLinkedList*);
void modifyService(HashTable*, char*, char*);
void moveNode(StructNode**, StructNode*);
void detachStructure(StructNode**, StructNode*);

int main()
{
	FILE* pFile = fopen("Data.txt", "r");
	HashTable HT = { .buckets = NULL, .size = 0 };
	DoubleLinkedList* doubleList = NULL;

	if (pFile)
	{
		char name[128];
		unsigned int months;
		unsigned char isElectronic;
		char service[128];
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];

		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			strcpy(name, token);

			token = strtok(NULL, delimiter);
			months = (unsigned int)atoi(token);

			token = strtok(NULL, delimiter);
			isElectronic = (unsigned char)atoi(token);

			token = strtok(NULL, delimiter);
			strcpy(service, token);

			WarrantyInfo* warranty = createWarranty(name, months, isElectronic, service);
			populateHT(&HT, warranty);
		}

		{
			int nrWarranties = searchForNumberOfWarrantiesOnService(HT, "SecureGuard Insurance");
			printf("Number of warranties: %d\n", nrWarranties);
		}

		{
			extendWarranties(&HT, 7);
		}

		{
			printHT(HT);
			printf("After modifying service:\n\n");
			modifyService(&HT, "TurboSpark Electric Blender", "SecureGuard Insurance");
			printHT(HT);
		}

		/*
		doubleList = convertHTToDoubleLinkedList(HT, 20);
		displayDoubleLinkedList(doubleList);
		printHT(HT);
		*/
	}
	return 0;
}

WarrantyInfo* createWarranty(char* name, unsigned int months, unsigned char isElectronic, char* service)
{
	WarrantyInfo* warranty = (WarrantyInfo*)malloc(sizeof(WarrantyInfo));
	warranty->name = (char*)malloc(strlen(name) + 1);
	warranty->service = (char*)malloc(strlen(service) + 1);

	strcpy(warranty->name, name);
	strcpy(warranty->service, service);
	warranty->isElectronic = isElectronic;
	warranty->months = months;
	return warranty;
}

void populateHT(HashTable* HT, WarrantyInfo* warranty)
{
	if (HT->buckets == NULL)
	{
		HT->buckets = (StructNode**)malloc(sizeof(StructNode*) * HT_INITIAL_SIZE);
		memset(HT->buckets, 0, sizeof(StructNode*) * HT_INITIAL_SIZE);
		HT->size = HT_INITIAL_SIZE;
	}
	StructNode* node = createNode(warranty);
	int key = hashFunction(node->info->service, HT->size);
	putNode(&(HT->buckets[key]), node);
}

StructNode* createNode(WarrantyInfo* warranty)
{
	StructNode* node = (StructNode*)malloc(sizeof(StructNode));
	node->info = warranty;
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

int searchForNumberOfWarrantiesOnService(HashTable HT, char* service)
{
	int count = 0;
	int key = hashFunction(service, HT.size);
	StructNode* aux = HT.buckets[key];

	if (aux)
	{
		do
		{
			if (strcmp(aux->info->service, service) == 0)
			{
				count++;
			}
			aux = aux->next;
		} while (aux != HT.buckets[key]);
	}
	return count;
}

void extendWarranties(HashTable* HT, int addedMonths)
{
	for (int i = 0; i < HT->size; i++)
	{
		if (HT->buckets[i])
		{
			StructNode* aux = HT->buckets[i];
			do
			{
				if (aux->info->isElectronic == 1)
				{
					aux->info->months += addedMonths;
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
		printf("Hash Table:\n\n");
		for (int i = 0; i < HT.size; i++)
		{
			if (HT.buckets[i])
			{
				StructNode* aux = HT.buckets[i];
				do
				{
					displayWarranty(aux->info);
					aux = aux->next;
				} while (aux != HT.buckets[i]);
			}
		}
	}
}

void displayWarranty(WarrantyInfo* warranty)
{
	if (warranty)
	{
		printf("%s -> %d -> %d -> %s\n",
		       warranty->name,
		       warranty->months,
		       warranty->isElectronic,
		       warranty->service);
	}
}

DoubleLinkedList* createDoubleLinkedListNode(WarrantyInfo* warranty)
{
	DoubleLinkedList* node = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
	node->info = warranty;
	node->next = node->prev = NULL;
	return node;
}

DoubleLinkedList* convertHTToDoubleLinkedList(HashTable HT, int limit)
{
	DoubleLinkedList* list = NULL;
	for (int i = 0; i < HT.size; i++)
	{
		if (HT.buckets[i])
		{
			StructNode* aux = HT.buckets[i];
			do
			{
				if (aux->info->months < limit)
				{
					WarrantyInfo* copy = createWarranty(
						aux->info->name,
						aux->info->months,
						aux->info->isElectronic,
						aux->info->service
					);
					list = insertNodeIntoDoubleLinkedList(list, createDoubleLinkedListNode(copy));
				}
				aux = aux->next;
			} while (aux != HT.buckets[i]);
		}
	}
	return list;
}

DoubleLinkedList* insertNodeIntoDoubleLinkedList(DoubleLinkedList* list, DoubleLinkedList* node)
{
	if (list == NULL)
	{
		node->next = node->prev = node;
	}
	else
	{
		node->next = list;
		node->prev = list->prev;
		list->prev->next = node;
		list->prev = node;
	}
	return node;
}

void displayDoubleLinkedList(DoubleLinkedList* list)
{
	if (list)
	{
		printf("Double linked list:\n\n");
		DoubleLinkedList* aux = list;
		do
		{
			displayWarranty(aux->info);
			aux = aux->next;
		} while (aux != list);
	}
}

void modifyService(HashTable* HT, char* name, char* newService)
{
	if (HT->buckets)
	{
		for (int i = 0; i < HT->size; i++)
		{
			if (HT->buckets[i])
			{
				StructNode* aux = HT->buckets[i];
				int cmpResult = 1;
				do
				{
					if ((cmpResult = strcmp(aux->info->name, name)) == 0)
					{
						int newHash = hashFunction(newService, HT->size);
						detachStructure(&HT->buckets[i], aux);
						free(aux->info->service);
						aux->info->service = (char*)malloc(strlen(newService) + 1);
						strcpy(aux->info->service, newService);
						moveNode(&(HT->buckets[newHash]), aux);
					}
					aux = aux->next;
				} while (aux != HT->buckets[i] && cmpResult != 0);
			}
		}
	}
}

void moveNode(StructNode** bucket, StructNode* warranty)
{
	if (*bucket == NULL)
	{
		warranty->next = warranty;
		*bucket = warranty;
	}
	else
	{
		warranty->next = (*bucket)->next;
		(*bucket)->next = warranty;
	}
	*bucket = warranty;
}

void detachStructure(StructNode** bucket, StructNode* node)
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

		while (strcmp((*bucket)->info->name, node->info->name) != 0)
		{
			*bucket = (*bucket)->next;
			prev = prev->next;
		}
		StructNode* aux = *bucket;
		*bucket = prev;
		(*bucket)->next = aux->next;
	}
}
