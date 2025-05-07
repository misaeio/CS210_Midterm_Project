#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
using namespace std;

struct CityData {
    string country;
    int population;
};

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    unordered_map<string, int> country_population;
};

class Trie {
public:
    Trie() { root = new TrieNode(); }

    void insert(const string& city, const string& country, int population) {
        TrieNode* node = root;
        for (char c : city) {
            if (!node->children[c]) node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->country_population[country] = population;
    }

    bool search(const string& city, const string& country, int& population) {
        TrieNode* node = root;
        for (char c : city) {
            if (!node->children[c]) return false;
            node = node->children[c];
        }
        if (node->country_population.count(country)) {
            population = node->country_population[country];
            return true;
        }
        return false;
    }

private:
    TrieNode* root;
};

struct CacheEntry {
    string key;
    int population;
};

class Cache {
public:
    Cache(string type) : strategy(type) {}

    bool get(const string& key, int& population) {
        if (cache_map.count(key)) {
            if (strategy == "LFU") freq[key]++;
            population = cache_map[key].population;
            return true;
        }
        return false;
    }

    void put(const string& key, int population) {
        if (cache_map.size() >= 10) evict();
        cache_map[key] = { key, population };
        if (strategy == "LFU") freq[key] = 1;
        if (strategy == "FIFO") fifo.push_back(key);
        if (strategy == "Random") keys.push_back(key);
    }

    void print() {
        cout << "\nCache Contents:\n";
        for (auto& pair : cache_map)
            cout << pair.first << " -> " << pair.second.population << endl;
    }

private:
    unordered_map<string, CacheEntry> cache_map;
    unordered_map<string, int> freq;
    vector<string> fifo;
    vector<string> keys;
    string strategy;

    void evict() {
        string victim;
        if (strategy == "LFU") {
            int min_freq = INT_MAX;
            for (auto& f : freq) {
                if (f.second < min_freq) {
                    min_freq = f.second;
                    victim = f.first;
                }
            }
            freq.erase(victim);
        }
        else if (strategy == "FIFO") {
            victim = fifo.front();
            fifo.erase(fifo.begin());
        }
        else if (strategy == "Random") {
            int idx = rand() % keys.size();
            victim = keys[idx];
            keys.erase(keys.begin() + idx);
        }
        cache_map.erase(victim);
    }
};

void load_data(Trie& trie) {
    ifstream file("world_cities.csv");
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string city, country;
        int population;
        getline(ss, city, ',');
        getline(ss, country, ',');
        ss >> population;
        trie.insert(city, country, population);
    }
}

int main() {
    Trie trie;
    load_data(trie);

    string strategy;
    cout << "Choose cache strategy (LFU/FIFO/Random): ";
    cin >> strategy;
    Cache cache(strategy);

    while (true) {
        string city, country;
        cout << "\nEnter city (or 'exit'): ";
        cin >> city;
        if (city == "exit") break;
        cout << "Enter country code: ";
        cin >> country;

        string key = city + "," + country;
        int population;

        if (cache.get(key, population)) {
            cout << "Cache Hit. Population: " << population << endl;
        }
        else {
            if (trie.search(city, country, population)) {
                cout << "Cache Miss. Trie Lookup. Population: " << population << endl;
                cache.put(key, population);
            }
            else {
                cout << "City not found.\n";
            }
        }

        cache.print();
    }

    return 0;
}
