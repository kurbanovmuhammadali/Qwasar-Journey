requests = {}

requests['Display all actors'] = "SELECT * FROM actors;"
requests['Display all genres'] = "SELECT * FROM genres;"
requests['Display the name and year of the movies'] = "SELECT mov_title, mov_year FROM movies;"
requests['Display reviewer name from the reviews table'] = "SELECT rev_name FROM reviews;"

requests["Find the year when the movie American Beauty released"] = "SELECT mov_year FROM movies WHERE mov_title = 'American Beauty';"
requests["Find the movie which was released in the year 1999"] = "SELECT mov_title FROM movies WHERE mov_year = 1999;"
requests["Find the movie which was released before 1998"] = "SELECT mov_title FROM movies WHERE mov_year < 1998;"
requests["Find the name of all reviewers who have rated 7 or more stars to their rating order by reviews rev_name (it might have duplicated names :-))"] = "
SELECT r.rev_name 
FROM reviews r 
JOIN movies_ratings_reviews mrr ON r.id = mrr.rev_id 
WHERE mrr.rev_stars >= 7 
ORDER BY r.rev_name;
"
requests["Find the titles of the movies with ID 905, 907, 917"] = "SELECT mov_title FROM movies WHERE id IN (905, 907, 917);"
requests["Find the list of those movies with year and ID which include the words Boogie Nights"] = "SELECT id, mov_title, mov_year FROM movies WHERE mov_title LIKE '%Boogie Nights%';"
requests["Find the ID number for the actor whose first name is 'Woody' and the last name is 'Allen'"] = "SELECT id FROM actors WHERE act_fname = 'Woody' AND act_lname = 'Allen';"

requests["Find the actors with all information who played a role in the movies 'Annie Hall'"] = "
SELECT a.* 
FROM actors a 
JOIN movies_actors ma ON a.id = ma.act_id 
JOIN movies m ON ma.mov_id = m.id 
WHERE m.mov_title = 'Annie Hall';
"
requests["Find the first and last names of all the actors who were cast in the movies 'Annie Hall', and the roles they played in that production"] = "
SELECT a.act_fname, a.act_lname, ma.role 
FROM actors a 
JOIN movies_actors ma ON a.id = ma.act_id 
JOIN movies m ON ma.mov_id = m.id 
WHERE m.mov_title = 'Annie Hall';
"

requests["Find the name of movie and director who directed a movies that casted a role as Sean Maguire"] = "
SELECT d.dir_fname, d.dir_lname, m.mov_title 
FROM movies m 
JOIN directors_movies dm ON m.id = dm.mov_id 
JOIN directors d ON dm.dir_id = d.id 
JOIN movies_actors ma ON m.id = ma.mov_id 
WHERE ma.role = 'Sean Maguire';
"

requests['Find all the actors who have not acted in any movie between 1990 and 2000 (select only actor first name, last name, movie title and release year)'] =
  'SELECT a.act_fname, a.act_lname, m.mov_title, CAST(strftime("%Y", m.mov_dt_rel) AS INT) AS release_year ' \
  'FROM actors a ' \
  'JOIN movies_actors ma ON a.id = ma.act_id ' \
  'JOIN movies m ON m.id = ma.mov_id ' \
  'WHERE NOT EXISTS ( ' \
  '  SELECT 1 ' \
  '  FROM movies_actors ma2 ' \
  '  JOIN movies m2 ON m2.id = ma2.mov_id ' \
  '  WHERE ma2.act_id = a.id ' \
  '    AND CAST(strftime("%Y", m2.mov_dt_rel) AS INT) BETWEEN 1990 AND 2000 ' \
  ');'