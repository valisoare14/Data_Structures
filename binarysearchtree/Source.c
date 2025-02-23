#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#define V_INIT_SIZE 8

// Structure describing a photograph
struct Photo
{
	unsigned int photoId;       // was idFotografie
	char* city;                 // was oras
	char* date;                 // was data
	float resolution;           // was rezolutie
};
typedef struct Photo PhotoInfo;

// Standard binary search tree structure (by photoId)
struct BinarySearchTree
{
	PhotoInfo* info;
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
};
typedef struct BinarySearchTree BinarySearchTree;

// Function prototypes
PhotoInfo* createPhoto(unsigned int, char*, char*, float);
void insertBST(BinarySearchTree**, PhotoInfo*);
BinarySearchTree* createNode(PhotoInfo*);
void inorder(BinarySearchTree*);
void displayPhoto(PhotoInfo*);
int searchForPhotosInCity(BinarySearchTree*, char*);
void modifyDate(BinarySearchTree**, unsigned int, char*);
void deallocateRoot(BinarySearchTree**);
PhotoInfo** fromBSTToVector(BinarySearchTree*, float, int*);
int searchNumberOfPhotos(BinarySearchTree*, float);
void populateVector(BinarySearchTree*, PhotoInfo**, int*, float);
void printVector(PhotoInfo**, int*);
void displayPhotosAfterDate(BinarySearchTree*, char*);

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	BinarySearchTree* BST = NULL;
	if (pFile)
	{
		unsigned int photoId;
		char city[128];
		char date[128];
		float resolution;
		char* token;
		char delimiter[] = ",\n";
		char buffer[128];

		// Read data from file and insert into BST
		while (fgets(buffer, sizeof(buffer), pFile))
		{
			token = strtok(buffer, delimiter);
			photoId = atoi(token);

			token = strtok(NULL, delimiter);
			strcpy(city, token);

			token = strtok(NULL, delimiter);
			strcpy(date, token);

			token = strtok(NULL, delimiter);
			resolution = atof(token);

			PhotoInfo* photo = createPhoto(photoId, city, date, resolution);
			insertBST(&BST, photo);
		}

		// 1) In-order display
		inorder(BST);

		// 2) Search the number of photos taken in city "Giurgiu"
		{
			int photoCount = searchForPhotosInCity(BST, "Giurgiu");
			printf("Number of photos in the city Giurgiu: %d\n", photoCount);
		}

		// 3) Modify date of the photo with a certain ID
		{
			modifyDate(&BST, 23, "2023-07-07");
			printf("After modifying date:\n\n");
			inorder(BST);
		}

		// 4) Deallocate root node (example operation)
		{
			printf("After deallocating root:\n\n");
			deallocateRoot(&BST);
			inorder(BST);
		}

		// 5) Extract photos of a certain resolution into a vector
		{
			int* size = (int*)malloc(sizeof(int));
			size[0] = 0;
			PhotoInfo** vector = fromBSTToVector(BST, 120, size);
			printVector(vector, size);
		}

		// 6) Display only photos taken after the date "2024-07-05"
		{
			printf("Photos taken after 2024-07-05:\n\n");
			displayPhotosAfterDate(BST, "2024-07-05");
		}
	}
}

// Creates a new Photo
PhotoInfo* createPhoto(unsigned int photoId, char* city, char* date, float resolution)
{
	PhotoInfo* photo = (PhotoInfo*)malloc(sizeof(PhotoInfo));
	photo->date = (char*)malloc(strlen(date) + 1);
	photo->city = (char*)malloc(strlen(city) + 1);
	strcpy(photo->date, date);
	strcpy(photo->city, city);
	photo->photoId = photoId;
	photo->resolution = resolution;
	return photo;
}

// Inserts a photo into BST by ID
void insertBST(BinarySearchTree** BST, PhotoInfo* photo)
{
	if (*BST == NULL)
	{
		*BST = createNode(photo);
	}
	else
	{
		if ((*BST)->info->photoId > photo->photoId)
		{
			insertBST(&(*BST)->left, photo);
		}
		else if ((*BST)->info->photoId < photo->photoId)
		{
			insertBST(&(*BST)->right, photo);
		}
		else
		{
			PhotoInfo* aux = (*BST)->info;
			(*BST)->info = photo;
			free(aux->city);
			free(aux->date);
			free(aux);
		}
	}
}

// Creates a BST node
BinarySearchTree* createNode(PhotoInfo* photo)
{
	BinarySearchTree* bst = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	bst->info = photo;
	bst->left = bst->right = NULL;
	return bst;
}

// In-order traversal
void inorder(BinarySearchTree* BST)
{
	if (BST)
	{
		inorder(BST->left);
		displayPhoto(BST->info);
		inorder(BST->right);
	}
}

// Displays one photo
void displayPhoto(PhotoInfo* photo)
{
	printf("%d -> %s -> %s -> %.2f\n", photo->photoId, photo->city, photo->date, photo->resolution);
}

// Searches for photos in a specified city
int searchForPhotosInCity(BinarySearchTree* BST, char* city)
{
	if (BST)
	{
		if (strcmp(BST->info->city, city) == 0)
		{
			return 1 + searchForPhotosInCity(BST->left, city) + searchForPhotosInCity(BST->right, city);
		}
		else
		{
			return searchForPhotosInCity(BST->left, city) + searchForPhotosInCity(BST->right, city);
		}
	}
	else
	{
		return 0;
	}
}

// Modifies the date of the photo with the given ID
void modifyDate(BinarySearchTree** BST, unsigned int photoId, char* newDate)
{
	if (*BST)
	{
		if ((*BST)->info->photoId == photoId)
		{
			free((*BST)->info->date);
			(*BST)->info->date = (char*)malloc(strlen(newDate) + 1);
			strcpy((*BST)->info->date, newDate);
		}
		else
		{
			modifyDate(&(*BST)->left, photoId, newDate);
			modifyDate(&(*BST)->right, photoId, newDate);
		}
	}
}

// Deallocates the root node and re-links children
void deallocateRoot(BinarySearchTree** BST)
{
	BinarySearchTree* oldRoot = *BST;
	BinarySearchTree* rightChild = (*BST)->right;
	BinarySearchTree* rightChildLeft = (*BST)->right->left;

	if ((*BST)->left != NULL)
	{
		BinarySearchTree* aux = (*BST)->left->right;
		while (aux)
		{
			aux = aux->right;
		}
		aux = rightChild;
		rightChild->left = (*BST)->left;
	}
	*BST = rightChild;
	free(oldRoot->info->date);
	free(oldRoot->info->city);
	free(oldRoot->info);
	free(oldRoot);
}

// Creates a vector of photos from the BST that match a certain resolution
PhotoInfo** fromBSTToVector(BinarySearchTree* BST, float resolution, int* size)
{
	int n = searchNumberOfPhotos(BST, resolution);
	PhotoInfo** vector = (PhotoInfo**)malloc(sizeof(PhotoInfo*) * n);
	memset(vector, 0, sizeof(PhotoInfo*) * n);
	populateVector(BST, vector, size, resolution);
	return vector;
}

// Searches how many photos of a specified resolution are in the BST
int searchNumberOfPhotos(BinarySearchTree* BST, float resolution)
{
	if (BST)
	{
		if (BST->info->resolution == resolution)
		{
			return 1 + searchNumberOfPhotos(BST->left, resolution) + searchNumberOfPhotos(BST->right, resolution);
		}
		else
		{
			return searchNumberOfPhotos(BST->left, resolution) + searchNumberOfPhotos(BST->right, resolution);
		}
	}
	else
	{
		return 0;
	}
}

// Populates the vector with photos that match a given resolution
void populateVector(BinarySearchTree* BST, PhotoInfo** vector, int* size, float resolution)
{
	if (BST)
	{
		if (BST->info->resolution == resolution)
		{
			vector[size[0]] = createPhoto(BST->info->photoId, BST->info->city, BST->info->date, BST->info->resolution);
			size[0]++;
		}
		populateVector(BST->left, vector, size, resolution);
		populateVector(BST->right, vector, size, resolution);
	}
}

// Prints a vector of photos
void printVector(PhotoInfo** vector, int* size)
{
	if (vector)
	{
		printf("Vector:\n\n");
		for (int i = 0; i < size[0]; i++)
		{
			displayPhoto(vector[i]);
		}
	}
}

// Displays only photos after a specified date
void displayPhotosAfterDate(BinarySearchTree* BST, char* threshold)
{
	if (BST)
	{
		char delimiter[] = "-\0";
		char* token;
		char buffer1[128];
		strcpy(buffer1, threshold);

		token = strtok(buffer1, delimiter);
		int yThreshold = atoi(token);
		token = strtok(NULL, delimiter);
		int mThreshold = atoi(token);
		token = strtok(NULL, delimiter);
		int dThreshold = atoi(token);

		memset(buffer1, 0, 128);
		strcpy(buffer1, BST->info->date);
		token = strtok(buffer1, delimiter);
		int y = atoi(token);
		token = strtok(NULL, delimiter);
		int m = atoi(token);
		token = strtok(NULL, delimiter);
		int d = atoi(token);

		if (y >= yThreshold)
		{
			if (y > yThreshold)
			{
				displayPhoto(BST->info);
			}
			else
			{
				if (m >= mThreshold)
				{
					if (m > mThreshold)
					{
						displayPhoto(BST->info);
					}
					else
					{
						if (d > dThreshold)
						{
							displayPhoto(BST->info);
						}
					}
				}
			}
		}
		displayPhotosAfterDate(BST->left, threshold);
		displayPhotosAfterDate(BST->right, threshold);
	}
}
