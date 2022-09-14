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

map<int, map<int, double>> parse_ratings_pair(string ratings_filepath, string user1, string user2) {
    map<int, map<int, double>> ratings;

    ifstream ratings_file;
    ratings_file.open(ratings_filepath);

    string line;
    int found = 0;
    bool target = false;

    while(getline(ratings_file, line) && found <= 2) {
        string user_id = line.substr(0, line.find("::"));

        if (user_id == user1 || user_id == user2) {
            if (!target) {
                found++;
                target = true;
            }

            vector<string> tokens = tokenize(line, "::");
            int user = stoi(tokens[0]);
            int movie = stoi(tokens[1]);
            double rating = stod(tokens[2]);
            ratings[user][movie] = rating;
        } else if (target) {
            target = false;
        }
    }

    return ratings;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Wrong number of arguments, please supply only two valid userIDs." << endl;
        return -1;
    }

    map<int, map<int, double>> ratings = parse_ratings_pair("ratings.dat", argv[1], argv[2]);

    int user1 = stoi(argv[1]), user2 = stoi(argv[2]);
    map<int, double> common;
    set_intersection(ratings[user1].begin(), ratings[user1].end(),
        ratings[user2].begin(), ratings[user2].end(),
        inserter(common, common.begin()),
        compare_ratings);

    if (common.empty()) {
        cout << "The users have not seen any movies in common, therefore cannot calculate RatingDistance" << endl;
        return 0;
    }

    double distance = 0;
    for (auto pair : common) {
        // cout << pair.first << ", " << pair.second << endl;
        // cout << pair.first << ", " << ratings[user2][pair.first] << endl;
        distance += abs(pair.second - ratings[user2][pair.first]);
    } 
    distance /= common.size();

    cout << "The RatingDistance is " << distance << endl;

    return 0;
}
