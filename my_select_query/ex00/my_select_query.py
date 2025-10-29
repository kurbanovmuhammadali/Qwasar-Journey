import csv
import io

class MySelectQuery:
    def __init__(self, csv_content):
        self.rows = list(csv.reader(io.StringIO(csv_content)))
        self.headers = self.rows[0]
        self.data = self.rows[1: ]

    def where(self, column_name, name):
        idx = self.headers.index(column_name)
        result =[]

        for i in self.data:
            if i[idx] == name:
                result.append(",".join(i))
        return result

# with open("file.csv", 'r') as file:
#     csvreader = file.read()

#     query = MySelectQuery(csvreader)
#     result = query.where("name", "Perry Young")
#     print(result)