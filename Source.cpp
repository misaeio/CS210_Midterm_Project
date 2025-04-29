#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

struct CityKey {
    string countryCode;
    string cityName;

    bool operator==(const CityKey& other) const {
        return countryCode == other.countryCode && cityName == other.cityName;
    }
};

struct CityKeyHash {
    size_t operator()(const CityKey& key) const {
        return hash<string>()(key.countryCode) ^ (hash<string>()(key.cityName) << 1);
    }
};

class CityCache {
private:
    list<pair<CityKey, double>> cacheList;
    unordered_map<CityKey, list<pair<CityKey, double>>::iterator, CityKeyHash> cacheMap;
    const size_t maxSize = 10;

public:
    bool get(const CityKey& key, double& population) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
        
            cacheList.splice(cacheList.begin(), cacheList, it->second);
            population = it->second->second;
            return true;
        }
        return false;
    }

    void put(const CityKey& key, double population) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            cacheList.erase(it->second);
            cacheMap.erase(it);
        }
        cacheList.push_front({ key, population });
        cacheMap[key] = cacheList.begin();

        if (cacheList.size() > maxSize) {
            auto last = cacheList.end();
            last--;
            cacheMap.erase(last->first);
            cacheList.pop_back();
        }
    }

    void displayCache() {
        cout << "Current Cache:\n";
        for (auto& entry : cacheList) {
            cout << entry.first.countryCode << ", " << entry.first.cityName
                << " -> " << entry.second << endl;
        }
    }
};

double searchCSV(const string& filename, const CityKey& key) {
    ifstream file(filename);
    string line;
    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string countryCode, cityName, populationStr;
        getline(ss, countryCode, ',');
        getline(ss, cityName, ',');
        getline(ss, populationStr, ',');

        if (countryCode == key.countryCode && cityName == key.cityName) {
            return stod(populationStr);
        }
    }
    return -1; 
}

int main() {
    string filename = "world_cities.csv";
    CityCache cache;

    while (true) {
        cout << "\nEnter country code (or 'exit' to quit): ";
        string countryCode;
        cin >> countryCode;
        if (countryCode == "exit") break;

        cout << "Enter city name: ";
        string cityName;
        cin.ignore();
        getline(cin, cityName);

        CityKey key = { countryCode, cityName };
        double population;

        if (cache.get(key, population)) {
            cout << "Population (from cache): " << population << endl;
        }
        else {
            population = searchCSV(filename, key);
            if (population != -1) {
                cout << "Population (from file): " << population << endl;
                cache.put(key, population);
            }
            else {
                cout << "City not found.\n";
            }
        }

        cache.displayCache(); 
    }

    return 0;
}
