# Welcome to My M And A
***

## Task
You've worked as a Junior Data Engineer at Plastic Free Boutique for three months.

Your first mission was to build a strong, robust, and scalable customers database for the exponential growth the company will soon have. Your manager is delighted.

We've just acquired a new company, Only Wood Box, which will be a perfect solution for our packaging department. They are experts in making wood packages at a competitive, light, and cheap price.

## Description
Expert in their technology, they didn't believe in the digital world. Despite the decent number of customers, they didn't have to invest in their infrastructure. Before quitting, their engineer told us that at least we had stored all the information; I don't understand what he meant.

You should use import pandas as pd

Your mission will be to merge their three customers (yes 3 :D) table into ours.

Table 1

Table 2

Table 3



## Installation
to install this project follow this link: git@git.us.qwasar.io:my_m_and_a_194018_nz97xe/my_m_and_a.git

## Usage
The function reads the three CSV files into pandas DataFrames and merges them vertically using pd.concat, combining all customer rows into one table. It standardizes column names by stripping spaces, then ensures all required columns from the target schema exist. For FirstName and LastName, it removes placeholder values like "string_*" to clean the data. The Age column is converted to string, City is title-cased, and Gender is filtered to only allow "Female" or "Male". Finally, the DataFrame columns are reordered to match the exact schema. You can check it by printing merged_df.head() to see the top rows, merged_df.columns to verify column names, and merged_df.shape to confirm the total number of rows and columns.
```
./my_project argument1 argument2
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
