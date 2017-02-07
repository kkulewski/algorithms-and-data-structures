#include <stdio.h>
#include <string.h>

void main()
{
	int wordCount = 16;
	int i, j;
	const int ALPHABET_LENGTH = 37;
	
	char* source[] = { "nowak", "kowalski", "wisniewski", "1nowaf", "kozlowski", "9mazur", "krawczyk", "nowakowski", "michalski", "nowicki", "na", "micha4ski", "blablablablablablablabla", "a", "abcv", "222146913" };
	char* output[wordCount];
	int wordLength[wordCount];
	int wordLengthOut[wordCount];
	
	int longestWordLength = strlen(source[0]);
	
	for(i=0; i < wordCount; i++)
	{
		wordLength[i] = strlen(source[i]);
		if(wordLength[i] > longestWordLength)
			longestWordLength = wordLength[i];
	}

	printf("INPUT:\n");
	for(i=0; i < wordCount; i++)
		printf("[%2i] %s\n", wordLength[i], source[i]);	
	
	
	for(i=longestWordLength; i > 0; i--)
	{
		int TimesOccurred[ALPHABET_LENGTH];
		for(j=0; j<ALPHABET_LENGTH; j++)
			TimesOccurred[j] = 0;
		
		
		for(j=0; j < wordCount; j++)
		{
			int letterNumber;
			if(wordLength[j] >= i)
			{
				int _letterNumber = 0;
				_letterNumber = (int)(source[j][i-1]);

				//if letterNumber in range [48..57] then its [0..9] -> dont shift at all
				if(_letterNumber >= 48 && _letterNumber <= 57)
				{
					letterNumber = _letterNumber - 48;
				}
				//if letterNumber in range [65..90] then shift by 10 and substract 'A' as its [A..Z]
				if(_letterNumber >= 65 && _letterNumber <= 90)
				{
					letterNumber = _letterNumber - 65 + 10;
				}
				//if letterNumber in range [39..65] then substract 'a' again and shift 10 right [a..z]
				if(_letterNumber >= 97 && _letterNumber <= 122)
				{
					letterNumber = _letterNumber - 97 + 10;
				}
				letterNumber += 1;
			}
			else
				letterNumber = 0;
			
			TimesOccurred[letterNumber] += 1;
		}
		

		for(j=1; j < ALPHABET_LENGTH; j++)
		{
			TimesOccurred[j] += TimesOccurred[j-1];
		}
		

		for(j=wordCount-1; j >= 0; j--)
		{
			int letterNumber;

			if(wordLength[j] >= i)
			{
				int _letterNumber = 0;
				_letterNumber = (int)(source[j][i-1]);

				//if letterNumber in range [48..57] then its [0..9] -> dont shift at all
				if(_letterNumber >= 48 && _letterNumber <= 57)
				{
					letterNumber = _letterNumber - 48;
				}
				//if letterNumber in range [65..90] then shift by 10 and substract 'A' as its [A..Z]
				if(_letterNumber >= 65 && _letterNumber <= 90)
				{
					letterNumber = _letterNumber - 65 + 10;
				}
				//if letterNumber in range [39..65] then substract 'a' again and shift 10 right [a..z]
				if(_letterNumber >= 97 && _letterNumber <= 122)
				{
					letterNumber = _letterNumber - 97 + 10;
				}
				letterNumber += 1;

			}
			else
				letterNumber = 0;
			
			TimesOccurred[letterNumber] -= 1;
			
			wordLengthOut[TimesOccurred[letterNumber]] = wordLength[j];
			output[TimesOccurred[letterNumber]] = source[j];  
		}
		
		
		for(j=0; j < wordCount; j++)
		{
			source[j] = output[j];
			wordLength[j] = wordLengthOut[j];
		}
	}
	
	printf("\nOUTPUT:\n");		
	for(j=0; j < wordCount; j++)
		printf("[%2i] %s\n", wordLength[j], source[j]);
}