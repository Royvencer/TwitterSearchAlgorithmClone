#include "Trendtracker.h"
#include <iostream>
Trendtracker::Trendtracker(string filename) {
    // Implementing the constructor to read the file and populate hashtags vector
    ifstream file(filename);

    if (file.is_open()) {
        string hashtag;
        while (getline(file, hashtag)) {
            Entry entry;
            entry.hashtag = hashtag;
            entry.pop = 0; // Initializing the popularity count to 0
            E.push_back(entry);
        }
        file.close();
    }

    cout << "Size of E after constructor: " << E.size() << endl;

    // Check if E.size() is less than 3
    if (E.size() < 3) {
        // If E has fewer than 3 hashtags, push back all indices of E into S
        for (int i = 0; i < E.size(); i++) {
            S.push_back(i);
        }
    }
    else {
        // If E has 3 or more hashtags, push back only the first three indices of E into S
        for (int i = 0; i < 3; i++) {
            S.push_back(i);
        }
    }
}


int Trendtracker::size() {
    // Implementing the size function
    return E.size();
}

void Trendtracker::tweeted(string ht) {
    int index = search(ht);

    if (index != -1) {
        E[index].pop++;

        // Check if the hashtag is already in the top 3 (S)
        auto it = std::find(S.begin(), S.end(), index);

        if (it != S.end()) {
            // Update the position of the hashtag in S based on popularity
            while (it != S.begin() && E[*(it - 1)].pop < E[*it].pop) {
                std::iter_swap(it, it - 1);
                --it;
            }
        }
        else {
            // If the hashtag is not in S, check if it should be in the top 3
            if (S.size() < 3 || E[index].pop > E[S.back()].pop) {
                // Insert the hashtag index at the correct position in S
                S.push_back(index);

                // Sort S based on popularity
                int pos = S.size() - 1;
                while (pos > 0 && E[S[pos]].pop > E[S[pos - 1]].pop) {
                    std::iter_swap(S.begin() + pos, S.begin() + pos - 1);
                    --pos;
                }

                // Ensure S has a maximum size of 3
                while (S.size() > 3) {
                    S.pop_back();
                }
            }
        }
    }
}


int Trendtracker::popularity(string name) {
    // Implementing the popularity function using binary search
    int index = search(name);

    if (index != -1) {
        return E[index].pop;
    }

    return -1; // Hashtag not found
}

string Trendtracker::top_trend() {
    if (!S.empty()) {
        return E[S[0]].hashtag;
    }

    return ""; // Trendtracker has no hashtags
}

void Trendtracker::top_three_trends(vector<string>& T) {
    T.clear();

    // If there are fewer than 3 existing hashtags, only copy the available ones from E
    int count = std::min(3, static_cast<int>(S.size()));

    if (S.empty()) {
        // If nothing has been tweeted, populate S with up to 3 hashtags from E
        for (int i = 0; i < std::min(3, static_cast<int>(E.size())); i++) {
            S.push_back(i);
        }
    }

    // Ensure that the size of S does not exceed 3
    S.resize(std::min(3, static_cast<int>(S.size())));

    for (int i = 0; i < count; i++) {
        int index = S[i];
        if (index < E.size()) {
            T.push_back(E[index].hashtag);
        }
        else {
            // Handle the case where the index is out of bounds
            T.push_back("N/A"); // or any other placeholder value
        }
    }

    // Print the size of S after resizing
    cout << "Size of S after resizing: " << S.size() << endl;

    // Print the contents of vector T
    
}


int Trendtracker::search(string ht) {
    // Implementing the binary search function
    int left = 0, right = E.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (E[mid].hashtag == ht) {
            return mid;
        }
        else if (E[mid].hashtag < ht) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1; // Hashtag not found
}