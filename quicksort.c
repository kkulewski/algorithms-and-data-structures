#include <stdio.h>

int Partition(int Array[], int pivotIndex, int length)
{
	int temp, i, j, pivotValue;
	
	//current pivot value for comparison
	pivotValue = Array[length-1];
	printf("Current pivotValue: %i\nCurrent length: %i\n\n", pivotValue, length-1);
	//wall
	i = pivotIndex - 1;
	
	for(j = pivotIndex; j < length; j++)
	{
		if(Array[j] <= pivotValue)
		{
			i++;
			temp = Array[i];
			Array[i] = Array[j];
			Array[j] = temp;
		}
	}
	
	if(i < length)
		return i;
	else
		return i-1;
}

void Quicksort(int Array[], int pivotIndex, int length)
{
	if (pivotIndex < length)
	{
		int q = Partition(Array, pivotIndex, length);
		Quicksort(Array, pivotIndex, q);
		Quicksort(Array, q+1, length);
	}
}

int main(int argc, char* argv[])
{
	//sanity check
	if(argc != 3)
	{
		printf("Wrong number of arguments supplied!\nPlease try again in this manner:\n./heapsort SOURCE_FILE.txt DESTINATION_FILE.txt");
		return 1;
	}
	
	//F
	char const* sourceFileName = argv[1];
	char const* destinationFileName = argv[2];
	FILE* sourceFile;
	FILE* destinationFile;
	
	//TRY TO OPEN SOURCE FILE
	if(!(sourceFile = fopen(sourceFileName, "r")))
	{
		printf("Source file does not exist!");
		return 1;
	}
	
	//COUNT LINES
	int fileLength = 0;
	char c;
    while( (c=fgetc(sourceFile)) != EOF) 
	{
        if(c == '\n')
            fileLength++;
    }
	fclose(sourceFile);
	
	//there is only 4 newlines in file containing 5 elements, so we need to increment array length by 1
	int length = fileLength+1;
	
	//INIT ARRAY
	int Array[length];
		//printf("Number of lines: %i\n", length);
	
	//FILL ARRAY FROM INPUT FILE
	sourceFile = fopen(sourceFileName, "r");
	for(int j = 0; j < length; j++)
	{
		fscanf(sourceFile, "%i\n", &Array[j]);
	}
	fclose(sourceFile);
	
	//SORT
	Quicksort(Array, 0, length);
	
	//TRY TO OPEN DESTINATION FILE
	if(!(destinationFile = fopen(destinationFileName, "w")))
	{
		printf("Cannot create destination file!");
		return 1;
	}
	
	//OUTPUT
	for(int i = 0; i < length; i++)
	{
		fprintf(destinationFile, "%i\r\n", Array[i]);
	}
	fclose(destinationFile);
	

	return 0;
}