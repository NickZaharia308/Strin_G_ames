# **Strin_G_ames**
Copyright 2023 Alexandru-Andrei CRET, Nicusor-Alexandru ZAHARIA - 315CA

## **Description**

Our project consists of three popular word-based minigames in one app, using the concepts we learned during the **PCLP3** course: 


1.  __[Hangman](https://en.wikipedia.org/wiki/Hangman_(game))__: a guessing game where the player tries to guess a hidden word by suggesting letters. Each incorrect guess leads to the gradual drawing of a hangman's gallows. Players win by guessing the word before the complete hangman figure is drawn, providing an engaging test of vocabulary and deduction skills.
2. __[Wordle](https://www.nytimes.com/games/wordle/index.html)__: a minigame where the player attempts to guess a five-letter word within six attempts. After each guess, the game provides feedback on correct letters and their positions. It challenges players' word-guessing skills and deductive reasoning, making it a fun and addictive word puzzle.
3. __[Scrabble](https://dexonline.ro/omleta)__ : a minigame that can be played in two modes: given a word whose letters are shuffled, the player has to figure it out or for a given prefix the player has to find as many correct words as possible.

## **Interface**

In order to maintain a modular character, our project has a main interface from where the user can choose the prefered minigame. The **main.c** file is intended to be as clean as possible, with 2 helper-functions:

+ **print_logo**: it prints on the terminal's screen the project's title, using __[ASCII art](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=STRIN_G_AMES)__ 

+ **select_minigame**: a function that parses the input given by the user and launches the chosen minigame. **EXIT** command is available for a smooth exit.

The code for each minigame is stored in a separate **.c** file, which are interconnected through **functions.h**, which uses preprocessing directives such as **#ifndef** and **#endif** for conditional compilation. After any minigame is over, the user is redirected back to **select_minigame**.


## **Hangman**

This minigame has three difficulty levels (EASY/MEDIUM/HARD), so that the user can choose between a shorter or a larger word to guess. The implementation consists of different functions: 

+ **hangman_intro**: it reads the difficulty level from the user and loads the corresponding word file. The game must start with a random word, so we chose to use **rand** from **time.h** to determine a random line_index (and thus a random word). The number of lines are found by counting each line in the file.
```C
    srand(time(NULL));
    int random_line_number = rand() % line_count + 1;
```

 
+ **start_hangman**: once the random word is determined, the game can start. We take a string **guess** (allocated in HEAP, we used the macrodefinition **DIE** for defensive programming) in which the guessed letters are stored, alongside with an **alphabet** so that the user always knows which letters are still available. As at the start of each game the first and last letter are already known, we replace the '_' corresponding characters by calling the function **populate**. Moreover, there is a loop which takes care of the number of mistakes. The instruction **system("clear")**' s role is to clear and refresh the terminal screen so that the game looks smoother. At every "iteration" the current hangman stage is printed alongside the current guess and the available letters. Whether the player has won or not, the application will return to its main interface, enabling the possibility to play another minigame.

+ **populate**: this function searches for every occurence of a certain letter and replaces the '_' character on the corresponding position: 

```C
    while (p) {
        *guessed_letters = *guessed_letters + 1;
        pos = p - word;
        guess[pos] = letter;
        p = strchr(p + 1, letter);
    }
```

+ **print_hangman**: this function prints the ASCII representation of the hangman, based on the number of mistakes: 

```
    _______
    |/    |
    |    (_)
    |    \|/
    |     |
    |    / \
    |
   _|___
```

## **Wordle**

The idea of the game is to find a random generated word by giving as input words that have letters in common with the generated word.
The game starts in **initialise_wordle()** function. This function opens the given file (for this game we use a file that we generated and it has only words of five letters). We read the number of lines and then we generate the word by moving in the file to the given line.
```C
offset = INT_LEN + (random_index - 1) * (no_of_chars + 1);
fseek(f_in, offset, SEEK_SET);
fscanf(f_in, "%s", word);
```

After that, we create a matrix that contains all the words from the file. The function calls **rules_wordle()** that shows the rules of the game and then enters in the **tries()** function.

In **tries()** function the player is asked to introduce words. As in the original game, the player has six tries. After each word, the function calls **err_messages()**, but we will come back to it later. After that, we create an array named 'alphabet' that stores the occurence of each letter in the original word, so that we can compare the letters of word that has to be found and the word that the player enters. When comparing the letters we check three cases:
1. If the letters match and the position is the same; example: 'a' in 'above' and 'after'
2. If the letters match, but the position is different; example: 'd' in 'road' and 'done' (In the first word letter 'd' appears as the last one, but in the second word appears as the first one)
3. If the letters mismatch; example: 'dog' and 'cat' have no letters in common
If all of the letters are in the first case, the game is won.


The **err_messages()** function checks if the input word is not valid and by that we have:
1. The player introduces other characters than the english alphabet letters
2. The players's word and the word that is searched have different number of letters
3. The word is not found in the matrix. We use a binary search function (a concept that was presented in the laboratory) called **binary_search_word_matrix()** that searches for a word in the matrix in an efficient way and it looks like this:

```C
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
```

After six tries (or less if the word is found) the game ends, so good luck!

**Concepts:** binary search functions, dynamically allocated arrays and matrices, define


## **Scrabble**

This minigame has two game modes:
1. "Scrambled Anagram", where you get a scrambled word and you have to guess the original word.
2. "Pref_X", where you get a prefix (two letters) and you have to guess all of the words that starts with the given prefix.

The game starts in **initialise_scrabble()** function. It creates the trie structure (concept presented in the laboratory) and opens the file (generated, of course, with our little helpers, the **python scripts**). We put all the words in the trie and then the rules of the game are displayed. From there, the player is asked to choose between the first minigame and the second one.

**NOTE:** The file also has the trie functions that help us work in an efficient way with the words.

+ **Scrambled Anagram**:
The players is asked to enter the number of anagrams that have to be guessed. A random word is generated in a similar way as in the previous games and then we shuffle it:
```C
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
```
After that, the scrambled/shuffled word is displayed and the player has to guess it.

The program continues in **solve_anagram()** where it reads the words of the player and compares it with the original word. All of that is done after the word is valid (errors are verified). After guessing all the words, the player wins.

+ **Pref_X**:
We generate a number and we take the first two letters of it. Now we have a valid prefix. Now we put all the words from trie that start with those 2 letters in a word matrix using **words_with_prefix()** function, that is a complex version of the **DFS** algorithm and it looks like this:
```C
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
```

Now, the player has to guess all the words. Depending on how lucky you are , you can have a few words with the given prefix or a LOT of words that start with the given prefix. As in the previous game mode, errors are verified and then we check if the given word it is between those in the matrix. To know which words have been found and which have not, we have a frequency array. If the player gets stuck, a free offer for a hint is showed. When the player finds all the words the game is finally won.

**Concepts:** Trie, pointers to structures


## **Word proccessing**

We chose to use the english dictionary for this project, as there were not many alternatives for romanian. We found a top of __[the most common english words](https://github.com/powerlanguage/word-lists/blob/master/1000-most-common-words.txt)__ which we proccessed with Python programming language. Our scripts are: 

+ **process_hang_words.py** for Hangman: due to the existence of three difficulty levels, we chose to create three separated word files (**easy.txt**, **medium.txt**, **hard.txt**) whose words are sorted both lexicographically and by the number of letters: 
```Python
# Filter words with different lengths
easy_words = [word for word in words if 4 <= len(word) <= 5]
# Sort the filtered words by the number of letters and then lexicographically
easy_sorted_words = sorted(easy_words, key=lambda w: (len(w), w))
# Write the sorted words to the respective files
with open('easy.txt', 'w') as file:
    file.write('\n'.join(easy_sorted_words))
```

+ **5-words.py** for Wordle: this script outputs all the words which have exactly 5 letters. This is similar to **process_hang_words**.

+ **prepare_for_scrabble.py**: this script extracts the words whose number of letters are between 4 and 9

## **Conclusions**

### What we learned:
 The C programming language is more versatile than we thought. We definitely deepened our knowledge in developing a complex project, which uses two programming languages in its structure.

### Difficulties:
It goes without saying that anything seems to be hard in the beginning, but we managed to cross all the milestones. It took some time to decide the project's structure, but we are satisfied with the result. Manipulating strings with **Python** was, indeed, a challenge because it required a significant amount of research. Moreover, creating a functional and efficient **Makefile** was not as easy as expected. **Scrabble** raised some problems due to the fact that there were many mismatching errors to be handled. As it used a **Trie** in its implementation, there have been some struggles with the **Depth First Search** and remove functions.

### Why should one consider our project useful:
We believe that **Strin_G_ames** could make nice teaching material for programming beginners, especially for those willing to learn about manipulating strings in **C** and **Python**. Moreover, we **TRULY** believe that it represents a fun way to spend your free time 😄 

### Workload distribution:

+ **Andrei**: Word Proccessing, Hangman, ASCII art, 1/2 README
+ **Nicusor**: Wordle, Scrabble, main function and Makefile, 1/2 README
