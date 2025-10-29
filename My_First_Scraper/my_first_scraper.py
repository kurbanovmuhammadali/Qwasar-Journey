import requests
from bs4 import BeautifulSoup

#first part of mine code is the request for http to get request to the github
def request_github_trending(url):
    response = requests.get(url)#sens a get request to the specific url
    if response.status_code == 200:#checks the request
        return response #retruns http response object
    else:
        raise Exception(f"Failed to fetch URL: {url} (Status code: {response.status_code})")

#in this function i extract the repository element from html response
def extract(response):
    soup = BeautifulSoup(response.text, 'html.parser')#converts html to searchable object
    return soup.find_all('article', class_='Box-row')#elements with the class Box row 

#extracts useful information from the html elements and converts them into python dictionaries
def transform(html_repos):
    repos_data = []
    for repo in html_repos[:25]:  #limit for repositories
        full_name = repo.h1.text.strip().replace('\n', '').replace(' ', '')#github displays developer name or repository
        developer, repository_name = full_name.split('/')#splits the full name 
        star_tag = repo.find('a', href=lambda x: x and x.endswith('/stargazers'))
        if star_tag:
            nbr_stars = star_tag.text.strip().replace(',', '')#cleans not needed things like , and etc
        else:
            nbr_stars = '0'

        repos_data.append({
            'developer': developer,
            'repository_name': repository_name,
            'nbr_stars': nbr_stars
        })

    return repos_data

#converts structured dta into a csv string
def format(repositories_data):
    csv_output = "Developer,Repository Name,Number of Stars\n"
    for repo in repositories_data:#here i created csv header  and here appends a new line for each repository with developer name and etc...
        csv_output += f"{repo['developer']},{repo['repository_name']},{repo['nbr_stars']}\n"
    return csv_output

