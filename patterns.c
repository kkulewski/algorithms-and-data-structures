#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define ALPHABET_LENGTH 256

void Oczywisty(char* pattern, char* source)
{
	int patternLength = strlen(pattern);
	int sourceLength = strlen(source);
	int i, j;
	
	// loop over source characters
	for(i = 0; i < sourceLength - patternLength; i++)
	{
		// check char by char
		for(j = 0; j < patternLength; j++)
		{
			if(source[i+j] != pattern[j])
				break;
		}
		
		// if j correct characters of source == patternLength => found match
		if(j == patternLength)
		{
			printf("[OCZYWISTY         ] - index: %i\n", i);
		}
	}
}

void RabinKarp(char* pattern, char* source)
{
	int patternLength = strlen(pattern);
	int sourceLength = strlen(source);
	int i, j;
	
	int  h = 1, d = ALPHABET_LENGTH, q = 27077;
	
	// h = d^(m-1) % q
	// make h to the power of patternLength-1, modulo q
	for(i = 0; i < patternLength - 1; i++)
	{
		h = (h * d) % q;
	}
	
	// p - pattern hash, t - source window hash 
	int p = 0, t = 0; 
	for(i = 0; i < patternLength; i++)
	{
		// hash of pattern 
		p = (d * p + pattern[i]) % q;
		// hash first "pattern-window" of source
		t = (d * t + source[i]) % q;
	}
	
	// compare pattern with source char by char
	// move "window" of pattern length over source text
	// 
	// example: pattern = DE
	// [AB]CDEF => A[BC]DEF => AB[CD]EF => ABC[DE]F (match) => ABCD[EF]
	for(i = 0; i <= sourceLength - patternLength; i++)
	{
		// if hash of pattern == hash of current window => possible match
		if(p == t)
		{
			// check char one by one
			for(j = 0; j < patternLength; j++)
			{
				if(source[i+j] != pattern[j])
					break;
			}
			
			// if j correct characters of source == patternLength => found match
			if(j == patternLength)
			{
				printf("[RABIN-KARP        ] - index: %i\n", i);
			}
		}
		// get hash for next window:
		// 1. remove hash for old first character
		// 2. add hash for new last character
		// like: [AB]CDEF -> A[BC]DEF (remove A hash, add C hash)
		if(i < sourceLength - patternLength)
		{
			// count hash for new "window"
			t = ( d*(t-source[i]*h) + source[i+patternLength] ) % q;
			if(t < 0)
			{
				// make sure "window" hash is non-negative
				t += q;
			}
		}
	}
	
}

void createPrefixTable(char* P, int* pi)
{
	// creates prefixTable
	//
	// using prefixTable we can use knowledge about common prefixes+sufixes
	// to avoid matching already matched subsequences
	int patternLength = strlen(P);
	
	// our table starts from index=0
	// so it has to be '-1'
	pi[0] = -1;
	int k = -1;

	// build prefix table for pattern
	// go over pi[1..patternLength], at [0] value is -1
	for(int i = 1; i < patternLength; i++)
	{		
		// if prefix!=suffix, go over prefix table
		// to find "value" (longest prefix matching suffix)
		while(k > -1 && P[k+1] != P[i])
			k = pi[k];
		// if prefix==suffix, increase "value"
		if(P[k+1] == P[i])
			k++;
		// put that value in prefix table
		pi[i] = k;
	}
}

void KnuthMorrisPratt(char* P, char* T)
{
	int patternLength = strlen(P);
	int sourceLength = strlen(T);
	
	// build prefix table
	int pi[patternLength];
	createPrefixTable(P, pi);
	
	// our table starts from index=0
	// so '0' has to be '-1'
	int k = -1;
	
	// go over source text
	for(int i = 0; i < sourceLength; i++)
	{
		// if we are not at start of prefix table (where value = -1)
		// and current character does not match
		// then go over prefix table by 
		// 1. checking previous entry, 
		// 2. going to that index in prefix table
		// 3. matching current character
		// repeat till matching character is found
		while(k > -1 && P[k+1] != T[i])
			k = pi[k];
		// when matching character is found - go to next character in pattern
		if(P[k+1] == T[i])
			k++;
		// if current index in pattern == pattern length => pattern matched
		if(k == patternLength - 1)
		{
			printf("[KNUTH-MORRIS-PRATT] - index: %i\n", i-k);
			
			// pattern has been matched, but maybe in P[0..k]
			// there was beginning of another pattern, for example starting at P[4]
			// => find this longest matching subpattern in current position
			//
			// concrete example: pattern AAA, source BAAAAB
			// found pattern at B[A]AAAB
			// now dont start from BAAA[A]B, but from BA[A]AAB
			//
			// prefix function table will tell where to start
			k = pi[k];
		}
			
	}
}

void TransitionFunction(char* pattern, int TF[][ALPHABET_LENGTH])
{
	int state, character;
	int pi = 0;
	int patternLength = strlen(pattern);
	
	// set state 0 for all characters (first row)
	for(character = 0; character < ALPHABET_LENGTH; character++)
	{
		TF[0][character] = 0;
	}
	
	// if first character matches => go to state 1
	TF[0][pattern[0]] = 1;
	
	// set states in all other rows
	for(state = 1; state <= patternLength; state++)
	{
		// copy the entries from the row at index equal to prefix function
		// for all possible characters
		for(character = 0; character < ALPHABET_LENGTH; character++)
		{
			// we dont need to start from the beginning
			// instead, use knowledge about common prefixes+sufixes
			// to avoid matching already-matched subsequence again and again
			//
			// prefix function table holds the information where to go next
			// in given state, with given character
			TF[state][character] = TF[pi][character];
		}
		
		// update entry for current character - automata goes to next state
		// in other words: in this state, with this character => go to state+1
		//
		// pattern[state] may be misleading => it is current character, ie. 'C' or 'D'
		// so we set TF[state]['C'], 
		// ie. TF[3]['C'] => when in state 3, and character = 'C', go to next state (4)
		TF[state][pattern[state]] = state + 1;
		
		// update prefix function row index
		// for next transition row to be filled
		// so we will know where to copy from
		if(state < patternLength)
		{
			pi = TF[pi][pattern[state]];
		}
	}
}

void FiniteAutomata(char* pattern, char* source)
{
	int patternLength = strlen(pattern);
	int sourceLength = strlen(source);
	
	// prepare transitions table
	int TF[patternLength+1][ALPHABET_LENGTH];
	TransitionFunction(pattern, TF);
	
	int state = 0;
	
	// use finite automata on source text
	for(int i = 1; i < sourceLength; i++)
	{
		// determine new state to go to
		// currently in state == STATE and current character == SOURCE[i]
		// example: in state 4, current character 'c' => go to state 2 etc
		state = TF[state][source[i]];
		
		// if end state is reached => pattern matched
		if(state == patternLength)
		{		
			printf("[FINITE-AUTOMATA   ] - index: %i\n", i - patternLength + 1);
		}
	}
}

int getFileLength(FILE* file)
{
	rewind(file);
	int fileLength = 0;
	
	char c;
	while( (c = fgetc(file)) != EOF)
	{
		if( c != '\r' && c != '\n' )
			fileLength++;
	}
	return fileLength;	
}

char* readFile(FILE* file)
{	
	// read file length
	rewind(file);
	int characterCount = getFileLength(file);
	
	// malloc mem for file
	char* content = malloc( (characterCount+1) * sizeof(char));
	rewind(file);
	
	int currentCharacter = 0;
	
	// read file to array char by char
	char c;
	while( (c = fgetc(file)) != EOF )
	{

		if( c != '\r' && c != '\n' )
		{
			content[currentCharacter] = c;
			currentCharacter++;
		}
	}
	
	// add terminating char at end of array
	content[characterCount] = '\0';
	
	return content;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Wrong number of parameters!\n");
		return -1;
	}
	
	printf("\n");
	
	FILE* patternFile = fopen(argv[1], "r");
	if(patternFile == NULL)
	{
		printf("Cannot read pattern file!\n");
		return -1;
	}
	
	char* pattern = readFile(patternFile);
	printf("Pattern length: %i \n", getFileLength(patternFile));
	//printf("Pattern content:\n%s\n", pattern); 
	
	
	FILE* sourceFile = fopen(argv[2], "r");
	if(sourceFile == NULL)
	{
		printf("Cannot read source file!\n");
		return -1;
	}

	char* source = readFile(sourceFile);
	printf("Source length: %i \n", getFileLength(sourceFile));
	//printf("Source content:\n%s\n", source); 
	
	printf("\n");
	
	
	clock_t t;
	
    t = clock();
	Oczywisty(pattern, source);
    t = clock() - t;
    long double diff = ((long double)t)/CLOCKS_PER_SEC;
    printf("[OCZYWISTY         ]: %Lf s\n", diff);
	
    t = clock();
	RabinKarp(pattern, source);
    t = clock() - t;
    diff = ((double)t)/CLOCKS_PER_SEC;
    printf("[RABIN-KARP        ]: %Lf s\n", diff);
	
    t = clock();
	KnuthMorrisPratt(pattern, source);
    t = clock() - t;
    diff = ((double)t)/CLOCKS_PER_SEC;
    printf("[KNUTH-MORRIS-PRATT]: %Lf s\n", diff);
	
    t = clock();
	FiniteAutomata(pattern, source);
    t = clock() - t;
    diff = ((double)t)/CLOCKS_PER_SEC;
    printf("[FINITE-AUTOMATA   ]: %Lf s\n", diff);
	
	printf("\n");
	
	return 0;
}