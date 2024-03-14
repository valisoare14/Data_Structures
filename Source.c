#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define INITIAL_SIZE 4

struct Vagon
{
	int nrVagon;
	char* firmaVagon;
	int nrBileteVandute;
	int capacitateVagon;
};
typedef struct Vagon VagonInfo;

struct Node
{
	struct Node* prev;
	struct Node* next;
	VagonInfo* info;
};
typedef struct Node ListNode;

struct Pqueue
{
	VagonInfo** vagoane;
	int qsize;
	int currentIndex;
};
typedef struct Pqueue PrioQueue;

//usefull info memory
VagonInfo* createVagon(int,char*,int,int);
ListNode* createNode(VagonInfo*);
ListNode* insertNodeToHead(ListNode*, ListNode*);
void displayDoubleLinkedListHeadToTail(ListNode*);
void displayDoubleLinkedListTailToHead(ListNode*);
void stergereVagonMinim(ListNode*);
int searchMin(ListNode*);
PrioQueue convertFromListToQueue(ListNode*, PrioQueue);
void enqueue(PrioQueue*, VagonInfo*);
void reHeapUp(PrioQueue*, int);
float gradDeOcupare(VagonInfo*);
void displayQueue(PrioQueue);
PrioQueue changeNumberOfTickets(PrioQueue, int, int);
void reHeapDown(PrioQueue*, int);
VagonInfo* dequeue(PrioQueue*);

void main()
{
	FILE* pfile = fopen("Data.txt", "r");
	ListNode* DoubleLinkedList = NULL;
	PrioQueue queue = { .vagoane = NULL , .qsize = 0 , .currentIndex = 0 };
	if (pfile)
	{
		char* token;
		char delimiter[] = ",\n";
		char lineBuffer[128];
		int nrVagon;
		char firmaVagon[128];
		int nrBileteVandute;
		int capacitateVagon;
		while (fgets(lineBuffer, sizeof(lineBuffer), pfile))
		{
			token = strtok(lineBuffer, delimiter);
			nrVagon = atoi(token);
			token = strtok(NULL, delimiter);
			strcpy(firmaVagon, token);
			token = strtok(NULL, delimiter);
			nrBileteVandute = atoi(token);
			token = strtok(NULL, delimiter);
			capacitateVagon = atoi(token);
			VagonInfo* vagon = createVagon(nrVagon,firmaVagon ,nrBileteVandute,capacitateVagon);
			ListNode* node = createNode(vagon);
			DoubleLinkedList = insertNodeToHead(DoubleLinkedList, node);
		}
		//2.
		displayDoubleLinkedListHeadToTail(DoubleLinkedList);
		displayDoubleLinkedListTailToHead(DoubleLinkedList);

		//3.
		stergereVagonMinim(DoubleLinkedList);
		printf("Dupa stergere minim :\n-----------------------\n");
		displayDoubleLinkedListHeadToTail(DoubleLinkedList);
		displayDoubleLinkedListTailToHead(DoubleLinkedList);

		//4.
		queue=convertFromListToQueue(DoubleLinkedList, queue);
		displayQueue(queue);

		//.5
		queue=changeNumberOfTickets(queue, 47, 180);
		displayQueue(queue);

		//dequeue
		VagonInfo* vagon = dequeue(&queue);
		printf("Vagon extras :%d -> %s -> %d -> %d\n", vagon->nrVagon, vagon->firmaVagon, vagon->nrBileteVandute, vagon->capacitateVagon);
		displayQueue(queue);
	}
}
VagonInfo* createVagon(int nrVagon, char* firmaVagon, int nrBileteVandute, int capacitateVagon)
{
	VagonInfo* vagon = (VagonInfo*)malloc(sizeof(VagonInfo));
	vagon->firmaVagon = (char*)malloc(strlen(firmaVagon) + 1);
	vagon->capacitateVagon = capacitateVagon;
	vagon->nrVagon = nrVagon;
	vagon->nrBileteVandute = nrBileteVandute;
	strcpy(vagon->firmaVagon, firmaVagon);
	return vagon;
}
ListNode* createNode(VagonInfo* vagon)
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->info = vagon;
	node->next = node->prev = NULL;
	return node;
}
ListNode* insertNodeToHead(ListNode* DoubleLinkedList, ListNode* node)
{
	if (DoubleLinkedList)
	{
		node->next = DoubleLinkedList;
		node->prev = DoubleLinkedList->prev;
		DoubleLinkedList->prev->next = node;
		DoubleLinkedList->prev = node;
	}
	else
	{
		node->next = node->prev = node;
	}
	return node;
}
void displayDoubleLinkedListHeadToTail(ListNode* DoubleLinkedList)
{
	ListNode* aux = DoubleLinkedList;
	printf("Afisare Head to Tail:\n\n");
	do
	{
		printf("%d -> %s -> %d -> %d\n" , aux->info->nrVagon , aux->info->firmaVagon , aux->info->nrBileteVandute , aux->info->capacitateVagon);
		aux = aux->next;
	} while (aux != DoubleLinkedList);
}
void displayDoubleLinkedListTailToHead(ListNode* DoubleLinkedList)
{
	ListNode* aux = DoubleLinkedList;
	printf("Afisare Tail to Head:\n\n");
	do
	{
		printf("%d -> %s -> %d -> %d\n", aux->prev->info->nrVagon, aux->prev->info->firmaVagon, aux->prev->info->nrBileteVandute, aux->prev->info->capacitateVagon);
		aux = aux->prev;
	} while (aux != DoubleLinkedList);
}
int searchMin(ListNode* DoubleLinkedList)
{
	int minim = DoubleLinkedList->info->nrBileteVandute;
	ListNode* aux = DoubleLinkedList->next;
	do
	{
		if (aux->info->nrBileteVandute < minim)
		{
			minim = aux->info->nrBileteVandute;
		}
		aux = aux->next;
	} while (aux != DoubleLinkedList);
	return minim;
}
void stergereVagonMinim(ListNode* DoubleLinkedList)
{
	if (DoubleLinkedList)
	{
		int nrMinim = searchMin(DoubleLinkedList);
		ListNode* aux = DoubleLinkedList;
		do
		{
			ListNode* auxa = aux;
			aux = aux->next;
			if (auxa->info->nrBileteVandute == nrMinim)
			{
				auxa->prev->next = auxa->next;
				auxa->next->prev = auxa->prev;
				free(auxa->info->firmaVagon);
				free(auxa->info);
				free(auxa);
			}
		} while (aux != DoubleLinkedList);
	}
}
PrioQueue convertFromListToQueue(ListNode* DoubleLinkedList, PrioQueue queue)
{
	if (DoubleLinkedList)
	{
		ListNode* aux = DoubleLinkedList;
		do
		{
			enqueue(&queue, aux->info);
			aux = aux->next;
		}while(aux!=DoubleLinkedList);
	}
	return queue;
}
void enqueue(PrioQueue* queue, VagonInfo* vagon)
{
	if (queue->vagoane == NULL)
	{
		queue->vagoane = (VagonInfo**)malloc(sizeof(VagonInfo*) * INITIAL_SIZE);
		memset(queue->vagoane, 0, sizeof(VagonInfo*) * INITIAL_SIZE);
		queue->qsize = INITIAL_SIZE;
	}
	queue->vagoane[queue->currentIndex] = createVagon(vagon->nrVagon , vagon->firmaVagon , vagon->nrBileteVandute , vagon->capacitateVagon);
	reHeapUp(queue, queue->currentIndex);
	queue->currentIndex++;
}
void reHeapUp(PrioQueue* queue, int curentIndex)
{
	if (curentIndex > 0)
	{
		int parentIndex = curentIndex - 1;
		if (queue->vagoane[parentIndex]->nrBileteVandute> queue->vagoane[curentIndex]->nrBileteVandute)
		{
			VagonInfo* aux = queue->vagoane[parentIndex];
			queue->vagoane[parentIndex] = queue->vagoane[curentIndex];
			queue->vagoane[curentIndex] = aux;
			reHeapUp(queue, parentIndex);
		}
	}
}
float gradDeOcupare(VagonInfo* vagon)
{
	float a = vagon->nrBileteVandute / vagon->capacitateVagon;
	return a;
}
void displayQueue(PrioQueue queue)
{
	printf("Coada de prioritati:\n");
	for (int i = 0; i < queue.currentIndex; i++)
	{
		printf("%d -> %s -> %d -> %d\n", queue.vagoane[i]->nrVagon, queue.vagoane[i]->firmaVagon, queue.vagoane[i]->nrBileteVandute, queue.vagoane[i]->capacitateVagon);
	}
}
PrioQueue changeNumberOfTickets(PrioQueue queue, int numarVagon, int nrNouBilete)
{
	for (int i = 0; i < queue.qsize; i++)
	{
		if (queue.vagoane[i]->nrVagon == numarVagon)
		{
			int nrVechiBilete = queue.vagoane[i]->nrBileteVandute;
			queue.vagoane[i]->nrBileteVandute = nrNouBilete;
			if (nrNouBilete < nrVechiBilete)
			{
				reHeapUp(&queue, i);
			}
			else
			{
				reHeapDown(&queue, i);
			}
		}
	}
	return queue;
}
void reHeapDown(PrioQueue* queue, int parentIndex)
{
	if (parentIndex < queue->currentIndex-1)
	{
		int curentIndex = parentIndex + 1;
		if (queue->vagoane[parentIndex]->nrBileteVandute > queue->vagoane[curentIndex]->nrBileteVandute)
		{
			VagonInfo* aux = queue->vagoane[parentIndex];
			queue->vagoane[parentIndex] = queue->vagoane[curentIndex];
			queue->vagoane[curentIndex] = aux;
			reHeapDown(queue, curentIndex);
		}
	}
}
VagonInfo* dequeue(PrioQueue* queue)
{
	if (queue->vagoane[0] != NULL)
	{
		VagonInfo* result = queue->vagoane[0];
		queue->vagoane[0] = queue->vagoane[queue->currentIndex - 1];
		queue->vagoane[queue->currentIndex - 1] = NULL;
		queue->currentIndex--;
		reHeapDown(queue, 0);
		return result;
	}
	else
	{
		return NULL;
	}
}

