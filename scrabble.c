#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

#define MAX_LEN 50
#define ALPHABET_SIZE 26
#define MIN 1
#define INT_LEN 3
#define ALPHABET "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
    /* Value associated with key (set if end_of_word = 1) */
    void* value;

    /* 1 if current node marks the end of a word, 0 otherwise */
    int end_of_word;

    trie_node_t** children;
    int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
    trie_node_t* root;
    /* Generic Data Structure */
    int data_size;

    /* Number of nodes, useful to test correctness */
    int nNodes;
};

/**
 * Function that creates a node and puts it in the structure
*/ 
trie_node_t* trie_create_node(trie_t * trie, void *value) {

	// Allocating and initialising memory
    trie_node_t *node = calloc(1, sizeof(trie_node_t));
	node->n_children = 0;

    // If a value is given we put it in the 'value' field
    if (value) {
        node->value = malloc(trie->data_size);
        memcpy(node->value, value, trie->data_size);
    } else {
        node->value = NULL;
    }

    // Creating the children array and increasing the total number of nodes
	node->children = calloc(ALPHABET_SIZE, sizeof(trie_node_t *));
	trie->nNodes++;
	return node;
}

/**
 * Function that creates the trie structure and the root
*/
trie_t* trie_create(int data_size) {

	// Allocating memory
    trie_t *trie = malloc(sizeof(trie_t));

	// Initialising fields
	trie->data_size = data_size;
	trie->root = NULL;

	// Creating root
	int special_value = -1;
	trie_node_t *node = trie_create_node(trie, &special_value);
    node->end_of_word = 1;
    trie->root = node;
    return trie;
}

/**
 * Function that inserts a word
*/
void node_insert(trie_t* trie, trie_node_t* node, char* key, void* value)
{
    // If there are no more letters in key, we insert the value
	if (!strlen(key)) {
        if (node->value)
            free(node->value);
        node->value = malloc(trie->data_size);
		memcpy(node->value, value, trie->data_size);
		node->end_of_word = 1;
		return;
	}

    // Taking the first letter of the key
	char first_letter = key[0];
	trie_node_t *next_node = node->children[first_letter - 'a'];

    // If the child node does not exist, then I create it
	if (!next_node) {
        next_node = trie_create_node(trie, NULL);
        node->n_children++;
        node->children[first_letter - 'a'] = next_node;
	}

    // Recursive call
    key++;
	node_insert(trie, next_node, key, value);
}

/**
 * Function that returns a value if the given key is found in trie
*/
void *node_search(trie_t* trie, trie_node_t *node, char* key)
{
    // Key is found, returning value
    if (!strcmp(key, "\0") && node->end_of_word)
        return (int *)node->value;
    // The word itself does not exist, but his chars exist in trie
    // ex: word in trie: 'intro', key: 'in'
    else if (!strcmp(key, "\0") && !node->end_of_word)
        return NULL;

    // If the child does not exist, then the key does not exist
    char first_letter = key[0];
    trie_node_t *next_node = node->children[first_letter - 'a'];
	if (!next_node) {
        return NULL;
    }

    // Recursive call
    key++;
    return node_search(trie, next_node, key); 
}

/**
 * Function that frees the memory for a node
*/
void node_free(trie_node_t *node)
{
    free(node->value);
    free(node->children);
    free(node);
}

/**
 * Function that remove a node from trie
*/
int node_remove(trie_t* trie, trie_node_t *node, char* key)
{
    if (!strcmp(key, "\0")) {
        if (node->end_of_word) {
            node->end_of_word = 0;
            if (node->n_children)
                return 0;
            else
                return 1;
        }
        return 0;
    }

    char first_letter = key[0];
	trie_node_t *next_node = node->children[first_letter - 'a'];
    key++;
    if (next_node) {
        if (node_remove(trie, next_node, key)) {
            node_free(next_node);
            node->children[first_letter - 'a'] = NULL;
            node->n_children--;
            trie->nNodes--;
            if (!node->n_children && !node->end_of_word)
                return 1;
        }
    }
    return 0;
}

/**
 * Function that removes all the nodes in order to free the trie
*/
void remove_all_words(trie_node_t *node)
{    
    if (!node->n_children) {
        node_free(node);
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i]) {
            remove_all_words(node->children[i]);
            node->children[i] = NULL;
            node->n_children--;
        }
    if (!node->n_children)
        node_free(node);
}

/**
 * Function that frees the trie structure
*/
void trie_free(trie_t** pTrie) {
    trie_node_t *root = (*pTrie)->root;
    remove_all_words(root);
    free(*pTrie);
    *pTrie = NULL;
}

/**
 * Error messages for the game
*/
int error_messages(trie_t *trie, char introduced_word[MAX_LEN])
{
    int *value;

    // There are chars that are not in the english alphabet
    for (unsigned int i = 0; i < strlen(introduced_word); i++) 
        if (!strchr(ALPHABET, introduced_word[i])) {
            printf("Please introduce only valid characters.\n\n");
            return -1;
        }

    // Word not found in the trie structure
    value = node_search(trie, trie->root, introduced_word);
    if (!value) {
        printf("Word not found. Please try another.\n\n");
        return -1;
    }
    return 0;
}

/**
 * Function that takes input words from user and verify if 
 * the word is guessed
*/
void solve_anagram(trie_t *trie, char word[MAX_LEN], int no_of_words)
{
    int ok = 0, err;
    char introduced_word[MAX_LEN];
    printf("The search of the mighty word starts here!\n");
    while(!ok) {
        printf("Please introduce a word\n");
        
        // Check for error messages
        scanf("%s", introduced_word);
        err = error_messages(trie, introduced_word);
        if (!err) {

            // If the word is found and there are words to find
            if (!strcmp(word, introduced_word) && no_of_words != 1) {
                printf("Bingo!!!\nPreparing next word...\n\n");
                ok = 1;

            // If the word is found and it is the last one
            } else if (!strcmp(word, introduced_word) && no_of_words == 1) {
                printf("Bingo!!!\nGG!\n");
                ok = 1;

            // If the word is wrong
            } else {
                printf("No luck with this one. Let's try another.\n\n");
            }
        }
    }
}

/**
 * Function that reads a word from a given index (of the file)
*/
void go_to_line(FILE *f_in, int rows, char word[MAX_LEN])
{
    fseek(f_in, INT_LEN, SEEK_SET);
    for (int i = 0; i < rows; i++)
        fscanf(f_in, "%s", word);
}

/**
 * Function that shuffles the letters of a word
*/
void shuffle_word(char* word) {
    int length = strlen(word);
    srand(time(NULL));

    // Shuffling letters
    for (int i = length - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        char temp = word[i];
        word[i] = word[j];
        word[j] = temp;
    }
}

/**
 * Function that generates a scrambled word for the player to guess
*/
void scrambled_anagram(trie_t *trie, FILE *f_in, int rows)
{
    int no_of_words, random_index;
    char word[MAX_LEN], scrambled_word[MAX_LEN];

    printf("Please enter the number of words.\n");
    scanf("%d", &no_of_words);

    while(no_of_words) {

        // Getting the word with a random generated index
        srand(time(NULL));
	    random_index = rand() % (rows - MIN + 1) + MIN;
        go_to_line(f_in, random_index, word);
        strcpy(scrambled_word, word);

        // Shuffle function for the word
        shuffle_word(scrambled_word);
        printf("The scrambled word is: %s\n", scrambled_word);

        // The fun starts here
        solve_anagram(trie, word, no_of_words);
        no_of_words--;
    }

}

/**
 * Function that ends when all of the words with the given prefix are found
*/
void solve_pref_x(trie_t *trie, char **prefix_matrix, int no_of_words)
{
    char introduced_word[MAX_LEN], hint[MAX_LEN];
    int ok = 0, err, guessed_words = 0, found, one, first = 0;
    int *freq_arr = calloc(no_of_words, sizeof(int));
    if (!freq_arr) {
        printf("Calloc failed!\n");
        exit(EXIT_FAILURE);
    }
    while (!ok) {
        if (!first)
            printf("Please introduce a word\n");
        else
            printf("%d more to go. Please introduce a word\n", no_of_words - guessed_words);
        first = 1;

        scanf("%s", introduced_word);

        // Check for errors
        err = error_messages(trie, introduced_word);
        if (!err) {
            found = 0;

            // Checks if a word is found
            for (int i = 0; i < no_of_words; i++)
                if (!strcmp(prefix_matrix[i], introduced_word)) {
                    printf("You found the word: %s. Keep it going!\n\n", prefix_matrix[i]);
                    freq_arr[i]++;
                    guessed_words++;
                    found = 1;
                }
            // No word found
            if (found == 0) {
                printf("Better luck with next guess. You can press 1 if you need a hint\n\n");

                // Hint mechanism
                scanf("%d", &one);
                if (one == 1) {
                    for (int i = 0; i < no_of_words; i++)
                        if (!freq_arr[i]) {
                            strcpy(hint, prefix_matrix[i]);
                            hint[3] = '\0';
                            printf("Psst..Here is your hint: %s. Keep it secret\n\n", hint);
                            break;
                        }
                }

            // All of the words are found
            } else if (guessed_words == no_of_words) {
                ok = 1;
                printf("Divine! You've found all of the words!\n");
            }
        }
    }
    free(freq_arr);
}

/**
 * Function that creates the word matrix. Each word in the matrix start
 * with the given prefix (DFS like)
*/
void words_with_prefix(trie_node_t *node, char word[MAX_LEN], char **prefix_matrix, int *index, int len)
{
    if (!node->end_of_word) {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (node->children[i]) {
                word[len] = i + 'a';
                words_with_prefix(node->children[i], word, prefix_matrix, index, len + 1);
            }
    } else {
        word[len] = '\0';
        strcpy(prefix_matrix[*index], word);
        (*index)++;
    } 
}

/**
 * Function that generates a number word
*/
void get_prefix(FILE *f_in, char prefix[MAX_LEN], int rows)
{
    srand(time(NULL));
	int random_index = rand() % (rows - MIN + 1) + MIN;
    go_to_line(f_in, random_index, prefix);
}


void pref_x(trie_t *trie, FILE *f_in, int rows)
{
    int no_of_words = 0;
    char prefix[MAX_LEN], **prefix_matrix;

    // Get the first 2 letters of a random word
    get_prefix(f_in, prefix, rows);
    prefix[2] = '\0';

    printf("The prefix is: %s\n", prefix);

    // Allocating the word matrix
    prefix_matrix = malloc(MAX_LEN * sizeof(char *));
    if (!prefix_matrix) {
        printf("Malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_LEN; i++) {
        prefix_matrix[i] = malloc(MAX_LEN * sizeof(char));
        if (!prefix_matrix[i]) {
            printf("Malloc failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Creating the prefix and filling the matrix
    trie_node_t *node = trie->root;
    node = node->children[prefix[0] - 'a'];
    node = node->children[prefix[1] - 'a'];
    words_with_prefix(node, prefix, prefix_matrix, &no_of_words, 2);
    printf("You have to guess: %d words. Good luck with that!\n", no_of_words);

    // Fun starts here
    solve_pref_x(trie, prefix_matrix, no_of_words);
    
    for (int i = 0; i < MAX_LEN; i++)
        free(prefix_matrix[i]);
    free(prefix_matrix);
}

/**
 * Rules for the game
*/
void rules_scrabble()
{
    printf("Welcome to \"Scrabble\"!\n");
    printf("There are currently 2 game modes available:\n\n");

    printf("1. Scrambled Anagram.\nYou get a scrambled word and you have\n");
    printf("to form a valid word. You have to introduce the number\n");
    printf("of words that you want to guess\n\n");

    printf("2. Pref_X.\nYou get 2 letters and you have to form all\n");
    printf("of the words that start with those 2 letters\n");
    printf("Press 1 for Scrambled Anagram or 2 for Pref_X\n\n");
}

/**
 * First function of the game
*/
void initialise_scrabble()
{
    char word[MAX_LEN], filename[MAX_LEN] = "words/scrabble.txt";
	int no_of_lines, word_len, game_mode;

    // Creating the trie
	trie_t *trie = trie_create(sizeof(int));

    // Opening the file
	FILE *f_in = fopen(filename, "r");
    if (!f_in) {
		printf("File failed to open!\n");
		exit(EXIT_FAILURE);
	}

    // Reading from the file and inserting the nodes
    fscanf(f_in, "%d", &no_of_lines);
    for (int i = 0; i < no_of_lines; i++) {
        fscanf(f_in, "%s", word);
        word_len = strlen(word);
        node_insert(trie, trie->root, word, &word_len);
    }
    
    rules_scrabble();
    scanf("%d", &game_mode);
    if (game_mode == 1)
        scrambled_anagram(trie, f_in, no_of_lines);
    else
        pref_x(trie, f_in, no_of_lines);

    trie_free(&trie);
    fclose(f_in);
}
