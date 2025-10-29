# import requests
# import re
# from collections import Counter
# import matplotlib.pyplot as plt

# # -------------------------------
# # Step 1: Get content from Wikipedia
# # -------------------------------
# def get_content(article_name):
#     """
#     Fetch Wikipedia article content using the MediaWiki API.
#     """
#     url = "https://en.wikipedia.org/w/api.php"
#     params = {
#         "action": "query",
#         "format": "json",
#         "prop": "revisions",
#         "rvslots": "main",
#         "rvprop": "content",
#         "titles": article_name,
#         "redirects": 1
#     }
#     headers = {
#         "User-Agent": "MrCleanProjectBot/1.0 (your_email@example.com)"
#     }
#     response = requests.get(url, params=params, headers=headers)
#     if response.status_code != 200:
#         raise Exception(f"Failed to retrieve data: {response.status_code}")
#     return response.json()

# # -------------------------------
# # Step 2: Clean text
# # -------------------------------
# def clean_text(text):
#     """
#     Cleans the raw text by removing URLs, wiki markup, and special characters.
#     """
#     text = re.sub(r"http\S+|www\S+", "", text)  # Remove URLs
#     text = re.sub(r"\[\[.*?\]\]", " ", text)    # Remove [[WikiLinks]]
#     text = re.sub(r"{{.*?}}", " ", text)        # Remove {{templates}}
#     text = re.sub(r"<.*?>", " ", text)          # Remove HTML tags
#     text = re.sub(r"[^\w\s-]", " ", text)       # Keep letters, numbers, dash
#     text = re.sub(r"\s+", " ", text)            # Remove extra spaces
#     return text.strip()

# # -------------------------------
# # Step 3: Merge contents
# # -------------------------------
# def merge_contents(data):
#     """
#     Extract and clean text from the Wikipedia API response.
#     """
#     pages = data.get("query", {}).get("pages", {})
#     for page in pages.values():
#         try:
#             # Some articles store content under empty string key
#             text = page["revisions"][0]["slots"]["main"].get("", "")
#             if text:
#                 return clean_text(text)
#         except (KeyError, IndexError):
#             continue
#     return ""  # Return empty string if no content

# # -------------------------------
# # Step 4: Tokenize
# # -------------------------------
# def tokenize(content):
#     """
#     Tokenize text into words by splitting on whitespace and dashes.
#     """
#     content = re.sub(r"[^a-zA-Z0-9\-]", " ", content)
#     tokens = re.split(r"[\s\-]+", content)
#     return [token for token in tokens if token]

# # -------------------------------
# # Step 5: Lowercase collection
# # -------------------------------
# def lower_collection(collection):
#     """
#     Convert all tokens in the collection to lowercase.
#     """
#     return [token.lower() for token in collection]

# # -------------------------------
# # Step 6: Remove stop words
# # -------------------------------
# stop_words = set([
#     "the", "a", "an", "and", "or", "of", "to", "in", "on", "for", "with",
#     "by", "is", "are", "was", "were", "that", "this", "as", "at", "it",
#     "be", "from", "which", "but", "has", "had", "have", "its", "not",
#     "also", "s"  # Remove possessive artifacts
# ])

# def remove_stop_words(tokens):
#     return [token for token in tokens if token not in stop_words]

# # -------------------------------
# # Step 7: Count frequency
# # -------------------------------
# def count_frequency(tokens):
#     return Counter(tokens)

# # -------------------------------
# # Step 8: Plot most frequent
# # -------------------------------
# def plot_most_frequent(frequencies, n):
#     most_common = frequencies.most_common(n)
#     if not most_common:
#         print("No data to plot.")
#         return
#     words, counts = zip(*most_common)
#     plt.figure(figsize=(12,6))
#     plt.bar(words, counts)
#     plt.title(f"Top {n} Most Frequent Words")
#     plt.xticks(rotation=45)
#     plt.show()

# # -------------------------------
# # Main Execution
# # -------------------------------
# if __name__ == "__main__":
#     article_name = "Ozone_layer"
    
#     data = get_content(article_name)
#     merged_content = merge_contents(data)
    
#     if not merged_content:
#         raise Exception("Failed to retrieve or merge article content.")

#     tokens = tokenize(merged_content)
#     tokens = lower_collection(tokens)
#     tokens = remove_stop_words(tokens)

#     frequencies = count_frequency(tokens)
    
#     print("Top 10 most frequent words:")
#     for word, count in frequencies.most_common(10):
#         print(f"{word}: {count}")
    
#     plot_most_frequent(frequencies, 25)



import requests
import re
from collections import Counter
import matplotlib.pyplot as plt

# -------------------------------
# Step 1: Get content from Wikipedia
# -------------------------------
def get_content(article_name):
    """
    Fetch Wikipedia article content using the MediaWiki API.
    """
    url = "https://en.wikipedia.org/w/api.php"
    params = {
        "action": "query",
        "format": "json",
        "prop": "revisions",
        "rvslots": "main",
        "rvprop": "content",
        "titles": article_name,
        "redirects": 1
    }
    headers = {
        "User-Agent": "MrCleanProjectBot/1.0 (your_email@example.com)"
    }
    response = requests.get(url, params=params, headers=headers)
    if response.status_code != 200:
        raise Exception(f"Failed to retrieve data: {response.status_code}")
    return response.json()

# -------------------------------
# Step 2: Clean text
# -------------------------------
def clean_text(text):
    """
    Cleans the raw text by removing URLs, wiki markup, and special characters.
    """
    text = re.sub(r"http\S+|www\S+", "", text)  # Remove URLs
    text = re.sub(r"\[\[.*?\]\]", " ", text)    # Remove [[WikiLinks]]
    text = re.sub(r"{{.*?}}", " ", text)        # Remove {{templates}}
    text = re.sub(r"<.*?>", " ", text)          # Remove HTML tags
    text = re.sub(r"[^\w\s-]", " ", text)       # Keep letters, numbers, dash
    text = re.sub(r"\s+", " ", text)            # Remove extra spaces
    return text.strip()

# -------------------------------
# Step 3: Merge contents
# -------------------------------
def merge_contents(data):
    """
    Extract and clean text from the Wikipedia API response.
    """
    pages = data.get("query", {}).get("pages", {})
    for page in pages.values():
        try:
            # Try empty string key first, fallback to "*" key
            text = page["revisions"][0]["slots"]["main"].get("", "")
            if not text:
                text = page["revisions"][0]["slots"]["main"].get("*", "")
            if text:
                return clean_text(text)
        except (KeyError, IndexError):
            continue
    return ""  # Return empty string if no content

# -------------------------------
# Step 4: Tokenize
# -------------------------------
def tokenize(content):
    """
    Tokenize text into words by splitting on whitespace and dashes.
    """
    content = re.sub(r"[^a-zA-Z0-9\-]", " ", content)
    tokens = re.split(r"[\s\-]+", content)
    return [token for token in tokens if token]

# -------------------------------
# Step 5: Lowercase collection
# -------------------------------
def lower_collection(collection):
    """
    Convert all tokens in the collection to lowercase.
    """
    return [token.lower() for token in collection]

# -------------------------------
# Step 6: Remove stop words
# -------------------------------
stop_words = set([
    "the", "a", "an", "and", "or", "of", "to", "in", "on", "for", "with",
    "by", "is", "are", "was", "were", "that", "this", "as", "at", "it",
    "be", "from", "which", "but", "has", "had", "have", "its", "not",
    "also", "s"  # Remove possessive artifacts
])

def remove_stop_words(tokens):
    return [token for token in tokens if token not in stop_words]

# -------------------------------
# Step 7: Count frequency
# -------------------------------
def count_frequency(tokens):
    return Counter(tokens)

# -------------------------------
# Step 8: Plot most frequent
# -------------------------------
def plot_most_frequent(frequencies, n):
    most_common = frequencies.most_common(n)
    if not most_common:
        print("No data to plot.")
        return
    words, counts = zip(*most_common)
    plt.figure(figsize=(12,6))
    plt.bar(words, counts)
    plt.title(f"Top {n} Most Frequent Words")
    plt.xticks(rotation=45)
    plt.show()

# -------------------------------
# Main Execution
# -------------------------------
if __name__ == "__main__":
    article_name = "Ozone_layer"
    
    data = get_content(article_name)
    merged_content = merge_contents(data)
    
    if not merged_content:
        raise Exception("Failed to retrieve or merge article content.")

    tokens = tokenize(merged_content)
    tokens = lower_collection(tokens)
    tokens = remove_stop_words(tokens)

    frequencies = count_frequency(tokens)
    
    print("Top 10 most frequent words:")
    for word, count in frequencies.most_common(10):
        print(f"{word}: {count}")
    
    plot_most_frequent(frequencies, 25)
