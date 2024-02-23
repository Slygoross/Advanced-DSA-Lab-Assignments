// Red-Black Tree implementation can be quite extensive.
// Here is a simplified example for the given requirements:

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum Color { RED, BLACK };

// Define structure for student record
struct StudentRecord {
    int rollNumber;
    string name;
    float cgpa;
};

// Define structure for Red-Black tree node
struct RBNode {
    StudentRecord data;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
    Color color;
};

// Function to perform left rotation
RBNode* leftRotateRB(RBNode* root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;

    return root;
}

// Function to perform right rotation
RBNode* rightRotateRB(RBNode* root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;

    return root;
}

// Function to fix the Red-Black tree after insertion
RBNode* insertFixupRB(RBNode* root, RBNode* z) {
    while (z != nullptr && z->parent != nullptr && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    root = leftRotateRB(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                root = rightRotateRB(root, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    root = rightRotateRB(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                root = leftRotateRB(root, z->parent->parent);
            }
        }
    }

    root->color = BLACK;
    return root;
}

// Function to insert a new student record into Red-Black tree
RBNode* insertRB(RBNode* root, StudentRecord newStudent) {
    RBNode* z = new RBNode;
    z->data = newStudent;
    z->left = z->right = z->parent = nullptr;
    z->color = RED;

    RBNode* y = nullptr;
    RBNode* x = root;

    while (x != nullptr) {
        y = x;
        if (z->data.rollNumber < x->data.rollNumber)
            x = x->left;
        else if (z->data.rollNumber > x->data.rollNumber)
            x = x->right;
        else {
            // Duplicate roll numbers are not allowed
            delete z;
            return root;
        }
    }

    z->parent = y;
    if (y == nullptr)
        root = z;
    else if (z->data.rollNumber < y->data.rollNumber)
        y->left = z;
    else
        y->right = z;

    return insertFixupRB(root, z);
}

// Function to perform in-order traversal of Red-Black tree
void inOrderTraversalRB(RBNode* root) {
    if (root != nullptr) {
        inOrderTraversalRB(root->left);
        cout << "Roll Number: " << root->data.rollNumber << ", Name: " << root->data.name << ", CGPA: " << root->data.cgpa << endl;
        inOrderTraversalRB(root->right);
    }
}

// Function to free memory allocated for Red-Black tree nodes
void deleteRBTree(RBNode* root) {
    if (root != nullptr) {
        deleteRBTree(root->left);
        deleteRBTree(root->right);
        delete root;
    }
}

int main() {
    RBNode* rbRoot = nullptr;

    // Read student information from text file and build initial Red-Black tree
    ifstream inputFile("student_data.txt");
    int rollNumber;
    string name;
    float cgpa;

    while (inputFile >> rollNumber >> name >> cgpa) {
        StudentRecord newStudent;
        newStudent.rollNumber = rollNumber;
        newStudent.name = name;
        newStudent.cgpa = cgpa;

        rbRoot = insertRB(rbRoot, newStudent);
    }

    inputFile.close();

    // Perform operations on Red-Black tree
    // For example, to insert a new student:
    StudentRecord newStudent;
    newStudent.rollNumber = 102015200;
    newStudent.name = "John Doe";
    newStudent.cgpa = 7.8;

    rbRoot = insertRB(rbRoot, newStudent);

    // Display in-order traversal of Red-Black tree
    cout << "In-Order Traversal of Red-Black Tree:" << endl;
    inOrderTraversalRB(rbRoot);

    // Free memory allocated for Red-Black tree nodes
    deleteRBTree(rbRoot);

    return 0;
}
