#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct School {
    string name;
    string address;
    string city;
    string state;
    string county;

    School(string n, string a, string c, string s, string cn)
        : name(n), address(a), city(c), state(s), county(cn) {
    }
};

struct BSTNode {
    School school;
    BSTNode* left;
    BSTNode* right;

    BSTNode(School s) : school(s), left(nullptr), right(nullptr) {}
};

class SchoolBST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, School school) {
        if (!node) return new BSTNode(school);

        if (school.name < node->school.name)
            node->left = insertRec(node->left, school);
        else
            node->right = insertRec(node->right, school);

        return node;
    }

    BSTNode* searchRec(BSTNode* node, string name) {
        if (!node || node->school.name == name)
            return node;

        if (name < node->school.name)
            return searchRec(node->left, name);
        else
            return searchRec(node->right, name);
    }

    BSTNode* deleteRec(BSTNode* node, string name) {
        if (!node) return nullptr;

        if (name < node->school.name)
            node->left = deleteRec(node->left, name);
        else if (name > node->school.name)
            node->right = deleteRec(node->right, name);
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            BSTNode* temp = node->right;
            while (temp->left) temp = temp->left;
            node->school = temp->school;
            node->right = deleteRec(node->right, temp->school.name);
        }
        return node;
    }

    void inOrder(BSTNode* node) {
        if (!node) return;
        inOrder(node->left);
        cout << node->school.name << " - " << node->school.address << " - " << node->school.city
            << " - " << node->school.state << " - " << node->school.county << endl;
        inOrder(node->right);
    }

    void preOrder(BSTNode* node) {
        if (!node) return;
        cout << node->school.name << " - " << node->school.address << " - " << node->school.city
            << " - " << node->school.state << " - " << node->school.county << endl;
        preOrder(node->left);
        preOrder(node->right);
    }

    void postOrder(BSTNode* node) {
        if (!node) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->school.name << " - " << node->school.address << " - " << node->school.city
            << " - " << node->school.state << " - " << node->school.county << endl;
    }

public:
    SchoolBST() : root(nullptr) {}

    void insert(School school) {
        root = insertRec(root, school);
    }

    School* findByName(string name) {
        BSTNode* result = searchRec(root, name);
        return result ? &result->school : nullptr;
    }

    void deleteByName(string name) {
        root = deleteRec(root, name);
    }

    void displayInOrder() {
        inOrder(root);
    }

    void displayPreOrder() {
        preOrder(root);
    }

    void displayPostOrder() {
        postOrder(root);
    }

    ~SchoolBST() {
        deleteRec(root, "");
    }
};

void loadFromCSV(string filename, SchoolBST& bst) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line, name, address, city, state, county;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, address, ',');
        getline(ss, city, ',');
        getline(ss, state, ',');
        getline(ss, county, ',');

        bst.insert(School(name, address, city, state, county));
    }
    file.close();
}

int main() {
    SchoolBST bst;

    loadFromCSV("School.csv", bst);

    string searchName = "XYZ Academy";
    School* found = bst.findByName(searchName);
    if (found)
        cout << "Found: " << found->name << " at " << found->address << ", " << found->city << ", "
        << found->state << ", " << found->county << endl;
    else
        cout << "School not found." << endl;

    bst.deleteByName("LMN Institute");

    cout << "\nIn-Order Traversal:" << endl;
    bst.displayInOrder();

    cout << "\nPre-Order Traversal:" << endl;
    bst.displayPreOrder();

    cout << "\nPost-Order Traversal:" << endl;
    bst.displayPostOrder();

    return 0;
}
