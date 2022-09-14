#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <map>

using namespace std;

bool compare_ratings(pair<int, double> rating1, pair<int, double> rating2) {
    return rating1.first < rating2.first;
}

vector<string> tokenize(string str, string del) {
    vector<string> tokens;
    size_t prev = 0, nxt = 0;

    while (nxt != string::npos) {
        nxt = str.find(del, prev);

        tokens.push_back(str.substr(prev, nxt - prev));

        prev = nxt + del.size();
    }

    return tokens;
}

map<int, map<int, double>> parse_ratings(string ratings_filepath) {
    map<int, map<int, double>> ratings;

    ifstream ratings_file;
    ratings_file.open(ratings_filepath);

    string line;

    while(getline(ratings_file, line)) {
        vector<string> tokens = tokenize(line, "::");
        int user = stoi(tokens[0]);
        int movie = stoi(tokens[1]);
        double rating = stod(tokens[2]);
        
        ratings[user][movie] = rating;
    }

    return ratings;
}

double rating_distance(map<int, map<int, double>> &ratings, int user1, int user2) {
    map<int, double> common;
    set_intersection(ratings[user1].begin(), ratings[user1].end(),
        ratings[user2].begin(), ratings[user2].end(),
        inserter(common, common.begin()),
        compare_ratings);

    double distance = 0;
    for (auto pair : common) {
        distance += abs(pair.second - ratings[user2][pair.first]);
    } 
    distance /= common.size();
    return distance;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Wrong number of arguments, please supply only a valid userID and a movieID." << endl;
        return -1;
    }

    map<int, map<int, double>> ratings = parse_ratings("ratings.dat");

    int user1 = stoi(argv[1]), movie = stoi(argv[2]);

    double min_dist = 1000000000;
    int min_user = 0;

    for (auto user : ratings) {
        // Ignore if same user
        if (user.first == user1) {
            continue;
        }

        // Only consider users that have seen target movie
        try {
            user.second.at(movie);
        }
        catch(const out_of_range& e)
        {
            continue;
        }
        
        double dist = rating_distance(ratings, user1, user.first);

        if (dist < min_dist) {
            min_dist = dist;
            min_user = user.first;
        }
    }

    double pred_rating = ratings[min_user][movie];

    cout << "The predicted rating for UserID: " << user1 << " and MovieID: " << movie << " is " \
    << pred_rating << " based on the most similar user: " << min_user 
    << " with a ratingdistance: " << min_dist << endl;

    return 0;
}
