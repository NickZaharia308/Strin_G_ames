# Read the file
with open('top_1000.txt', 'r') as file:
    content = file.read()

# Split the content into words
words = content.split()

# Filter words with different lengths
easy_words = [word for word in words if 4 <= len(word) <= 5]
medium_words = [word for word in words if 6 <= len(word) <= 9]
hard_words = [word for word in words if 10 <= len(word) <= 11]

# Sort the filtered words by the number of letters and then lexicographically
easy_sorted_words = sorted(easy_words, key=lambda w: (len(w), w))
medium_sorted_words = sorted(medium_words, key=lambda w: (len(w), w))
hard_sorted_words = sorted(hard_words, key=lambda w: (len(w), w))

# Write the sorted words to the respective files
with open('easy.txt', 'w') as file:
    file.write('\n'.join(easy_sorted_words))

with open('medium.txt', 'w') as file:
    file.write('\n'.join(medium_sorted_words))

with open('hard.txt', 'w') as file:
    file.write('\n'.join(hard_sorted_words))

