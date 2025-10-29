from flask import Flask, request, jsonify
import csv
import json

app = Flask(__name__)#used for making the main flask application flask - is the main class for making web application

#in one word in this function we are loading data from a csv file that we ahve
def data_from_csvfile():
    movies_data = []
    with open('data.csv', newline='', encoding='utf-8') as csvfile:
        reader = csv.DictReader(csvfile)#reads a csv file
        for row in reader:
            movies_data.append(dict(row))#adds each information to a list 
    return movies_data
movies_data = data_from_csvfile()

#i tink you understand by your own what does this function means
def sorting_genre(classification):
    classification = classification.lower()#we lovercase the name of the genre for case-insesitive comparison or something like that
    return [movies_data for movies_data in movies_data if classification in movies_data.get("Genre", "").lower()]

#query by genre using url 
@app.route('/')
def quering_movies():
    classification = request.args.get('genre')
    
    if not classification:
        return jsonify([])  
    
    sorted_movies = sorting_genre(classification.lower())
    return json.dumps(sorted_movies) #returns the genre in JSON with json.dump command

@app.route('/<genre>')
def geeting_genre_byurl(genre):
    sorted_movies = sorting_genre(genre.lower())
    return json.dumps(sorted_movies)  #genre from the path i mean from the url path something like ....8080/comedy

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)