#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// TO AVOID ENDLESS LOOP
// AS SOURCE-FILE MIGHT CONTAIN REPETITIONS
int MAX_INSERT_TRY_COUNT = 1000;

typedef struct entry
{
	int popularity;
	char surname[50];
}* entryPointer;

unsigned int ComputeHash(char lineToHash[], int i, int hashTableLength)
{
	unsigned int k = 0, hash = 0;
	
	// COMPUTE WORD VALUE
	while(lineToHash[k] != '\0')
	{
		hash += lineToHash[k];
		k++;
	}
	
	// HASH WORD
	//FORMULA: hash = [(K MOD M) + (i * (K MOD M-2))] MOD M
	hash = ((hash % hashTableLength) + i * (hash % hashTableLength - 2) ) % hashTableLength;

	if(hash < 0)
	{
		hash *= -1;
		printf("## ERR: hash value less than zero\n");
	}
	return hash;
}

int TryInsert(entryPointer entPointer, entryPointer hashTable[], int position)
{
	//IF NO ENTRY AT GIVEN POSITION -> INSERT AND RETURN TRUE
	if(hashTable[position] == NULL)
	{
		hashTable[position] = entPointer;
		return 1;
	}
	//RETURN FALSE
	return 0;
}

int Insert(entryPointer entPointer, entryPointer hashTable[], int hashTableLength) 
{
	int i = 1; 
	int position;
	int tryCount = 0;
	int inserted = 0;
	while(inserted == 0)
	{
		position = ComputeHash(entPointer->surname, i, hashTableLength);
		//1 IF INSERTED, 0 IF NOT
		inserted = TryInsert(entPointer, hashTable, position);
		i++;
		
		tryCount++;
		
		if(tryCount > MAX_INSERT_TRY_COUNT)
		{
			printf("## ERROR: cannot insert [%20s] -- try count exceeded %i. (repetition?)\n", entPointer->surname, MAX_INSERT_TRY_COUNT);
			break;
		}
	}
	return tryCount;
}

double ParseAndInsert(char* fileName, int tableLength, double fillRatio, int testModeOn)
{
	// -------------------------------------------------------------------------------------
	// ------------------------------ PREPARE TABLE WITH GIVEN FILL RATIO ------------------
	// -------------------------------------------------------------------------------------
	
	//CREATE HASH-TABLE WITH GIVEN SIZE AND NULL IT
	entryPointer hashTable[tableLength];
	for(int k = 0; k < tableLength; k++)
		hashTable[k] = NULL;
	
	//FILE PREPARATION
	FILE* file = fopen(fileName, "r");
    char line[256];
	
	//STATISTICS DATA
	int insertedEntryCount = 0;
	int insertionTryCount = 0;
	double currentFillRatio = 0.0;
	double averageInsertionTryCount = 0.0;
	//TEMP
	int currentPopularity = 0;
	char currentSurname[256];
	int currentTryCount = 0;
	
	//READ AND HASH LINE BY LINE
    while (fscanf(file, "%i %s", &currentPopularity, &currentSurname) != EOF && currentFillRatio < fillRatio) 
	{
		//CREATE NEW STRUCT INSTANCE
		entryPointer entry = NULL;
		entry = calloc(1, sizeof(entry));
		entry->popularity = currentPopularity;
		strcpy(entry->surname, currentSurname);
		
		//HASH AND INSERT
		int currentTryCount = Insert(entry, hashTable, tableLength);
		//currentTryCount VALUE MIGHT BE USEFULL IN FINDING "UNLUCKY" VALUES
		//IE WHEN currentTryCount > X (FOR GIVEN X)
		
		//INCREASE ENTRY AND TRY COUNT
		insertedEntryCount++;
		insertionTryCount += currentTryCount;
		//COMPUTE CURRENT FILL RATIO
		currentFillRatio = (double)insertedEntryCount / (double)tableLength;
    }
    //fclose(file);
	// ^ not yet, as we are about to add 5m/100 entries more
	
	//COMPUTE AVERAGE OF TOTAL ATTEMPTS PER INSERTION
	averageInsertionTryCount = (double)insertionTryCount / (double)insertedEntryCount; 
	
	
	// -------------------------------------------------------------------------------------
	// ------------------------------ INSERT 5m/100 ENTRIES --------------------------------
	// -------------------------------------------------------------------------------------
	
	//SET fillRatio to 5m/100 = 0.05%
	fillRatio = 0.05;
	
	//ZERO VARIABLES AGAIN
	insertedEntryCount = 0.0;
	insertionTryCount = 0.0;
	currentFillRatio = 0.0;
	
	while (fscanf(file, "%i %s", &currentPopularity, &currentSurname) != EOF && currentFillRatio < fillRatio) 
	{
		//CREATE NEW STRUCT INSTANCE
		entryPointer entry = NULL;
		entry = calloc(1, sizeof(entry));
		entry->popularity = currentPopularity;
		strcpy(entry->surname, currentSurname);
		
		//HASH AND INSERT
		int currentTryCount = Insert(entry, hashTable, tableLength);
		
		//INCREASE ENTRY AND TRY COUNT
		insertedEntryCount++;
		insertionTryCount += currentTryCount;
		//COMPUTE CURRENT FILL RATIO
		currentFillRatio = (double)insertedEntryCount / (double)tableLength;
    }
	
	averageInsertionTryCount = (double)insertionTryCount / (double)insertedEntryCount; 
	
	
	
	// -------------------------------------------------------------------------------------
	// ------------------------------ IF TEST MODE -> PRINT RESULT -------------------------
	// -------------------------------------------------------------------------------------
	if(testModeOn == 1)
	{
		printf("TEST MODE (TAB LENGTH: %i)\n", tableLength);
		for(int z = 0; z < tableLength; z++)
		{
			if(hashTable[z] != NULL)
				printf("TAB[%3i]: [%6i] [%20s]\n", z, hashTable[z]->popularity, hashTable[z]->surname);
			else
				printf("TAB[%3i]: [------] [--------------------]\n", z);
		}

	}
	
	return averageInsertionTryCount;
	
}


int main(int argc, char* argv[])
{
	//FOR EASIER COPY-PASTING NEW TESTS
	char* fileName = argv[1];
	int tableLength = 0;
	int testMode = 0;
	double fillRatio = 0.0;
	double testResult = 0.0;
	//CALL AS FOLLOWS: ParseAndInsert(FILE_NAME, TABLE_LENGTH, FILL_RATIO, TEST_MODE_SWITCH)
	
	// /*
	//SMALL TEST
	tableLength = 43;
	testMode = 1;
	
	fillRatio = 0.4;
	printf("\n[LENGTH: %i, FILL RATIO: %.2lf ", tableLength, fillRatio );
	printf("=> %i + %i entries]\n", (int)ceil((double)tableLength*(double)fillRatio), (int)ceil((double)tableLength*0.05));
	
	testResult = ParseAndInsert(fileName, tableLength, fillRatio, 1);
	printf("TEST [%0.2lf FILL]: %lf \n", fillRatio, testResult);
	// */
	
	
	//STANDARD TEST
	tableLength = 5233;
	testMode = 0;
	
	printf("\nTABLE LENGTH = %i \n", tableLength);
	printf("(adding next 5m/100 entries and calculating avg. try count for each fill ratio) \n");
	
	fillRatio = 0.5;
	testResult = ParseAndInsert(fileName, tableLength, fillRatio, testMode);
	printf("TEST [%0.2lf FILL]: %lf \n", fillRatio, testResult);
	
	fillRatio = 0.7;
	testResult = ParseAndInsert(fileName, tableLength, fillRatio, testMode);
	printf("TEST [%0.2lf FILL]: %lf \n", fillRatio, testResult);
	
	fillRatio = 0.9;
	testResult = ParseAndInsert(fileName, tableLength, fillRatio, testMode);
	printf("TEST [%0.2lf FILL]: %lf \n", fillRatio, testResult);
	
	
	
    return 0;
}