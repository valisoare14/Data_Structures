#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

////////////////////////////////////////////////////////////////////////////////
// Structures
////////////////////////////////////////////////////////////////////////////////

struct Employee
{
	char* name;
	unsigned int departmentCode;
	int age;
	float hourlySalary;
};
typedef struct Employee EmployeeInfo;

struct Node
{
	EmployeeInfo* info;
	struct Node* next;
};
typedef struct Node ListNode;

struct ListOfList
{
	EmployeeInfo* info;
	struct ListOfList* next;
	ListNode* neighbours;
};
typedef struct ListOfList DoubleListNode;

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////

EmployeeInfo* createEmployee(char*, unsigned int, int, float);
DoubleListNode* createDoubleListNode(EmployeeInfo*);
DoubleListNode* insertNode(DoubleListNode*, EmployeeInfo*);
ListNode* createNode(EmployeeInfo*);
void displayListOfLists(DoubleListNode*);
void displayEmployee(EmployeeInfo*);
EmployeeInfo** employeesAboveSalary(DoubleListNode*, int*, float);
void displayArray(EmployeeInfo**, int*);
ListNode* buildQueueFromDepartment(DoubleListNode*, int);
void displayQueue(ListNode*);
ListNode* getNodeFromQueue(ListNode**);
EmployeeInfo** smallestSalaries(ListNode*, int*);
void clearQueueBySalary(ListNode**, float);

////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main() 
{
	FILE* pFile = fopen("Data.txt", "r");
	DoubleListNode* listOfLists = NULL;

	if (pFile)
	{
		char name[128];
		unsigned int departmentCode;
		int age;
		float hourlySalary;
		char linebuffer[128];
		char delimiter[] = ",\n";
		char* token;

		while (fgets(linebuffer, sizeof(linebuffer), pFile))
		{
			token = strtok(linebuffer, delimiter);
			strcpy(name, token);

			token = strtok(NULL, delimiter);
			departmentCode = (unsigned int)atoi(token);

			token = strtok(NULL, delimiter);
			age = atoi(token);

			token = strtok(NULL, delimiter);
			hourlySalary = (float)atof(token);

			EmployeeInfo* emp = createEmployee(name, departmentCode, age, hourlySalary);
			listOfLists = insertNode(listOfLists, emp);
		}
		fclose(pFile);

		// 1) Display the list-of-lists
		displayListOfLists(listOfLists);

		// 2) Create an array of employees whose salary is above 160.5
		int* countAbove = (int*)malloc(sizeof(int));
		countAbove[0] = 0;
		EmployeeInfo** employeesArr = employeesAboveSalary(listOfLists, countAbove, 160.5f);
		displayArray(employeesArr, countAbove);

		// 3) Build a queue from employees in department #2
		ListNode* queue = buildQueueFromDepartment(listOfLists, 2);
		// Optional: displayQueue(queue);

		// 4) Create an array of employees with the 3 smallest salaries from queue
		int* countSmallest = (int*)malloc(sizeof(int));
		countSmallest[0] = 0;
		EmployeeInfo** smallestSalariesArr = smallestSalaries(queue, countSmallest);
		displayArray(smallestSalariesArr, countSmallest);

		// Cleanup or further usage...
	}
	else
	{
		printf("Could not open file: Data.txt\n");
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////////

// Creates a new Employee
EmployeeInfo* createEmployee(char* name, unsigned int departmentCode, int age, float hourlySalary)
{
	EmployeeInfo* e = (EmployeeInfo*)malloc(sizeof(EmployeeInfo));
	e->name = (char*)malloc(strlen(name) + 1);
	strcpy(e->name, name);
	e->departmentCode = departmentCode;
	e->age = age;
	e->hourlySalary = hourlySalary;
	return e;
}

// Creates a new top-level node in the "ListOfList"
DoubleListNode* createDoubleListNode(EmployeeInfo* emp)
{
	DoubleListNode* node = (DoubleListNode*)malloc(sizeof(DoubleListNode));
	node->info = emp;
	node->neighbours = NULL;
	node->next = NULL;
	return node;
}

// Inserts an Employee into the list-of-lists data structure
DoubleListNode* insertNode(DoubleListNode* listOfLists, EmployeeInfo* emp)
{
	// If empty, simply create and return a new node
	if (listOfLists == NULL)
	{
		listOfLists = createDoubleListNode(emp);
		return listOfLists;
	}
	else
	{
		DoubleListNode* start = listOfLists;
		// Search for a matching department
		while (listOfLists != NULL && emp->departmentCode != listOfLists->info->departmentCode)
		{
			listOfLists = listOfLists->next;
		}

		// If we reached the end (NULL), no matching department is found
		if (listOfLists == NULL)
		{
			DoubleListNode* newHead = createDoubleListNode(emp);
			newHead->next = start;
			start = newHead;
		}
		else
		{
			// We found the matching department node
			ListNode* newNode = createNode(emp);
			newNode->next = listOfLists->neighbours;
			listOfLists->neighbours = newNode;
		}
		return start;
	}
}

// Creates a singly linked list node
ListNode* createNode(EmployeeInfo* emp)
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->info = emp;
	node->next = NULL;
	return node;
}

// Displays the entire list-of-lists
void displayListOfLists(DoubleListNode* listOfLists)
{
	DoubleListNode* aux = listOfLists;
	while (aux)
	{
		displayEmployee(aux->info);
		if (aux->neighbours)
		{
			ListNode* n = aux->neighbours;
			while (n)
			{
				displayEmployee(n->info);
				n = n->next;
			}
		}
		aux = aux->next;
	}
}

// Displays a single Employee
void displayEmployee(EmployeeInfo* emp)
{
	if (emp)
	{
		printf("DeptID: %d | Name: %s | Age: %d | Salary: %.2f\n",
		       emp->departmentCode, emp->name, emp->age, emp->hourlySalary);
	}
}

// Creates an array of employees whose salary is above the given threshold
EmployeeInfo** employeesAboveSalary(DoubleListNode* listOfLists, int* count, float threshold)
{
	DoubleListNode* aux = listOfLists;
	// First pass: count how many match
	while (aux)
	{
		if (aux->info->hourlySalary > threshold)
		{
			count[0]++;
		}
		ListNode* ln = aux->neighbours;
		while (ln)
		{
			if (ln->info->hourlySalary > threshold)
			{
				count[0]++;
			}
			ln = ln->next;
		}
		aux = aux->next;
	}

	// Allocate the array
	EmployeeInfo** result = (EmployeeInfo**)malloc(sizeof(EmployeeInfo*) * count[0]);
	memset(result, 0, sizeof(EmployeeInfo*) * count[0]);

	// Second pass: fill in the array
	aux = listOfLists;
	int idx = 0;
	while (aux)
	{
		if (aux->info->hourlySalary > threshold)
		{
			result[idx++] = createEmployee(aux->info->name,
			                               aux->info->departmentCode,
			                               aux->info->age,
			                               aux->info->hourlySalary);
		}
		ListNode* ln = aux->neighbours;
		while (ln)
		{
			if (ln->info->hourlySalary > threshold)
			{
				result[idx++] = createEmployee(ln->info->name,
				                               ln->info->departmentCode,
				                               ln->info->age,
				                               ln->info->hourlySalary);
			}
			ln = ln->next;
		}
		aux = aux->next;
	}
	return result;
}

// Displays an array of employees
void displayArray(EmployeeInfo** arr, int* count)
{
	printf("Array of employees:\n-----------------\n");
	for (int i = 0; i < count[0]; i++)
	{
		displayEmployee(arr[i]);
	}
}

// Builds a queue (singly circular list) of employees from a given department
ListNode* buildQueueFromDepartment(DoubleListNode* listOfLists, int dept)
{
	// Find the node in top-level list for that department
	while (listOfLists && listOfLists->info->departmentCode != dept)
	{
		listOfLists = listOfLists->next;
	}
	if (listOfLists == NULL)
	{
		return NULL;
	}

	// Create a circular list from that node's info + neighbours
	ListNode* queue = createNode(createEmployee(listOfLists->info->name,
	                                           listOfLists->info->departmentCode,
	                                           listOfLists->info->age,
	                                           listOfLists->info->hourlySalary));
	queue->next = queue; // circular
	ListNode* tail = queue;

	// Add each neighbour to that queue
	ListNode* ln = listOfLists->neighbours;
	while (ln)
	{
		ListNode* node = createNode(createEmployee(ln->info->name,
		                                           ln->info->departmentCode,
		                                           ln->info->age,
		                                           ln->info->hourlySalary));
		node->next = tail->next;
		tail->next = node;
		tail = node;
		ln = ln->next;
	}
	return tail;
}

// Displays and empties a queue
void displayQueue(ListNode* tail)
{
	if (!tail) return;

	ListNode* node = NULL;
	printf("Queue (circular list) content:\n");
	while ((node = getNodeFromQueue(&tail)) != NULL)
	{
		displayEmployee(node->info);
		// Optionally free node->info etc.
	}
}

// Gets one node from the front of the queue
ListNode* getNodeFromQueue(ListNode** tail)
{
	if (!(*tail)) 
	{
		return NULL;
	}
	ListNode* front = (*tail)->next;
	if (front == (*tail))
	{
		// Only one element
		*tail = NULL;
		return front;
	}
	else
	{
		(*tail)->next = front->next;
		return front;
	}
}

// Creates an array of the 3 smallest-salary employees from the given queue
EmployeeInfo** smallestSalaries(ListNode* tail, int* count)
{
	count[0] = 3; // as per the code snippet

	EmployeeInfo** arr = (EmployeeInfo**)malloc(sizeof(EmployeeInfo*) * count[0]);
	memset(arr, 0, sizeof(EmployeeInfo*) * count[0]);

	for (int i = 0; i < 3; i++)
	{
		// Find min salary in the queue
		ListNode* minNode = tail;
		ListNode* aux = tail->next;
		do
		{
			if (aux->info->hourlySalary < minNode->info->hourlySalary)
			{
				minNode = aux;
			}
			aux = aux->next;
		} while (aux != tail);

		// Copy that employee
		arr[i] = createEmployee(minNode->info->name,
		                        minNode->info->departmentCode,
		                        minNode->info->age,
		                        minNode->info->hourlySalary);

		// Mark or set the min salary employee
		clearQueueBySalary(&tail, minNode->info->hourlySalary);
	}
	return arr;
}

// Recursively marks the queue node with the specified salary
// with some sentinel value (like 1000), as done in code snippet
void clearQueueBySalary(ListNode** tail, float salary)
{
	if ((*tail)->info->hourlySalary != salary)
	{
		clearQueueBySalary(&(*tail)->next, salary);
	}
	else
	{
		(*tail)->info->hourlySalary = 1000; 
	}
}
