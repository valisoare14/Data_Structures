#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Angajat
{
	char* nume;
	unsigned int codDepartament;
	int varsta;
	float salariulOrar;
};
typedef struct Angajat AngajatInfo;

struct Node
{
	AngajatInfo* info;
	struct Node* next;
};
typedef struct Node ListNode;

struct ListOfList
{
	AngajatInfo* info;
	struct ListOfList* next;
	ListNode* neighbours;
};
typedef struct ListOfList DoubleListNode;

//info
AngajatInfo* createAngajat(char* , unsigned int , int , float );
DoubleListNode* createDoubleListNode(AngajatInfo*);
DoubleListNode* insertNode(DoubleListNode*,AngajatInfo*);
ListNode* createNode(AngajatInfo*);
void displayListOfLists(DoubleListNode*);
void displayAngajat(AngajatInfo*);
AngajatInfo** vectorAngajati(DoubleListNode*,int*,float);
void displayVector(AngajatInfo**, int*);
ListNode* determinareAngajatiDinDepartamentulX(DoubleListNode*, int);
ListNode* getNode(ListNode**);
void displayQueue(ListNode*);
void golireQueue(ListNode**, float);


void main() {
	FILE* pFile = fopen("Data.txt", "r");
	DoubleListNode* listaDeListe = NULL;
	if (pFile)
	{
		char nume[128];
		unsigned int codDepartament;
		int varsta;
		float salariulOrar;
		char linebuffer[128];
		char delimiter[] = ",\n";
		char* token;
		while (fgets(linebuffer, sizeof(linebuffer), pFile))
		{
			token = strtok(linebuffer, delimiter);
			strcpy(nume, token);
			token = strtok(NULL, delimiter);
			codDepartament = atoi(token);
			token = strtok(NULL, delimiter);
			varsta = atoi(token);
			token = strtok(NULL, delimiter);
			salariulOrar = atof(token);
			AngajatInfo* ang = createAngajat(nume,codDepartament,varsta,salariulOrar);
			listaDeListe = insertNode(listaDeListe,ang);
		}
		//.1
		displayListOfLists(listaDeListe);

		//.2
		int* nrAngajati = (int*)malloc(sizeof(int));
		nrAngajati[0] = 0;
		AngajatInfo** angajati = vectorAngajati(listaDeListe, nrAngajati,160.5);
		displayVector(angajati, nrAngajati);

		//.3
		ListNode* coada = determinareAngajatiDinDepartamentulX(listaDeListe , 2);
		//displayQueue(coada);

		//.4
		int* nrAngajati1 = (int*)malloc(sizeof(int));
		nrAngajati1[0] = 0;
		AngajatInfo** angajati1 = vectorAngajatiSalariiMici(coada, nrAngajati1);
		displayVector(angajati1, nrAngajati1);
	}
}
AngajatInfo* createAngajat(char* nume, unsigned int codDepartament, int varsta, float salariulOrar)
{
	AngajatInfo* res = (AngajatInfo*)malloc(sizeof(AngajatInfo));
	res->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(res->nume, nume);
	res->codDepartament = codDepartament;
	res->salariulOrar = salariulOrar;
	res->varsta = varsta;
	return res;
}
DoubleListNode* createDoubleListNode(AngajatInfo* ang)
{
	DoubleListNode* aux = (DoubleListNode*)malloc(sizeof(DoubleListNode));
	aux->info = ang;
	aux->neighbours = NULL;
	aux->next = NULL;
	return aux;
}
DoubleListNode* insertNode(DoubleListNode* listaDeListe,AngajatInfo* ang)
{
	if (listaDeListe == NULL)
	{
		listaDeListe = createDoubleListNode(ang);
		return listaDeListe;
	}
	else
	{
		DoubleListNode* aux = listaDeListe;
		while (listaDeListe != NULL && ang->codDepartament != listaDeListe->info->codDepartament)
		{
			listaDeListe = listaDeListe->next;
		}
		if (listaDeListe == NULL)
		{
			DoubleListNode* lista= createDoubleListNode(ang);
			lista->next = aux;
			aux = lista;
		}
		else
		{
			ListNode* node = createNode(ang);
			node->next = listaDeListe->neighbours;
			listaDeListe->neighbours = node;
		}
		return aux;
	}
	
}
ListNode* createNode(AngajatInfo* ang)
{
	ListNode* aux = (ListNode*)malloc(sizeof(ListNode));
	aux->info = ang;
	aux->next = NULL;
	return aux;
}
void displayListOfLists(DoubleListNode* listaDeListe)
{
	if (listaDeListe)
	{
		DoubleListNode* aux = listaDeListe;
		do {
			displayAngajat(aux->info);
			if (aux->neighbours)
			{
				ListNode* axu = aux->neighbours;
				do
				{
					displayAngajat(axu->info);
					axu = axu->next;
				} while (axu != NULL);
			}
			aux = aux->next;
		} while (aux != NULL);
	}
}
void displayAngajat(AngajatInfo* ang)
{
	if (ang)
	{
		printf("ID: %d->Nume: %s->Varsta: %d->Salariul: %.2f\n",ang->codDepartament,ang->nume,ang->varsta,ang->salariulOrar);
	}
}
AngajatInfo** vectorAngajati(DoubleListNode* listaDeListe,int* nrAngajati , float prag)
{
	DoubleListNode* aux = listaDeListe;
	while (aux)
	{
		if (aux->info->salariulOrar > prag)
		{
			nrAngajati[0]++;
		}
		if (aux->neighbours)
		{
			ListNode* axx = aux->neighbours;
			while (axx)
			{
				if (axx->info->salariulOrar > prag)
				{
					nrAngajati[0]++;
				}
				axx = axx->next;
			}
		}
		aux = aux->next;
	}
	AngajatInfo** vector = (AngajatInfo**)malloc(nrAngajati[0] * sizeof(AngajatInfo));
	memset(vector, 0, nrAngajati[0] * sizeof(AngajatInfo));
	aux = listaDeListe;
	int currentIdx = 0;
	while (aux)
	{
		if (aux->info->salariulOrar > prag)
		{
			vector[currentIdx] = createAngajat(aux->info->nume, aux->info->codDepartament, aux->info->varsta, aux->info->salariulOrar);
			currentIdx++;
		}
		if (aux->neighbours)
		{
			ListNode* axx = aux->neighbours;
			while (axx)
			{
				if (axx->info->salariulOrar > prag)
				{
					vector[currentIdx] = createAngajat(axx->info->nume, axx->info->codDepartament, axx->info->varsta, axx->info->salariulOrar);
					currentIdx++;
				}
				axx = axx->next;
			}
		}
		aux = aux->next;
	}
	return vector;
}
void displayVector(AngajatInfo** angajati, int* nrAngajati)
{
	printf("Vector:\n---------------\n");
	for (int i = 0; i < nrAngajati[0]; i++)
	{
		displayAngajat(angajati[i]);
	}
}
ListNode* determinareAngajatiDinDepartamentulX(DoubleListNode* listaDeListe, int departament)
{
	while (listaDeListe && listaDeListe->info->codDepartament!=departament)
	{
		listaDeListe = listaDeListe->next;
	}
	if (listaDeListe)
	{
		ListNode* coada=createNode(createAngajat(listaDeListe->info->nume, listaDeListe->info->codDepartament, listaDeListe->info->varsta, listaDeListe->info->salariulOrar));
		coada->next = coada;
		while (listaDeListe->neighbours)
		{

			ListNode* node= createNode(createAngajat(listaDeListe->neighbours->info->nume, listaDeListe->neighbours->info->codDepartament, listaDeListe->neighbours->info->varsta, listaDeListe->neighbours->info->salariulOrar));
			node->next = coada->next;
			coada->next = node;
			coada = node;
			listaDeListe->neighbours = listaDeListe->neighbours->next;
		}
		return coada;
	}
	else
	{
		return NULL;
	}
}
void displayQueue(ListNode* coada)
{
	ListNode* node = NULL;
	printf("Coada:\n");
	while ((node = getNode(&coada)) != NULL)
	{
		displayAngajat(node->info);
	}
}
ListNode* getNode(ListNode** coada)
{
	if(*coada)
	{
		if ((*coada) == (*coada)->next)
		{
			ListNode* node = (*coada);
			*coada = NULL;
			return node;
		}
		else
		{
			ListNode* node = (*coada)->next;
			(*coada)->next = (*coada)->next->next;
			return node;
		}
	}
	else
	{
		return NULL;
	}
}
vectorAngajatiSalariiMici(ListNode* coada,int* nrAngajati1)
{
	nrAngajati1[0] = 3;
	AngajatInfo** vector = (AngajatInfo**)malloc(nrAngajati1[0] * sizeof(AngajatInfo*));
	memset(vector, 0, nrAngajati1[0] * sizeof(AngajatInfo*));
	for (int i = 0; i < 3; i++)
	{
		ListNode* minim = coada;
		ListNode* aux = coada->next;
		do
		{
			if (aux->info->salariulOrar < minim->info->salariulOrar)
			{
					minim = aux;
			}
			aux = aux->next;
		} while (aux != coada);
		vector[i] = createAngajat(minim->info->nume, minim->info->codDepartament, minim->info->varsta, minim->info->salariulOrar);
		golireQueue(&coada, minim->info->salariulOrar);
	}
	return vector;
}
void golireQueue(ListNode** coada, float salariul)
{
	if ((*coada)->info->salariulOrar != salariul)
	{
		golireQueue(&(*coada)->next, salariul);
	}
	else
	{
		(*coada)->info->salariulOrar = 1000;
	}
}