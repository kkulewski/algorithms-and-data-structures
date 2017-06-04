#include <stdio.h>
#include <string.h>

#define MAX_LEN 200

int NUM[MAX_LEN][MAX_LEN];
char DIR[MAX_LEN][MAX_LEN];

void LCSLength(char A[], char B[], int lenA, int lenB);
void PrintLCS(char A[], int i, int j);

void main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Zla liczba argumentow. Prawidlowe wywolanie:\n%s ciag1 ciag2\n", argv[0]);
		return;
	}

	int lenA = strlen(argv[1]);
	int lenB = strlen(argv[2]);

	LCSLength(argv[1], argv[2], lenA, lenB);
	PrintLCS(argv[1], lenA, lenB);
	printf("\n");
}

void LCSLength(char A[], char B[], int lenA, int lenB)
{
	for (int i = 1; i <= lenA; i++)
	{
		for (int j = 1; j <= lenB; j++)
		{
			if (A[i - 1] == B[j - 1])
			{
				NUM[i][j] = NUM[i - 1][j - 1] + 1;
				DIR[i][j] = '\\';
			}
			else
			{
				if (NUM[i - 1][j] >= NUM[i][j - 1])
				{
					NUM[i][j] = NUM[i - 1][j];
					DIR[i][j] = '|';
				}
				else
				{
					NUM[i][j] = NUM[i][j - 1];
					DIR[i][j] = '-';
				}
			}
		}
	}
}

void PrintLCS(char A[], int i, int j)
{
	if (i == 0 || j == 0)
		return;

	if (DIR[i][j] == '\\')
	{
		PrintLCS(A, i - 1, j - 1);
		printf("%c", A[i - 1]);
	}
	else
	{
		if (DIR[i][j] == '|')
			PrintLCS(A, i - 1, j);
		else
			PrintLCS(A, i, j - 1);
	}
}