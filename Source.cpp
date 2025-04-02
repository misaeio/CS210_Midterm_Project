#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <sstream>

using namespace std;

class School {
public:
    string name;
    string city;
    int yearEstablished;

    School(string n, string c, int y) : name(n), city(c), yearEstablished(y) {}
};

class SchoolHashTable {
private:
    vector<list<School>> table; 
    int tableSize;

    int hashFunction(string key) {
        int hash = 0;
        for (char ch : key) {
            hash += ch; 
        }
        return hash % tableSize; 
    }

public:
    SchoolHashTable(int size) : tableSize(size) {
        table.resize(tableSize);
    }

    void insert(School school) {
        int index = hashFunction(school.name);
        table[index].push_back(school);  
    }

    School* findByName(string name) {
        int index = hashFunction(name);
        for (School& school : table[index]) {
            if (school.name == name) {
                return &school;  
            }
        }
        return nullptr; 
    }

    void deleteByName(string name) {
        int index = hashFunction(name);
        auto& schools = table[index];
        for (auto it = schools.begin(); it != schools.end(); ++it) {
            if (it->name == name) {
                schools.erase(it);  
                return;
            }
        }
    }

    void display() {
        for (int i = 0; i < tableSize; ++i) {
            cout << "Index " << i << ": ";
            for (const School& school : table[i]) {
                cout << school.name << " (" << school.city << ", " << school.yearEstablished << ") ";
            }
            cout << endl;
        }
    }
};

int main() {
    SchoolHashTable hashTable(3);

    ifstream file("School.csv");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, city;
        int year;
        getline(ss, name, ',');
        getline(ss, city, ',');
        ss >> year;
        hashTable.insert(School(name, city, year));
    }

    cout << "All schools in the table:" << endl;
    hashTable.display();

    string searchName;
    cout << "\nEnter school name to search: ";
    getline(cin, searchName);
    School* foundSchool = hashTable.findByName(searchName);
    if (foundSchool) {
        cout << "Found school: " << foundSchool->name << " in " << foundSchool->city << endl;
    }
    else {
        cout << "School not found." << endl;
    }

    cout << "\nEnter school name to delete: ";
    getline(cin, searchName);
    hashTable.deleteByName(searchName);

    cout << "\nSchools after deletion:" << endl;
    hashTable.display();

    return 0;
}
