#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"


#define MAX_LEN 50
#define ALPHABET_LEN 26
#define MIN 1
#define INT_LEN 3
#define NO_OF_TRIES 6
#define ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


/**
 * Function that prints the rules of the game.
 * 'n' is the number of letters of the random word.
*/
void rules_wordle(int n)
{
	printf("Welcome to the \"WORDLE\" game!\n");

	printf("The word you have to guess has %d letters.\n", n);

	printf("If a letter has 0 underneath, it means it is not in the word.\n");

	printf("If a letter has 1 underneath, it means it is in the word,\n");
	printf("but in the wrong position.\n");

	printf("If a letter has 2 underneath, it means it is in the word,\n");
	printf("in the correct position.\n");

	printf("NOTE: If there are repeated letters in the target word, for\n");
	printf("each repetition,0, 1, or 2 will be displayed depending on the case.\n");
}

/**
 * Function that searchs if the input word is in the matrix,
 * using binary search
*/
int binary_search_word_matrix(char **matrix, int rows, char* target) {
    int left = 0;
    int right = rows;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (!strcmp(matrix[mid], target))
            return mid;
        else if (strcmp(matrix[mid], target) < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    // Word not found
    return -1;
}

/**
 * Function that prints error messages
*/
int err_messages(char *input, char *word, char **word_matrix, int no_of_lines)
{
	// Checking if the given characters are from the alphabet
	for (unsigned int i = 0; i < strlen(input); i++) 
		if (!strchr(ALPHABET, input[i])) {
			printf("Please introduce only valid characters.\n");
			return -1;
		}

	// Checking if the size match
	if (strlen(input) > strlen(word)) {
		printf("Word's length is greater that the length of the searched word.\n");
		return -1;
	} else if (strlen(input) < strlen(word)) {
		printf("Word's length is shorter that the length of the searched word.\n");
		return -1;
	}

	// Checking if the word exists in the matrix
	int it_exits = binary_search_word_matrix(word_matrix, no_of_lines, input);
	if (it_exits < 0) {
		printf("Word not found in the list. Please try another.\n");
		return -1;
	}

	return 0;
}

void tries(char word[MAX_LEN], char **word_matrix, int no_of_lines) {
	int no_of_tries = NO_OF_TRIES, ok = 0;
	char *input = malloc(sizeof(char) * MAX_LEN);

	printf("Start to introduce words.\n");
	while (no_of_tries && !ok) {
		scanf("%s", input);
		int *alphabet = calloc(ALPHABET_LEN, sizeof(int));
		int is_error = err_messages(input, word, word_matrix, no_of_lines);
		if (!is_error) {
			unsigned int matching_letters = 0;

			// Makes all the letters lowercase
			for (unsigned int i = 0; i < strlen(word); i++) {
				if (word[i] >= 'A' && word[i] <= 'Z')
					word[i] += 32;
				if (input[i] >= 'A' && input[i] <= 'Z')
					input[i] += 32;
			}

			// Incrementing the letter's occurrence count
			for (unsigned int i = 0; i < strlen(word); i++)
					alphabet[word[i]-'a']++;
			
			// Checking how many letters match
			for (unsigned int i = 0; i < strlen(word); i++) {

				// If the position and the letter is correct
				if (word[i] == input[i]) {
					printf("2");
					alphabet[input[i]-'a']--;
					matching_letters++;
				} else {

					// If only the letter is correct (exists in the word)
					if (alphabet[input[i]-'a']) {
						alphabet[input[i]-'a']--;
						printf("1");
					
					// The letter does not exist in the word
					} else {
						printf("0");
					}
				}
			}
			printf("\n");

			// If all the words match
			if (matching_letters == strlen(word))
				ok = 1;
		}
		free(alphabet);
		no_of_tries--;
	}
	if (ok == 1)
		printf("Congrats! You have found the word: %s\n", word);
	else
		printf("No luck today! The word was: %s\n", word);

	free(input);
}

void initialise_wordle()
{
	char word[MAX_LEN], filename[MAX_LEN], first_word[MAX_LEN], **word_matrix;
	int no_of_lines, no_of_chars, offset, random_index, i;

	// Opening file
	strcpy(filename, "words/5_letter.txt");
	FILE *f_in = fopen(filename, "r");
	if (!f_in) {
		printf("File failed to open!\n");
		exit(EXIT_FAILURE);
	}

	// On the first line, we have number of rows in the file
	fscanf(f_in, "%d", &no_of_lines);

	/* On the second line, we have the first word. We read it so that
	   we can find the lenght of the words */
	fscanf(f_in, "%s", first_word);
	no_of_chars = strlen(first_word);

	// Generate a random number
	srand(time(NULL));
	random_index = rand() % (no_of_lines - MIN + 1) + MIN;
	offset = INT_LEN + (random_index - 1) * (no_of_chars + 1);

	// I move the file pointer to the desired position and read the word
	fseek(f_in, offset, SEEK_SET);
	fscanf(f_in, "%s", word);

	// Creating the word matrix
	word_matrix = malloc(no_of_lines * sizeof(char *));
	if (!word_matrix) {
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < no_of_lines; i++) {
		word_matrix[i] = malloc((no_of_chars + 1) * sizeof(char));
		if (!word_matrix[i]) {
			printf("Malloc failed!\n");
			exit(EXIT_FAILURE);
		}
	}

	// Reading all of the words in matrix
	i = 0;
	fseek(f_in, INT_LEN, SEEK_SET);
	for (i = 0; i < no_of_lines; i++)
		fscanf(f_in, "%s", word_matrix[i]);
		
	// Show the rules and enter the game
	rules_wordle(no_of_chars);
	tries(word, word_matrix, no_of_lines);

	// Freeing matrix memory and closing file
	for (i = 0; i < no_of_lines; i++)
		free(word_matrix[i]);
	free(word_matrix);
	fclose(f_in);
}
