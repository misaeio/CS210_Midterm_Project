#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void load_queries(const string& filename, vector<pair<string, string>>& queries) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string city, country;
        getline(ss, city, ',');
        getline(ss, country, ',');
        queries.push_back({ city, country });
    }
    cout << "Loaded " << queries.size() << " queries.\n";  // Debugging print
}

int main() {
    vector<pair<string, string>> queries;
    load_queries("queriesS.csv", queries);  // Ensure this is the correct file path

    if (queries.empty()) {
        cout << "No queries loaded. Exiting...\n";
        return 0;
    }

    for (const auto& query : queries) {
        cout << "City: " << query.first << ", Country: " << query.second << endl;
    }

    return 0;
}
