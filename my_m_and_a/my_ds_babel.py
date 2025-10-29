import sqlite3
import csv
from io import StringIO

def sql_to_csv(database, table_name):
    conn = sqlite3.connect(database)
    cursor = conn.cursor()
    cursor.execute(f"PRAGMA table_info({table_name})")
    columns = [info[1] for info in cursor.fetchall()]

    cursor.execute(f"SELECT * FROM {table_name}")
    rows = cursor.fetchall()
    output = StringIO(newline='')
    writer = csv.writer(output, lineterminator='\n')
    writer.writerow(columns)
    writer.writerows(rows)

    conn.close()
    return output.getvalue().strip()


def csv_to_sql(csv_content, database, table_name):
    conn = sqlite3.connect(database)
    cursor = conn.cursor()

    reader = csv.reader(csv_content)
    headers = next(reader)

    quoted_headers = [f'"{col}"' for col in headers]
    columns_def = ", ".join([f'{col} TEXT' for col in quoted_headers])

    cursor.execute(f'CREATE TABLE IF NOT EXISTS "{table_name}" ({columns_def});')

    placeholders = ", ".join(["?"] * len(headers))
    insert_query = f'INSERT INTO "{table_name}" VALUES ({placeholders})'

    for row in reader:
        cursor.execute(insert_query, row)

    conn.commit()
    conn.close()
