#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#define MAX_STRING_SIZE 256


void print_logo()
{

     char *asciiArt = "  ______________________________.___ _______        ________        _____      _____  ___________ _________\n"
                     " /   _____/\\__    ___/\\______   \\   |\\      \\      /  _____/       /  _  \\    /     \\ \\_   _____//   _____/\n"
                     " \\_____  \\   |    |    |       _/   |/   |   \\    /   \\  ___      /  /_\\  \\  /  \\ /  \\ |    __)_ \\_____  \\ \n"
                     " /        \\  |    |    |    |   \\   /    |    \\   \\    \\_\\  \\    /    |    \\/    Y    \\|        \\/        \\\n"
                     "/_______  /  |____|    |____|_  /___\\____|__  /____\\______  /____\\____|__  /\\____|__  /_______  /_______  /\n"
                     "        \\/                    \\/            \\/_____/      \\/_____/       \\/         \\/        \\/        \\/";

    printf("%s\n\n", asciiArt); 

}

void select_minigame()
{
    char command[MAX_STRING_SIZE];
    print_logo();
    printf("Hello there! Please select the minigame: WORDLE / HANGMAN / SCRABBLE:\n\n");
    fgets(command, MAX_STRING_SIZE, stdin);
	if (strncmp(command, "WORDLE", 6) == 0) {
	    initialise_wordle();
	} else if (strncmp(command, "HANGMAN", 7) == 0) {
	    hangman_intro();
	} else if (strncmp(command, "SCRABBLE", 8) == 0) {
	    initialise_scrabble();
	} else if (strncmp(command, "EXIT", 4) == 0) {
	    exit(0);
	}

}

int main()
{
    select_minigame();   
    return 0;
}
