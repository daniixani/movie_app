#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Movie {
    private:
        string movie_name;
        double average_rating;
        int num_votes;
        vector<string> keywords;

    public:
        Movie(string name, double rating, int votes, vector<string> kw = {}) 
            : movie_name(name), average_rating(rating), num_votes(votes), keywords(kw) {}

        string getMovieName() const {
            return movie_name;
        }

        double getAverageRating() const {
            return average_rating;
        }

        int getNumVotes() const {
            return num_votes;
        }

        vector<string> getKeywords() const {
            return keywords;
        }

        void displayMovieInfo() const {
            cout << "Movie Name: " << movie_name << endl;
            cout << "Average Rating: " << average_rating << endl;
            cout << "Number of Votes: " << num_votes << endl;
            if (!keywords.empty()) {
                cout << "Keywords: ";
                for (const auto& kw : keywords) {
                    cout << kw << " ";
                }
                cout << endl;
            }
        }
};

class User {
    private:
        vector<Movie> liked_movies;
        vector<string> preferred_keywords;

    public:
        User(vector<string> keywords) : preferred_keywords(keywords) {}

        void likeMovie(const Movie& movie) {
            liked_movies.push_back(movie);
        }

        vector<Movie> getLikedMovies() const {
            return liked_movies;
        }

        vector<string> getPreferredKeywords() const {
            return preferred_keywords;
        }
};

// Function to check keyword matches
bool checkKeywordMatch(const vector<string>& movie_keywords, const vector<string>& user_keywords, int threshold) {
    int match_count = 0;
    for (const auto& kw : movie_keywords) {
        if (find(user_keywords.begin(), user_keywords.end(), kw) != user_keywords.end()) {
            match_count++;
        }
        if (match_count >= threshold) {
            return true;
        }
    }
    return false;
}

// Function to recommend a movie
bool recommendMovie(const Movie& movie, const User& user, int kw_threshold, int movie_match_threshold) {
    // Check if the movie matches the user's preferred keywords
    if (!checkKeywordMatch(movie.getKeywords(), user.getPreferredKeywords(), kw_threshold)) {
        return false;
    }

    // Check if the recommended movie matches the user's liked movies
    int liked_movie_matches = 0;
    for (const auto& liked_movie : user.getLikedMovies()) {
        if (checkKeywordMatch(movie.getKeywords(), liked_movie.getKeywords(), kw_threshold)) {
            liked_movie_matches++;
        }
        if (liked_movie_matches >= movie_match_threshold) {
            return true;
        }
    }
    return false;
}

// Function to load movies from a TSV file
vector<Movie> loadMoviesFromFile(const string& filename) {
    vector<Movie> movies;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return movies;
    }

    string line;
    getline(file, line); // Skip the header line
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        double rating;
        int votes;
        vector<string> keywords;
        string keyword;
        // Assume TSV format: tconst    averageRating    numVotes
        getline(ss, name, '\t');
        ss >> rating;
        ss.ignore();
        ss >> votes;
        // Placeholder for keywords
        keywords = {"example_keyword1", "example_keyword2"}; // Replace with actual keywords loading logic if available
        movies.push_back(Movie(name, rating, votes, keywords));
    }
    return movies;
}

int main() {
    // User's preferred keywords
    vector<string> user_keywords = {"action", "adventure", "comedy", "drama", "fantasy"};

    // User's liked movies
    User user(user_keywords);
    user.likeMovie(Movie("Movie A", 8.0, 1000, {"action", "drama", "thriller"}));
    user.likeMovie(Movie("Movie B", 9.0, 1500, {"comedy", "action", "romance"}));
    user.likeMovie(Movie("Movie C", 7.0, 900, {"fantasy", "adventure", "action"}));
    

    // Load movies from file
    vector<Movie> all_movies = loadMoviesFromFile("title.ratings.tsv");

    // Recommend movies
    for (const auto& movie : all_movies) {
        if (recommendMovie(movie, user, 5, 10)) {
            cout << "Recommended movie for you: " << movie.getMovieName() << endl;
        }
    }

    return 0;
}
