# Open the input file containing all English words
with open('top_1000.txt', 'r') as input_file:
    # Read all the lines from the input file
    lines = input_file.readlines()

# Filter the words with five letters
five_letter_words = [word.strip() for word in lines if len(word.strip()) == 5]

five_letter_words = sorted(five_letter_words)

# Open the output file to write the filtered words
with open('ceva.txt', 'w') as output_file:
    # Write each filtered word to the output file
    for word in five_letter_words:
        output_file.write(word + '\n')

print('Filtered words with five letters written to ceva.txt.')
