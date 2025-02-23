#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define HASHTABLE_INIT_SIZE 5
#define LINE_SIZE 128

// Structure describing a project
struct Project
{
	unsigned int code;        // was cod
	char* title;              // was titlu
	char* beneficiary;        // was beneficiar
	unsigned char executants; // was numar_executanti
	float budget;             // was buget
};
typedef struct Project ProjectInfo;

// A node in the singly circular linked list for each bucket of the hash table
struct Node
{
	ProjectInfo* info;
	struct Node* next;
};
typedef struct Node HashNode;

// Hash table structure
struct HashTable
{
	HashNode** buckets;
	int size;
};
typedef struct HashTable HashTable;

// Function prototypes
ProjectInfo* createProject(unsigned int, char*, char*, unsigned char, float);
void insertIntoHashTable(HashTable*, ProjectInfo*);
HashNode* createNode(ProjectInfo*);
int hashFunction(char*, int);
void putNode(HashNode**, HashNode*);
void displayHashTable(HashTable);
void displayProject(ProjectInfo*);
float totalBudget(HashTable, char*);
HashNode* fromHashTableToList(HashTable, float);

// Main
int main()
{
	FILE* pFile = fopen("Data.txt", "r");
	HashTable HT;
	HT.buckets = NULL;
	HT.size = 0;

	if (pFile)
	{
		unsigned int code;
		char title[LINE_SIZE];
		char beneficiary[LINE_SIZE];
		unsigned char executants;
		float budget;

		char* token;
		char delimiter[] = ",\n";
		char buffer[LINE_SIZE];

		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			code = (unsigned int)atoi(token);

			token = strtok(NULL, delimiter);
			strcpy(title, token);

			token = strtok(NULL, delimiter);
			strcpy(beneficiary, token);

			token = strtok(NULL, delimiter);
			executants = (unsigned char)atoi(token);

			token = strtok(NULL, delimiter);
			budget = (float)atof(token);

			ProjectInfo* proj = createProject(code, title, beneficiary, executants, budget);
			insertIntoHashTable(&HT, proj);
		}

		fclose(pFile);

		// 2) Display the hash table
		displayHashTable(HT);

		// 3) Compute total budget for a given beneficiary
		float total = totalBudget(HT, "ROMAC");
		printf("Total budget for beneficiary 'ROMAC': %.2f\n", total);

		// 5) Extract a list of projects with budget > 20000
		HashNode* list = fromHashTableToList(HT, 20000.0f);
		printf("\nProjects with budget > 20000:\n");
		while (list)
		{
			displayProject(list->info);
			list = list->next;
		}
	}
	else
	{
		printf("Failed to open file.\n");
	}

	return 0;
}

// Creates a project
ProjectInfo* createProject(unsigned int code,
                           char* title,
                           char* beneficiary,
                           unsigned char executants,
                           float budget)
{
	ProjectInfo* p = (ProjectInfo*)malloc(sizeof(ProjectInfo));
	p->title = (char*)malloc(strlen(title) + 1);
	p->beneficiary = (char*)malloc(strlen(beneficiary) + 1);

	strcpy(p->title, title);
	strcpy(p->beneficiary, beneficiary);

	p->budget = budget;
	p->code = code;
	p->executants = executants;
	return p;
}

// Inserts a project into the hash table
void insertIntoHashTable(HashTable* HT, ProjectInfo* proj)
{
	if (HT->buckets == NULL)
	{
		HT->buckets = (HashNode**)malloc(sizeof(HashNode*) * HASHTABLE_INIT_SIZE);
		memset(HT->buckets, 0, sizeof(HashNode*) * HASHTABLE_INIT_SIZE);
		HT->size = HASHTABLE_INIT_SIZE;
	}

	HashNode* node = createNode(proj);
	int key = hashFunction(proj->beneficiary, HT->size);
	putNode(&(HT->buckets[key]), node);
}

// Creates a new hash node
HashNode* createNode(ProjectInfo* proj)
{
	HashNode* node = (HashNode*)malloc(sizeof(HashNode));
	node->info = proj;
	node->next = NULL;
	return node;
}

// Simple hash function based on first character of the beneficiary
int hashFunction(char* beneficiary, int size)
{
	return beneficiary[0] % size;
}

// Inserts a node into the circular list at the given bucket
void putNode(HashNode** bucket, HashNode* node)
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

// Displays the entire hash table
void displayHashTable(HashTable HT)
{
	if (HT.buckets)
	{
		printf("\nHash Table:\n\n");
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

// Displays a single project
void displayProject(ProjectInfo* proj)
{
	if (proj)
	{
		printf("Project code: %d | Title: %s | Beneficiary: %s | Budget: %.2f\n",
		       proj->code, proj->title, proj->beneficiary, proj->budget);
	}
}

// Returns the total budget for a given beneficiary
float totalBudget(HashTable HT, char* beneficiary)
{
	float total = 0.0f;
	int key = hashFunction(beneficiary, HT.size);

	HashNode* bucket = HT.buckets[key];
	if (bucket)
	{
		HashNode* aux = bucket;
		do
		{
			if (strcmp(aux->info->beneficiary, beneficiary) == 0)
			{
				total += aux->info->budget;
			}
			aux = aux->next;
		} while (aux != bucket);
	}
	return total;
}

// Extracts projects from the hash table into a linear list if the budget is > given threshold
HashNode* fromHashTableToList(HashTable HT, float budget)
{
	HashNode* list = NULL;

	for (int i = 0; i < HT.size; i++)
	{
		if (HT.buckets[i])
		{
			HashNode* aux = HT.buckets[i];
			do
			{
				if (aux->info->budget > budget)
				{
					// Create a copy of the project for the list
					ProjectInfo* copy = createProject(
					    aux->info->code,
					    aux->info->title,
					    aux->info->beneficiary,
					    aux->info->executants,
					    aux->info->budget);

					// Insert at the head of the linear list
					HashNode* newNode = createNode(copy);
					newNode->next = list;
					list = newNode;
				}
				aux = aux->next;
			} while (aux != HT.buckets[i]);
		}
	}
	return list;
}
