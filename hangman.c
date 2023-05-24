#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)



#define MAX_STRING_SIZE 50
#define MAX_TRIES 6

int from_char_to_int(char *s)
{
    int len = strlen(s), num = 0;

    for (int i = 0 ; i < len; i++)
        num = num * 10 + (s[i] - '0');

    return num;
}

int populate(char letter, char *word, char *guess, int* guessed_words)
{
    int pos;
    char *p = strchr(word, letter);

    if (!p)
        return 1;

    while (p) {
        *guessed_words = *guessed_words + 1;
        pos = p - word;
        guess[pos] = letter;
        p = strchr(p + 1, letter);
    }

    return 0;
}

void print_hangman(int tries)
{
        
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
    printf("\n\n");
    
    if (tries > 4)
        printf("YOU LOST\n");

}

void start_hangman(char *word)
{   
    //scap de \n
    word[strlen(word) - 1] = 0;

    int letters =  strlen(word), tries = 0, signal, guess_no = 0;
    char let, trash, first, last;

    //init guess
    char *guess = calloc(letters, 1);
    DIE(!guess, "Calloc failed");
    memset(guess, 95, letters - 1); //initializez cu _
    guess[letters] = 0;

    first = word[0];
    last = word[letters - 1];


    populate(first, word, guess, &guess_no);
    if (first != last)
        populate(last, word, guess, &guess_no);

    //init alphabet
    char alphabet[26];
    for (int i = 0; i < 26; i++) {
        alphabet[i] = 'a' + i;
    }

    alphabet[first - 'a'] = 2;
    alphabet[last - 'a'] = 2;
   

    system("clear");

    while (tries < 6) {

        system("clear");

       // printf("\n\nguess: %d\n", guess_no); 
        print_hangman(tries);

        if (tries == 5)
            break;

        printf("\nWORD: %s\n", word);
        for (size_t i = 0; i < strlen(guess); i++)
            printf("%c ", guess[i]);

        if (guess_no == letters) {
            printf("\n\nCONGRATS!\n");
            break;
        }     

        printf("\n\nAVAILABLE LETTERS: \n\n");
        for (int i = 0; i < 26; i++) {
            if (alphabet[i] != 2) {
                printf("%c ", alphabet[i]);
            } else 
                printf("* ");
        }
        printf("\n\n");

        
            printf("\n\nINSERT A NEW LETTER: ");
            scanf("%c%c", &trash, &let);
            printf("LET: %c\n", let);
            printf("\n\n");

            if (alphabet[let - 'a'] == 2)
                continue;

        alphabet[let - 'a'] = 2;
        
        signal = populate(let, word, guess, &guess_no);

        if (signal)
            tries++;
   
    }

   free(guess);
}



void intro()
{
    char difficulty[MAX_STRING_SIZE];
    int dif, x, y;

    printf("\nWelcome to Hangman!\n\n");
    printf("Please set the difficulty level: 1 / 2 / 3 :\n\n");


    while (1) {
        scanf("%s", difficulty);
        printf("\n");

        dif = from_char_to_int(difficulty);
        if (dif < 1 || dif > 3) {
            printf("Please insert a valid level\n");
        } else 
            break;
    }

    // if (dif == 1) {
    //     x = 3;
    //     y = 5;
    // } else if (dif == 2) {
    //     x = 6;
    //     y = 8;
    // } else {
    //     x = 9;
    //     y = 11;
    // }

   // printf("%d %d\n", x, y);


    FILE *file = fopen("number_sorted.txt", "r");

    int line_count = 0;
    char ch;
    char word[MAX_STRING_SIZE];

    fgets(word, sizeof(word), file);
    word[strlen(word) - 1] = 0;

    line_count = from_char_to_int(word);
    

    srand(time(NULL));
    int random_line_number = rand() % line_count + 1;


    //rewind(file);
    int current_line = 0;
   

    while (fgets(word, sizeof(word), file) != NULL) {
        current_line++;
        if (current_line == random_line_number) {
            
            printf("Random Word: %s", word);
            break;
        }
    }


    //printf("LINE %d\n", random_line_number);

    fclose(file);

   // strcpy(word, "excersise\n");

    start_hangman(word);
    

}

int main()
{
    intro();
    return 0;
}