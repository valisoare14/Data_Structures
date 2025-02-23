#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#define INITIAL_SIZE 4

struct Wagon
{
	int wagonNumber;
	char* wagonCompany;
	int soldTickets;
	int wagonCapacity;
};
typedef struct Wagon WagonInfo;

struct Node
{
	struct Node* prev;
	struct Node* next;
	WagonInfo* info;
};
typedef struct Node ListNode;

struct Pqueue
{
	WagonInfo** wagons;
	int qsize;
	int currentIndex;
};
typedef struct Pqueue PrioQueue;

// Function prototypes
WagonInfo* createWagon(int, char*, int, int);
ListNode* createNode(WagonInfo*);
ListNode* insertNodeToHead(ListNode*, ListNode*);
void displayDoubleLinkedListHeadToTail(ListNode*);
void displayDoubleLinkedListTailToHead(ListNode*);
void deleteMinWagon(ListNode*);
int searchMinTickets(ListNode*);
PrioQueue listToPriorityQueue(ListNode*, PrioQueue);
void enqueue(PrioQueue*, WagonInfo*);
void reheapUp(PrioQueue*, int);
float occupancyRate(WagonInfo*);
void displayQueue(PrioQueue);
PrioQueue changeNumberOfTickets(PrioQueue, int, int);
void reheapDown(PrioQueue*, int);
WagonInfo* dequeue(PrioQueue*);

void main()
{
	FILE* pfile = fopen("Data.txt", "r");
	ListNode* doubleLinkedList = NULL;
	PrioQueue queue = { .wagons = NULL, .qsize = 0, .currentIndex = 0 };

	if (pfile)
	{
		char* token;
		char delimiter[] = ",\n";
		char lineBuffer[128];
		int wagonNumber;
		char wagonCompany[128];
		int soldTickets;
		int wagonCapacity;

		while (fgets(lineBuffer, sizeof(lineBuffer), pfile))
		{
			token = strtok(lineBuffer, delimiter);
			wagonNumber = atoi(token);

			token = strtok(NULL, delimiter);
			strcpy(wagonCompany, token);

			token = strtok(NULL, delimiter);
			soldTickets = atoi(token);

			token = strtok(NULL, delimiter);
			wagonCapacity = atoi(token);

			WagonInfo* wagon = createWagon(wagonNumber, wagonCompany, soldTickets, wagonCapacity);
			ListNode* node = createNode(wagon);
			doubleLinkedList = insertNodeToHead(doubleLinkedList, node);
		}

		// 2. Display the double linked list (head-to-tail and tail-to-head)
		displayDoubleLinkedListHeadToTail(doubleLinkedList);
		displayDoubleLinkedListTailToHead(doubleLinkedList);

		// 3. Delete the wagon with the minimum number of sold tickets
		deleteMinWagon(doubleLinkedList);
		printf("After deleting the min:\n-----------------------\n");
		displayDoubleLinkedListHeadToTail(doubleLinkedList);
		displayDoubleLinkedListTailToHead(doubleLinkedList);

		// 4. Convert from double linked list to a priority queue
		queue = listToPriorityQueue(doubleLinkedList, queue);
		displayQueue(queue);

		// 5. Modify the number of tickets for wagonNumber=47
		queue = changeNumberOfTickets(queue, 47, 180);
		displayQueue(queue);

		// Dequeue an element (pop from priority queue)
		WagonInfo* wagon = dequeue(&queue);
		printf("Extracted wagon: %d -> %s -> %d -> %d\n",
		       wagon->wagonNumber, wagon->wagonCompany,
		       wagon->soldTickets, wagon->wagonCapacity);
		displayQueue(queue);
	}
}

WagonInfo* createWagon(int wagonNumber, char* wagonCompany, int soldTickets, int wagonCapacity)
{
	WagonInfo* w = (WagonInfo*)malloc(sizeof(WagonInfo));
	w->wagonCompany = (char*)malloc(strlen(wagonCompany) + 1);
	w->wagonNumber = wagonNumber;
	w->soldTickets = soldTickets;
	w->wagonCapacity = wagonCapacity;
	strcpy(w->wagonCompany, wagonCompany);
	return w;
}

ListNode* createNode(WagonInfo* wagon)
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->info = wagon;
	node->next = node->prev = NULL;
	return node;
}

ListNode* insertNodeToHead(ListNode* doubleLinkedList, ListNode* node)
{
	if (doubleLinkedList)
	{
		node->next = doubleLinkedList;
		node->prev = doubleLinkedList->prev;
		doubleLinkedList->prev->next = node;
		doubleLinkedList->prev = node;
	}
	else
	{
		node->next = node->prev = node;
	}
	return node;
}

void displayDoubleLinkedListHeadToTail(ListNode* doubleLinkedList)
{
	ListNode* aux = doubleLinkedList;
	printf("Display head-to-tail:\n\n");
	if (!aux) return;

	do
	{
		printf("%d -> %s -> %d -> %d\n",
		       aux->info->wagonNumber,
		       aux->info->wagonCompany,
		       aux->info->soldTickets,
		       aux->info->wagonCapacity);
		aux = aux->next;
	} while (aux != doubleLinkedList);
}

void displayDoubleLinkedListTailToHead(ListNode* doubleLinkedList)
{
	ListNode* aux = doubleLinkedList;
	printf("Display tail-to-head:\n\n");
	if (!aux) return;

	do
	{
		printf("%d -> %s -> %d -> %d\n",
		       aux->prev->info->wagonNumber,
		       aux->prev->info->wagonCompany,
		       aux->prev->info->soldTickets,
		       aux->prev->info->wagonCapacity);
		aux = aux->prev;
	} while (aux != doubleLinkedList);
}

int searchMinTickets(ListNode* doubleLinkedList)
{
	int minValue = doubleLinkedList->info->soldTickets;
	ListNode* aux = doubleLinkedList->next;

	do
	{
		if (aux->info->soldTickets < minValue)
		{
			minValue = aux->info->soldTickets;
		}
		aux = aux->next;
	} while (aux != doubleLinkedList);

	return minValue;
}

void deleteMinWagon(ListNode* doubleLinkedList)
{
	if (doubleLinkedList)
	{
		int minTickets = searchMinTickets(doubleLinkedList);
		ListNode* aux = doubleLinkedList;

		do
		{
			ListNode* current = aux;
			aux = aux->next;
			if (current->info->soldTickets == minTickets)
			{
				current->prev->next = current->next;
				current->next->prev = current->prev;
				free(current->info->wagonCompany);
				free(current->info);
				free(current);
			}
		} while (aux != doubleLinkedList);
	}
}

PrioQueue listToPriorityQueue(ListNode* doubleLinkedList, PrioQueue queue)
{
	if (doubleLinkedList)
	{
		ListNode* aux = doubleLinkedList;
		do
		{
			enqueue(&queue, aux->info);
			aux = aux->next;
		} while (aux != doubleLinkedList);
	}
	return queue;
}

void enqueue(PrioQueue* queue, WagonInfo* wagon)
{
	if (queue->wagons == NULL)
	{
		queue->wagons = (WagonInfo**)malloc(sizeof(WagonInfo*) * INITIAL_SIZE);
		memset(queue->wagons, 0, sizeof(WagonInfo*) * INITIAL_SIZE);
		queue->qsize = INITIAL_SIZE;
	}
	queue->wagons[queue->currentIndex] = createWagon(wagon->wagonNumber,
	                                                  wagon->wagonCompany,
	                                                  wagon->soldTickets,
	                                                  wagon->wagonCapacity);
	reheapUp(queue, queue->currentIndex);
	queue->currentIndex++;
}

void reheapUp(PrioQueue* queue, int currentIndex)
{
	if (currentIndex > 0)
	{
		int parentIndex = currentIndex - 1;
		if (queue->wagons[parentIndex]->soldTickets >
		    queue->wagons[currentIndex]->soldTickets)
		{
			WagonInfo* aux = queue->wagons[parentIndex];
			queue->wagons[parentIndex] = queue->wagons[currentIndex];
			queue->wagons[currentIndex] = aux;
			reheapUp(queue, parentIndex);
		}
	}
}

float occupancyRate(WagonInfo* wagon)
{
	return (float)wagon->soldTickets / wagon->wagonCapacity;
}

void displayQueue(PrioQueue queue)
{
	printf("Priority queue:\n");
	for (int i = 0; i < queue.currentIndex; i++)
	{
		printf("%d -> %s -> %d -> %d\n",
		       queue.wagons[i]->wagonNumber,
		       queue.wagons[i]->wagonCompany,
		       queue.wagons[i]->soldTickets,
		       queue.wagons[i]->wagonCapacity);
	}
}

PrioQueue changeNumberOfTickets(PrioQueue queue, int wagonNumber, int newTickets)
{
	for (int i = 0; i < queue.qsize; i++)
	{
		if (queue.wagons[i] != NULL &&
		    queue.wagons[i]->wagonNumber == wagonNumber)
		{
			int oldTickets = queue.wagons[i]->soldTickets;
			queue.wagons[i]->soldTickets = newTickets;
			if (newTickets < oldTickets)
			{
				reheapUp(&queue, i);
			}
			else
			{
				reheapDown(&queue, i);
			}
		}
	}
	return queue;
}

void reheapDown(PrioQueue* queue, int parentIndex)
{
	if (parentIndex < queue->currentIndex - 1)
	{
		int childIndex = parentIndex + 1;
		if (queue->wagons[parentIndex]->soldTickets >
		    queue->wagons[childIndex]->soldTickets)
		{
			WagonInfo* aux = queue->wagons[parentIndex];
			queue->wagons[parentIndex] = queue->wagons[childIndex];
			queue->wagons[childIndex] = aux;
			reheapDown(queue, childIndex);
		}
	}
}

WagonInfo* dequeue(PrioQueue* queue)
{
	if (queue->wagons[0] != NULL)
	{
		WagonInfo* result = queue->wagons[0];
		queue->wagons[0] = queue->wagons[queue->currentIndex - 1];
		queue->wagons[queue->currentIndex - 1] = NULL;
		queue->currentIndex--;
		reheapDown(queue, 0);
		return result;
	}
	else
	{
		return NULL;
	}
}
