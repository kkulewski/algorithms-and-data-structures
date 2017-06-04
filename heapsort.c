#include <stdio.h>

void IterativeHeapify(int Array[], int currentNode, int length)
{
	int leftChild = 2*currentNode;
	int rightChild = 2*currentNode+1;
	int largestNode;
	int temp;
	
	while(currentNode < length)
	{

		if(leftChild <= length && Array[leftChild]>Array[currentNode])
		{	
			largestNode = leftChild;
		}
		else
		{
			largestNode = currentNode;
		}
		if(rightChild <= length && Array[rightChild]>Array[largestNode])
		{
			largestNode = rightChild;
		}
		if(largestNode != currentNode)
		{
				//printf("largest(%i) != currentNode(%i)\n", largestNode, currentNode);
			temp = Array[currentNode];
			Array[currentNode] = Array[largestNode];
			Array[largestNode] = temp;
			
			currentNode = largestNode;
			leftChild = 2*currentNode;
			rightChild = 2*currentNode+1;
				//printf("currentNode = %i, leftChild = %i, rightChild = %i\n", currentNode, leftChild, rightChild);
		}
		else
		{
				//printf("largest(%i) == currentNode(%i) --BREAK\n", largestNode, currentNode);
			break;
		}
	
	}
}

void Heapify(int Array[], int currentNode, int length)
{
	int leftChild = 2*currentNode;
	int rightChild = 2*currentNode+1;
	int largestNode;
	int temp;
	
	if(leftChild <= length && Array[leftChild]>Array[currentNode])
	{	
		largestNode = leftChild;
	}
	else
	{
		largestNode = currentNode;
	}
	if(rightChild <= length && Array[rightChild]>Array[largestNode])
	{
		largestNode = rightChild;
	}
	if(largestNode != currentNode)
	{
		temp = Array[currentNode];
		Array[currentNode] = Array[largestNode];
		Array[largestNode] = temp;
		Heapify(Array, largestNode, length);
	}
}


void BuildHeap(int Array[], int length)
{
	for(int i = length/2; i > 0; i--)
	{
			//printf("\tBuildHeap calls Heapify(A[], i=%i, length=%i)\n", i, length);
		IterativeHeapify(Array,i,length);
	}
}


void HeapSort(int Array[], int length)
{
	BuildHeap(Array, length);

	int temp = 0;
	for(int i = length; i > 1; i--)
	{
		temp = Array[length];
		Array[length] = Array[1];
		Array[1] = temp;
		length--;

			//printf("\tHeapSort calls Heapify(A[], i=%i, length=%i)\n", 1, length);
		IterativeHeapify(Array, 1, length);
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
	
	//COUNT LINES
	int fileLength = 0;
	char c;
	sourceFile = fopen(sourceFileName, "r");
    while( (c=fgetc(sourceFile)) != EOF) 
	{
        if(c == '\n')
            fileLength++;
    }
	fclose(sourceFile);
	int length = fileLength+1;
	
	//INIT ARRAY
	int Array[length];
		//printf("Number of lines: %i\n", length);
	
	//FILL ARRAY FROM INPUT FILE
	sourceFile = fopen(sourceFileName, "r");
	for(int j = 1; j <= length; j++)
	{
		fscanf(sourceFile, "%i\n", &Array[j]);
	}
	fclose(sourceFile);
	
	//SORT
	HeapSort(Array, length);
	
	//OUTPUT
	destinationFile = fopen(destinationFileName, "w");
	for(int i = 1; i <= length; i++)
	{
		fprintf(destinationFile, "%i\r\n", Array[i]);
	}
	fclose(destinationFile);
	

	return 0;
}