#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class CSVReader {
public:
    static vector<vector<string>> readCSV(const string& filename) {
        ifstream file(filename);
        vector<vector<string>> data;
        string line, word;

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return data;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }
            data.push_back(row);
        }
        file.close();
        return data;
    }
};

struct School {
    string name;
    string city;
    string address;
    string state;
    string county;
    struct School* next;

    School(string n, string c, string a, string s, string y) :
        name(n), city(c), address(a), state(s), county(y), next(nullptr) {
    }
};

class SchoolList {
private:
    School* head;
public:
    SchoolList() : head(nullptr) {}

    void insertFirst(School school) {
        School* newSchool = new School(school);
        newSchool->next = head;
        head = newSchool;
    }

    void insertLast(School school) {
        School* newSchool = new School(school);
        if (!head) {
            head = newSchool;
        }
        else {
            School* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newSchool;
        }
    }

    void deleteByName(const string& name) {
        if (!head) return;

        if (head->name == name) {
            School* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        School* current = head;
        while (current->next && current->next->name != name) {
            current = current->next;
        }
        if (current->next) {
            School* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    School* findByName(const string& name) {
        School* current = head;
        while (current) {
            if (current->name == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void display() {
        School* current = head;
        if (!current) {
            cout << "The list is empty.\n";
            return;
        }
        while (current) {
            cout << "Name: " << current->name << "\n";
            cout << "Address: " << current->address << "\n";
            cout << "City: " << current->city << "\n";
            cout << "State: " << current->state << "\n";
            cout << "County: " << current->county << "\n";
            cout << "-------------------------\n";
            current = current->next;
        }
    }
};

int main() {
    SchoolList list;

    string filename = "School.csv"; 

    vector<vector<string>> csvData = CSVReader::readCSV(filename);
    for (const auto& row : csvData) {
        if (row.size() == 5) {  
            string name = row[0];
            string address = row[1];
            string city = row[2];
            string state = row[3];
            string county = row[4];
            list.insertLast(School(name, city, address, state, county));
        }
    }

    cout << "List of Schools:\n";
    list.display();

    string searchName;
    cout << "\nEnter the name of the school to search for: ";
    getline(cin, searchName);
    School* foundSchool = list.findByName(searchName);
    if (foundSchool) {
        cout << "Found School: " << foundSchool->name << endl;
        cout << "Address: " << foundSchool->address << endl;
        cout << "City: " << foundSchool->city << endl;
        cout << "State: " << foundSchool->state << endl;
        cout << "County: " << foundSchool->county << endl;
    }
    else {
        cout << "School not found.\n";
    }

    string deleteName;
    cout << "\nEnter the name of the school to delete: ";
    getline(cin, deleteName);
    list.deleteByName(deleteName);

    cout << "\nList of Schools after deletion:\n";
    list.display();

    return 0;
}
