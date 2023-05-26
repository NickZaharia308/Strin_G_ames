# Read the file
with open('top_1000.txt', 'r') as file:
    content = file.read()

# Split the content into words
words = content.split()

# Filter words with different lengths
scrabble_words = [word for word in words if 4 <= len(word) <= 9]

# Sort the filtered words by the number of letters and then lexicographically
sorted_scrabble_words = sorted(scrabble_words, key=lambda w: (len(w), w))

# Write the sorted words to the respective files
with open('scrabble.txt', 'w') as file:
    file.write('\n'.join(sorted_scrabble_words))


