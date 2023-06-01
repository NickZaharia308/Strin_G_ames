#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include "functions.h"


#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)


#define ALPHABET_SIZE 26
#define MAX_STRING_SIZE 50
#define MAX_TRIES 6
#define IMPOSSIBLE -1


int populate(char letter, char *word, char *guess, int* guessed_letters)
{
    int pos; 
    char *p = strchr(word, letter);

    if (!p)
        return 1;

    while (p) {
        // replace in guess each presence of the letter
        *guessed_letters = *guessed_letters + 1;
        pos = p - word;
        guess[pos] = letter;
        p = strchr(p + 1, letter);
    }

    return 0;
}

void print_hangman(int tries)
{
    // the hardcoding of our hangman:)

    printf(" _______\n");
    printf("|/");
    if (tries > 0)
        printf("      |");
    printf("\n");


    printf("|"); 
    if (tries > 1) 
    printf("      (_)");
    printf("\n");

    printf("|");
    if (tries > 2)
        printf("      \\|/");
    printf("\n");

    printf("|");

    if (tries > 3)
        printf("       |");
    printf("\n");

    printf("|");
    if (tries > 4)
        printf("      / \\");

    printf("\n");

    printf("|       \n");
    printf("|___\n");
    printf("\n");
    
    if (tries > 4)
        printf("YOU LOST\n");

}

void start_hangman(char *word)
{   
    // remove the \n that i got from fgets
    word[strlen(word) - 1] = 0;

    int letters =  strlen(word), tries = 0, signal, guess_no = 0;
    char let, trash, first, last;

    //allocate guess
    char *guess = calloc(letters + 1, 1);
    DIE(!guess, "Calloc failed");

    //init guess with '_'
    memset(guess, 95, letters - 1);
    guess[letters] = 0;

    //the first/last letter of the word are the initial hints
    first = word[0];
    last = word[letters - 1];

    // replace '_' with the appropriate letter
    populate(first, word, guess, &guess_no);
    if (first != last)
        populate(last, word, guess, &guess_no);

    //init alphabet
    char alphabet[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        alphabet[i] = 'a' + i;
    }

    // they have already been used
    alphabet[first - 'a'] = IMPOSSIBLE;
    alphabet[last - 'a'] = IMPOSSIBLE;


    while (tries < MAX_TRIES) {

        //bash command to clear the terminal screen
        system("clear");

        //print the hangman, depending on the number of mistakes
        print_hangman(tries);

        if (tries == MAX_TRIES - 1) {
            printf("\nThe word: %s\n\n", word);
            break;
        }
            

        // print the current guess
        for (int i = 0; i < letters; i++)
            printf("%c ", guess[i]);
        
        // if the guess is correct
        if (guess_no == letters) {
            printf("\n\nCONGRATS!\n\n");
            break;
        }     

        //print the available letters
        printf("\n\nAVAILABLE LETTERS: \n\n");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (alphabet[i] != IMPOSSIBLE) {
                printf("%c ", alphabet[i]);
            } else 
                printf("* ");
        }
        printf("\n\n");

        // read a new letter
        printf("INSERT A NEW LETTER: ");
        scanf("%c%c", &trash, &let);
        printf("\n\n");

        // if the read character is not valid
        if (!isalpha(let) || alphabet[let - 'a'] == IMPOSSIBLE)
            continue;

        alphabet[let - 'a'] = IMPOSSIBLE;
        
        // replace '_' with the corressponding letter
        signal = populate(let, word, guess, &guess_no);

        if (signal)
            tries++;
   
    }

   free(guess);
}

void hangman_intro()
{
    char difficulty[MAX_STRING_SIZE], filename[MAX_STRING_SIZE];

    printf("\nWelcome to Hangman!\n\n");
    printf("Please set the difficulty level: EASY / MEDIUM / HARD :\n\n");

    //I choose the file to be opened, depending on the difficulty

    while (1) {
        scanf("%s", difficulty);

        if (strncmp(difficulty, "EASY", 4) == 0) {
            strcpy(filename, "words/easy.txt");
            break;
        } else if (strncmp(difficulty, "MEDIUM", 6) == 0) {
            strcpy(filename, "words/medium.txt");
            break;
        } else if (strncmp(difficulty, "HARD", 4) == 0) {
            strcpy(filename, "words/hard.txt");
            break;
        } else {
            printf("\n");
            printf("Please insert a valid level\n");
        }
    }

    FILE *file = fopen(filename, "r");

    int line_count = 0, current_line = 0;
    char ch;
    char word[MAX_STRING_SIZE];


    line_count = 0;

    //I count the lines of the file in order so that i can get a random one

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }  

    //Get a random line index
    srand(time(NULL));
    int random_line_number = rand() % line_count + 1;

    rewind(file);

    //I retrieve the word from the randomized line
    while (fgets(word, MAX_STRING_SIZE, file) != NULL) {

        current_line++;

        if (current_line == random_line_number)
            break;
    }

    fclose(file);

    //continue with the
    start_hangman(word);
}
