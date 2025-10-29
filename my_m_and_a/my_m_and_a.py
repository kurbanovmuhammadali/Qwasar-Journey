import pandas as pd

def my_m_and_a(content_database_1, content_database_2, content_database_3):
    """
    Merge three CSVs into a single DataFrame with a unified schema.
    """
    # Read CSVs
    df1 = pd.read_csv(content_database_1)
    df2 = pd.read_csv(content_database_2)
    df3 = pd.read_csv(content_database_3)

    # Merge all tables
    merged_df = pd.concat([df1, df2, df3], ignore_index=True)

    # Standardize column names (strip spaces)
    merged_df.columns = [col.strip() for col in merged_df.columns]

    # Define target schema with exact capitalization
    schema_cols = ['Gender', 'FirstName', 'LastName', 'Email', 
                   'Age', 'City', 'Country', 'Created_at', 'Referral']

    # Add missing columns if any
    for col in schema_cols:
        if col not in merged_df.columns:
            merged_df[col] = ""

    # Clean FirstName and LastName: remove placeholder values
    for col in ['FirstName', 'LastName']:
        merged_df[col] = merged_df[col].astype(str)
        merged_df[col] = merged_df[col].apply(lambda x: "" if x.lower().startswith('string_') else x)

    # Clean Age: convert to string
    merged_df['Age'] = merged_df['Age'].astype(str)

    # Clean City: title-case
    merged_df['City'] = merged_df['City'].astype(str).str.title()

    # Clean Gender: only allow 'Female' and 'Male'
    merged_df['Gender'] = merged_df['Gender'].str.capitalize()
    merged_df = merged_df[merged_df['Gender'].isin(['Female', 'Male'])]

    # Reorder columns
    merged_df = merged_df[schema_cols]

    return merged_df
