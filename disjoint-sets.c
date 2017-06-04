#include <stdio.h>
#include <stdlib.h>

#define GRAPH_ARRAY_LENGTH 1000

typedef struct N
{
	int key;
	struct N* parent;
	int rank;
	
} Node;

Node* GraphArray[GRAPH_ARRAY_LENGTH];

Node* MakeSet(int key)
{
	Node* n = malloc(1 * sizeof(Node));
	n->key = key;
	n->rank = 0;
	n->parent = n;
	
	return n;
}

Node* FindSet(Node* n)
{
	if(n != n->parent)
	{
		n->parent = FindSet(n->parent);
	}
	return n->parent;
}

Node* Union(Node* y, Node* x)
{
	if(x->rank > y->rank)
	{
		y->parent = x;
		return y;
	}
		
	x->parent = y;	
	if(x->rank == y->rank)
	{
		y->rank++;
	}
	return x;
}

void Print(Node* n)
{
	// without rank
	//printf("%i", n->key);
	// with rank
	printf("[%i](r:%i)", n->key, n->rank);
	if(n != n->parent)
	{
		printf(" -> ");
		Print(n->parent);
	}
}

int VertexExists(int key)
{
	for(int i = 0; i < GRAPH_ARRAY_LENGTH; i++)
	{
		if(GraphArray[i] != NULL)
		{
			if(GraphArray[i]->key == key)
				return 1;
		}
	}
	return 0;
}

void AddVertex(int key)
{
	if(VertexExists(key) == 0)
	{
		GraphArray[key] = MakeSet(key);
		printf(" SUCCESS: [%i] added. \n", key);
	}
	else
	{
		printf(" FAILURE: [%i] already exists. \n", key);
	}

}

void AddEdge(int key1, int key2)
{
	if(VertexExists(key1) == 1)
	{
		if(VertexExists(key2) == 1)
		{
			Union(FindSet(GraphArray[key1]), FindSet(GraphArray[key2]));
			printf(" SUCCESS: edge [%i] - [%i] added. \n", key1, key2);
		}
		else
		{	
			printf(" FAILURE: [%i] not present in graph. \n", key2);
		}
	}
	else
	{
		printf(" FAILURE: [%i] not present in graph. \n", key1);
	}
}

void IsInSameSpanningTree(int key1, int key2)
{
	if(VertexExists(key1) == 1)
	{
		if(VertexExists(key2) == 1)
		{
			if(FindSet(GraphArray[key1]) == FindSet(GraphArray[key2]))
				printf(" SUCCESS: [%i] and [%i] in THE SAME spanning tree. \n", key1, key2);
			else
				printf(" SUCCESS: [%i] and [%i] in DIFFERENT trees. \n", key1, key2);
		}
		else
		{
			printf(" FAILURE: [%i] not present in graph. \n", key2);
		}
	}
	else
	{
		printf(" FAILURE: [%i] not present in graph. \n", key1);
	}
}

void main()
{
	/*
	// BEGIN AL 12.2
	Node* Z[10];
	for(int i = 1; i < 10; i++)
		Z[i] = MakeSet(i);
	
	Union(FindSet(Z[1]), FindSet(Z[2]));
	Union(FindSet(Z[3]), FindSet(Z[4]));
	Union(FindSet(Z[5]), FindSet(Z[4]));
	Union(FindSet(Z[1]), FindSet(Z[5]));
	Union(FindSet(Z[6]), FindSet(Z[7]));
	Union(FindSet(Z[8]), FindSet(Z[9]));
	Union(FindSet(Z[6]), FindSet(Z[8]));
	Union(FindSet(Z[7]), FindSet(Z[4]));
	
	for(int i = 1; i < 10; i++)
	{
		Print(Z[i]);
		printf("\n");
	}
	// END AL 12.2
	*/
	
	int run = 1;
	while(run == 1)
	{
		int key1 = 0;
		int key2 = 0;
		int option = 0;
		
		printf("\n\n\n\n\n\n\n\n");
		printf("#################################################################\n");
		printf("## 1. Add vertex. \n");
		printf("## 2. Add edge. \n");
		printf("## 3. Check if key1 and key2 are both in the same spanning tree. \n");
		printf("## 0. EXIT \n");
		printf("#################################################################\n");
		printf(">> option: ");
		scanf("%i", &option);
		switch(option)
		{
			case 1:
				printf(">> input key: ");
				scanf("%i", &key1);
				AddVertex(key1);
				break;
				
			case 2:
				printf(">> input key1: ");
				scanf("%i", &key1);
				printf(">> input key2: ");
				scanf("%i", &key2);
				AddEdge(key1, key2);
				break;
				
			case 3:
				printf(">> input key1: ");
				scanf("%i", &key1);
				printf(">> input key2: ");
				scanf("%i", &key2);
				IsInSameSpanningTree(key1, key2);
				break;
				
			case 0:
				run = 0;
				break;
			
			default:
				printf(">> WRONG OPTION! \n");
				break;
		}
		
		printf("#################################################################\n");
		getchar();
		getchar();
	}
	
}
