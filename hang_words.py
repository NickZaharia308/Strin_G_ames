def sort_words_by_length(input_file, output_file, x, y):
    # Read the input file
    with open(input_file, 'r') as file:
        content = file.read()

    # Split the content into words
    words = content.split()

    # Filter words with lengths between x and y (inclusive)
    filtered_words = [word for word in words if x <= len(word) <= y]

    # Sort the filtered words by the number of letters and then lexicographically
    sorted_words = sorted(filtered_words, key=lambda w: (len(w), w))

    # Write the sorted words to the output file
    with open(output_file, 'w') as file:
        file.write('\n'.join(sorted_words))
