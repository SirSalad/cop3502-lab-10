#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct Trie *root;
    int numOccurances;
    int numChildren;
    struct Trie **children;
    char character;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = NULL;
    pTrie->numOccurances = 0;
    pTrie->numChildren = 0;
    pTrie->children = NULL;
    pTrie->character = '\0';
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL || word == NULL) return;

    struct Trie* node = pTrie;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        char c = word[i];
        struct Trie* child = NULL;
        for (int j = 0; j < node->numChildren; j++)
        {
            if (node->children[j]->character == c)
            {
                child = node->children[j];  
                break;
            }
        }

        if (child == NULL)
        {
            child = createTrie();

            node->numChildren++;
            node->children = (struct Trie**)realloc(node->children, sizeof(struct Trie*) * node->numChildren);
            node->children[node->numChildren - 1] = child;
        }

        node = child;
    }
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL || word == NULL) return 0;
    int len = strlen(word);
    struct Trie* node = pTrie;
    for (int i = 0; i < len; i++) 
    {
        char c = word[i];
        struct Trie* child = NULL;

        for (int j = 0; j < node->numChildren; j++)
        {
            if (node->children[j]->character == c)
            {
                child = node->children[j];
                break;
            }
        }

        if (child == NULL) return 0;
        node = child;
    }
    return node->numOccurances;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) return NULL;
    for (int i=0;i<pTrie->numChildren;i++)
    {
        pTrie->children[i] = deallocateTrie(pTrie->children[i]);
    }
    free(pTrie->children);
    free(pTrie);
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;
    
    int numWords = 0;
    char line[256];
    while (fgets(line, 256, fp) != NULL)
    {
        line[strlen(line)-1] = '\0';
        pInWords[numWords] = (char *)malloc(sizeof(char)*(strlen(line)+1));
        strcpy(pInWords[numWords], line);
        numWords++;
    }
    fclose(fp);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}