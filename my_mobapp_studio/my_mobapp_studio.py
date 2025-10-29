# Google Play Store Analysis for My MobApp Studio
# Author: Your Name
# Date: 2025-10-27

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pandas.plotting import scatter_matrix

# -------------------------
# 1. Load dataset
# -------------------------
def load_dataset(path):
    """
    Load the Google Play Store dataset.
    
    Args:
        path (str): Path to the CSV file.
        
    Returns:
        pd.DataFrame
    """
    df = pd.read_csv(path)
    return df

# -------------------------
# 2. Clean dataset
# -------------------------
def clean_dataset(df):
    """
    Clean and preprocess the dataset.
    
    Steps:
    - Remove missing values in critical columns
    - Convert 'Reviews', 'Price', 'Installs' to numeric
    - Clean text columns
    """
    # Drop rows where Category or App is missing
    df = df.dropna(subset=['Category', 'App'])

    # Clean Installs
    df['Installs'] = df['Installs'].str.replace(',', '').str.replace('+', '')
    df['Installs'] = pd.to_numeric(df['Installs'], errors='coerce')

    # Clean Price
    df['Price'] = df['Price'].str.replace('$', '')
    df['Price'] = pd.to_numeric(df['Price'], errors='coerce')

    # Convert Reviews to numeric
    df['Reviews'] = pd.to_numeric(df['Reviews'], errors='coerce')

    # Fill missing numeric values with 0
    df[['Installs', 'Price', 'Reviews']] = df[['Installs', 'Price', 'Reviews']].fillna(0)

    return df

# -------------------------
# 3. Summarize dataset
# -------------------------
def print_summarize_dataset(df):
    print("Dataset Summary:")
    print(df.info())
    print("\nTop 5 Rows:")
    print(df.head())
    print("\nCategory Counts:")
    print(df['Category'].value_counts())

# -------------------------
# 4. Plot histograms
# -------------------------
def print_histograms(df):
    df['Price'].hist(bins=50)
    plt.title('Price Distribution')
    plt.xlabel('Price ($)')
    plt.ylabel('Frequency')
    plt.show()

    df['Installs'].hist(bins=50)
    plt.title('Installations Distribution')
    plt.xlabel('Number of Installs')
    plt.ylabel('Frequency')
    plt.show()

# -------------------------
# 5. Compute correlation matrix
# -------------------------
def compute_correlations_matrix(df):
    corr = df[['Reviews', 'Price', 'Installs']].corr()
    sns.heatmap(corr, annot=True, cmap='coolwarm')
    plt.title('Correlation Matrix')
    plt.show()

# -------------------------
# 6. Scatter matrix
# -------------------------
def print_scatter_matrix(df):
    scatter_matrix(df[['Reviews', 'Price', 'Installs']], figsize=(10, 6), diagonal='kde')
    plt.suptitle('Scatter Matrix')
    plt.show()

# -------------------------
# 7. Category-wise analysis
# -------------------------
def category_analysis(df):
    # Total installations per category
    installs_per_category = df.groupby('Category')['Installs'].sum().sort_values(ascending=False)
    print("Installations per category:")
    print(installs_per_category)

    # Pie chart installations
    installs_per_category.plot.pie(autopct='%1.1f%%', figsize=(8, 8))
    plt.ylabel('')
    plt.title('Installations by Category')
    plt.show()

    # Mean price per category
    mean_price = df.groupby('Category')['Price'].mean().sort_values(ascending=False)
    mean_price.plot.bar(figsize=(12, 6))
    plt.title('Mean Price per Category')
    plt.ylabel('Price ($)')
    plt.show()

    # Most expensive apps per category
    idx = df.groupby('Category')['Price'].idxmax()
    expensive_apps = df.loc[idx, ['Category', 'App', 'Price']]
    print("Most expensive apps per category:")
    print(expensive_apps)

# -------------------------
# 8. Paid Family apps analysis
# -------------------------
def paid_family_analysis(df):
    paid_family = df[(df['Category'] == 'FAMILY') & (df['Type'] == 'Paid')]

    # Top paid apps by installs
    top_paid = paid_family.sort_values(by='Installs', ascending=False).head(10)
    top_paid.plot.bar(x='App', y='Installs', figsize=(12, 6))
    plt.title('Top Paid Family Apps by Installs')
    plt.ylabel('Number of Installs')
    plt.xticks(rotation=45)
    plt.show()

    # Genre distribution
    genre_counts = paid_family['Genres'].value_counts()
    genre_counts.plot.pie(autopct='%1.1f%%', figsize=(8, 8))
    plt.ylabel('')
    plt.title('Genre Distribution in Paid Family Apps')
    plt.show()

# -------------------------
# 9. Main execution
# -------------------------
if __name__ == "__main__":
    dataset_path = "googleplaystore.csv"  # change to your downloaded CSV path
    df = load_dataset(dataset_path)
    df = clean_dataset(df)
    print_summarize_dataset(df)
    print_histograms(df)
    compute_correlations_matrix(df)
    print_scatter_matrix(df)
    category_analysis(df)
    paid_family_analysis(df)
