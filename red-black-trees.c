#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOTAL_CHARACTERS 256

typedef struct Node
{
	struct Node* left;
	struct Node* right;
	struct Node* parent;
	
	int isCharacter;
	int character;
	
	int occurrences;
} Node;

Node* nodeArray[TOTAL_CHARACTERS];

int charOccurrenceArray[TOTAL_CHARACTERS];

void initializeNodeArray(Node* array[], int arraySize)
{
	for(int i = 0; i < arraySize; i++)
	{
		Node* newNode = malloc(sizeof(Node));
		newNode->left = newNode->right = newNode->parent = NULL;
		newNode->isCharacter = 1;
		newNode->character = i;
		newNode->occurrences = 0;
		
		array[i] = newNode;
	}
}

int countOccurrencesInFile(Node* array[], int arraySize, char* fileName)
{
	FILE* file = fopen(fileName, "r");
	int totalOccurrences = -1;
	
	char currentCharacter;
	do
	{
		currentCharacter = fgetc(file);
		if((int)currentCharacter >= 0 && (int)currentCharacter < arraySize)
		{
			totalOccurrences++;
			array[(int)currentCharacter]->occurrences++;
			charOccurrenceArray[(int)currentCharacter]++;
		}
	}
	while (currentCharacter != EOF);
	
	fclose(file);
	return totalOccurrences;
}

Node* extractMinimum(Node* array[], int arraySize)
{
	int currentMinimumIndex = 0;
	int currentMinimum = 9999;
	Node* currentMinimumNode = NULL;
	
	for(int i = 0; i < arraySize; i++)
	{
		if(array[i] != NULL)
		{
			if(array[i]->occurrences < currentMinimum)
			{
				currentMinimumNode = array[i];
				currentMinimum = currentMinimumNode->occurrences;
				currentMinimumIndex = i;
			}
		}
	}
	
	array[currentMinimumIndex] = NULL;
	return currentMinimumNode;
}

void mergeNodes(Node* array[], int arraySize)
{
	// find minNodes
	Node* min1 = extractMinimum(array, arraySize);
	Node* min2 = extractMinimum(array, arraySize);
	
	// create Zx node
	Node* Zx = malloc(sizeof(Node));
	Zx->left = min1;
	Zx->right = min2;
	Zx->parent = NULL;
	Zx->character = min1->character;
	Zx->isCharacter = 0;
	Zx->occurrences = min1->occurrences + min2->occurrences;
	
	// put Zx in the place where min1 was in the array
	array[min1->character] = Zx;
}

int countNodeArrayElements(Node* array[], int arraySize)
{
	int size = 0;
	
	for(int i = 0; i < arraySize; i++)
	{
		if(array[i] != NULL)
		{
			size++;
		}
	}
	
	return size;
}

Node* buildTree(Node* array[], int arraySize)
{
	Node* root = NULL;
	
	// merge elements till only one left
	int nodeArrayElementsCount = countNodeArrayElements(array, arraySize);
	while(nodeArrayElementsCount > 1)
	{
		mergeNodes(array, arraySize);
		nodeArrayElementsCount = countNodeArrayElements(array, arraySize);
	}
	
	// find remaining element
	for(int i = 0; i < arraySize; i++)
	{
		if(array[i] != NULL)
		{
			root = array[i];
		}
	}
	
	return root;
}

void clearEmptyNodes(Node* array[], int arraySize)
{
	int cleared = 0;
	for(int i = 0; i < arraySize; i++)
	{
		if(array[i] != NULL)
		{
			if(array[i]->occurrences == 0)
			{
				cleared++;
				array[i] = NULL;
			}
		}
	}
}

void prepareDictionary(Node* root, char* dictionary[], char* codePart)
{
	Node* currentNode = root;
	
	if(currentNode->isCharacter == 1)
	{
		char charCode[TOTAL_CHARACTERS];
		strcpy(charCode, codePart);
		strcat(charCode, "\0");
		strcpy(dictionary[(int)currentNode->character], charCode);
	}		
	
	else
	{
		char codeLeft[TOTAL_CHARACTERS];
		strcpy(codeLeft, codePart);
		strcat(codeLeft, "0");
		prepareDictionary(currentNode->left, dictionary, codeLeft);
		
		char codeRight[TOTAL_CHARACTERS];
		strcpy(codeRight, codePart);
		strcat(codeRight, "1");
		prepareDictionary(currentNode->right, dictionary, codeRight);
	}
}

int countSize(int occurrences[], char* dictionary[], int arraySize)
{
	int totalSize = 0;
	
	for(int i = 0; i < arraySize; i++)
	{
		totalSize += occurrences[i] * strlen(dictionary[i]);
	}
	
	return totalSize;
}

void main(int argc, char** argv)
{
	// initialize node array
	initializeNodeArray(nodeArray, TOTAL_CHARACTERS);
		
	// read file + count character occurrences
	int totalChars = countOccurrencesInFile(nodeArray, TOTAL_CHARACTERS, argv[1]);
	
	// clear nodes with 0 occurrences
	clearEmptyNodes(nodeArray, TOTAL_CHARACTERS);
	
	// build tree
	Node* root = buildTree(nodeArray, TOTAL_CHARACTERS);
	
	// prepare dictionary for coded characters
	char* dictionary[TOTAL_CHARACTERS];
	for(int i = 0; i < TOTAL_CHARACTERS; i++)
	{
		char* node = "00000000";
		dictionary[i] = malloc(sizeof(node));
		strcpy(dictionary[i], "\0");
	}
	prepareDictionary(root, dictionary, "");
	
	// prepare quasi-dictionary for non-coded characters
	char* defaultDictionary[TOTAL_CHARACTERS];
	for(int i = 0; i < TOTAL_CHARACTERS; i++)
	{
		char* node = "00000000";
		defaultDictionary[i] = node;
	}
	
	// print result
	for(int i = 1; i < TOTAL_CHARACTERS; i++)
	{
		if(strlen(dictionary[i]) > 0)
		{
			printf("[%c = %3i]: %10s (%i) \n", (char)i, i, dictionary[i], (int)strlen(dictionary[i]));
		}
	}
	
	int inputSize = countSize(charOccurrenceArray, defaultDictionary, TOTAL_CHARACTERS);
	int outputSize = countSize(charOccurrenceArray, dictionary, TOTAL_CHARACTERS);
	
	printf("INPUT SIZE: %i \nOUTPUT SIZE: %i \n", inputSize, outputSize);
	
	//printf("total: %d chars => %d size \n", totalChars, totalChars*8);
}