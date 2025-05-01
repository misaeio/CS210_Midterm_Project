#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct CityKey {
    string countryCode;
    string cityName;

    bool operator==(const CityKey& other) const {
        return countryCode == other.countryCode && cityName == other.cityName;
    }
};

namespace std {
    template<>
    struct hash<CityKey> {
        size_t operator()(const CityKey& k) const {
            return hash<string>()(k.countryCode) ^ hash<string>()(k.cityName);
        }
    };
}

class CityCache {
public:
    virtual bool get(const CityKey& key, double& population) = 0;
    virtual void put(const CityKey& key, double population) = 0;
    virtual void displayCache() const = 0;
};

class FIFOCache : public CityCache {
    unordered_map<CityKey, double> cache;
    queue<CityKey> order;

public:
    bool get(const CityKey& key, double& population) {
        if (cache.find(key) != cache.end()) {
            population = cache[key];
            return true;
        }
        return false;
    }

    void put(const CityKey& key, double population) {
        if (cache.find(key) == cache.end()) {
            if (cache.size() >= 10) {
                CityKey oldKey = order.front();
                order.pop();
                cache.erase(oldKey);
            }
            order.push(key);
        }
        cache[key] = population;
    }

    void displayCache() const {
        cout << "FIFO Cache:\n";
        for (const auto& pair : cache) {
            cout << pair.first.countryCode << ", " << pair.first.cityName << " => " << pair.second << '\n';
        }
    }
};

class LFUCache : public CityCache {
    struct CacheEntry {
        double population;
        int freq;
    };

    unordered_map<CityKey, CacheEntry> cache;

public:
    bool get(const CityKey& key, double& population) {
        if (cache.find(key) != cache.end()) {
            cache[key].freq++;
            population = cache[key].population;
            return true;
        }
        return false;
    }

    void put(const CityKey& key, double population) {
        if (cache.find(key) == cache.end() && cache.size() >= 10) {
            CityKey toRemove;
            int minFreq = INT_MAX;
            for (auto& entry : cache) {
                if (entry.second.freq < minFreq) {
                    minFreq = entry.second.freq;
                    toRemove = entry.first;
                }
            }
            cache.erase(toRemove);
        }
        cache[key].population = population;
        cache[key].freq++;
    }

    void displayCache() const {
        cout << "LFU Cache:\n";
        for (const auto& pair : cache) {
            cout << pair.first.countryCode << ", " << pair.first.cityName << " => " << pair.second.population << " (freq: " << pair.second.freq << ")\n";
        }
    }
};

class RandomCache : public CityCache {
    unordered_map<CityKey, double> cache;
    vector<CityKey> keys;

public:
    RandomCache() { srand(time(0)); }

    bool get(const CityKey& key, double& population) {
        if (cache.find(key) != cache.end()) {
            population = cache[key];
            return true;
        }
        return false;
    }

    void put(const CityKey& key, double population) {
        if (cache.find(key) == cache.end() && cache.size() >= 10) {
            int index = rand() % keys.size();
            cache.erase(keys[index]);
            keys.erase(keys.begin() + index);
        }
        if (cache.find(key) == cache.end())
            keys.push_back(key);

        cache[key] = population;
    }

    void displayCache() const {
        cout << "Random Cache:\n";
        for (const auto& pair : cache) {
            cout << pair.first.countryCode << ", " << pair.first.cityName << " => " << pair.second << '\n';
        }
    }
};

double searchCSV(const string& filename, const CityKey& key) {
    ifstream file(filename);
    if (!file.is_open()) return -1;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, country;
        double population;

        getline(ss, name, ',');
        getline(ss, country, ',');
        ss >> population;

        if (country == key.countryCode && name == key.cityName) {
            return population;
        }
    }
    return -1;
}

int main() {
    string filename = "world_cities.csv";
    CityCache* cache = nullptr;

    cout << "Select cache strategy:\n1. LFU\n2. FIFO\n3. Random\nChoice: ";
    int choice;
    cin >> choice;

    if (choice == 1) cache = new LFUCache();
    else if (choice == 2) cache = new FIFOCache();
    else if (choice == 3) cache = new RandomCache();
    else {
        cout << "Invalid choice.\n";
        return 1;
    }

    while (true) {
        cout << "\nEnter country code (or 'exit' to quit): ";
        string countryCode;
        cin >> countryCode;
        if (countryCode == "exit") break;

        cout << "Enter city name: ";
        cin.ignore();
        string cityName;
        getline(cin, cityName);

        CityKey key = { countryCode, cityName };
        double population;

        if (cache->get(key, population)) {
            cout << "Population (from cache): " << population << '\n';
        }
        else {
            population = searchCSV(filename, key);
            if (population != -1) {
                cout << "Population (from file): " << population << '\n';
                cache->put(key, population);
            }
            else {
                cout << "City not found.\n";
            }
        }

        cache->displayCache();
    }

    delete cache;
    return 0;
}
