#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Define structure for student record
struct StudentRecord {
    int rollNumber;
    string name;
    float cgpa;
};

// Define structure for AVL tree node
struct AVLNode {
    StudentRecord data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

// Function to get height of a node
int getHeight(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Function to get balance factor of a node
int getBalanceFactor(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Function to update height of a node
void updateHeight(AVLNode* node) {
    if (node != nullptr)
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

// Function to perform right rotation
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// Function to perform left rotation
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Function to insert a new student record into AVL tree
AVLNode* insertAVL(AVLNode* root, StudentRecord newStudent) {
    if (root == nullptr) {
        AVLNode* newNode = new AVLNode;
        newNode->data = newStudent;
        newNode->left = newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (newStudent.rollNumber < root->data.rollNumber)
        root->left = insertAVL(root->left, newStudent);
    else if (newStudent.rollNumber > root->data.rollNumber)
        root->right = insertAVL(root->right, newStudent);
    else // Duplicate roll numbers are not allowed
        return root;

    // Update height of current node
    updateHeight(root);

    // Check balance factor and perform rotations if necessary
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && newStudent.rollNumber < root->left->data.rollNumber)
        return rightRotate(root);

    // Right Right Case
    if (balance < -1 && newStudent.rollNumber > root->right->data.rollNumber)
        return leftRotate(root);

    // Left Right Case
    if (balance > 1 && newStudent.rollNumber > root->left->data.rollNumber) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && newStudent.rollNumber < root->right->data.rollNumber) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Function to perform in-order traversal of AVL tree
void inOrderTraversal(AVLNode* root) {
    if (root != nullptr) {
        inOrderTraversal(root->left);
        cout << "Roll Number: " << root->data.rollNumber << ", Name: " << root->data.name << ", CGPA: " << root->data.cgpa << endl;
        inOrderTraversal(root->right);
    }
}

// Function to delete a student record from AVL tree
AVLNode* deleteNodeAVL(AVLNode* root, int rollNumber) {
    if (root == nullptr)
        return root;

    if (rollNumber < root->data.rollNumber)
        root->left = deleteNodeAVL(root->left, rollNumber);
    else if (rollNumber > root->data.rollNumber)
        root->right = deleteNodeAVL(root->right, rollNumber);
    else {
        // Node with only one child or no child
        if (root->left == nullptr || root->right == nullptr) {
            AVLNode* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp;

            delete temp;
        } else {
            // Node with two children, get the in-order successor (smallest in the right subtree)
            AVLNode* temp = root->right;
            while (temp->left != nullptr)
                temp = temp->left;

            // Copy the in-order successor's data to this node
            root->data = temp->data;

            // Delete the in-order successor
            root->right = deleteNodeAVL(root->right, temp->data.rollNumber);
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // Update height of current node
    updateHeight(root);

    // Check balance factor and perform rotations if necessary
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Function to free memory allocated for AVL tree nodes
void deleteAVLTree(AVLNode* root) {
    if (root != nullptr) {
        deleteAVLTree(root->left);
        deleteAVLTree(root->right);
        delete root;
    }
}

int main() {
    AVLNode* avlRoot = nullptr;

    // Read student information from text file and build initial AVL tree
    ifstream inputFile("student_data.txt");
    int rollNumber;
    string name;
    float cgpa;

    while (inputFile >> rollNumber >> name >> cgpa) {
        StudentRecord newStudent;
        newStudent.rollNumber = rollNumber;
        newStudent.name = name;
        newStudent.cgpa = cgpa;

        avlRoot = insertAVL(avlRoot, newStudent);
    }

    inputFile.close();

    // Perform operations on AVL tree
    // For example, to insert a new student:
    StudentRecord newStudent;
    newStudent.rollNumber = 102015200;
    newStudent.name = "John Doe";
    newStudent.cgpa = 7.8;

    avlRoot = insertAVL(avlRoot, newStudent);

    // Display in-order traversal of AVL tree
    cout << "In-Order Traversal of AVL Tree:" << endl;
    inOrderTraversal(avlRoot);

    // Perform delete operation
    int rollToDelete = 102015070;
    avlRoot = deleteNodeAVL(avlRoot, rollToDelete);

    // Display in-order traversal after deletion
    cout << "\nIn-Order Traversal after Deletion:" << endl;
    inOrderTraversal(avlRoot);

    // Free memory allocated for AVL tree nodes
    deleteAVLTree(avlRoot);

    return 0;
}
1. Define the structure for the student record and AVL tree node.
2. Implement functions to get the height and balance factor of a node, update the height of a node, perform right and left rotations.
3. Implement the function to insert a new student record into the AVL tree.
4. Implement the function to perform in-order traversal of the AVL tree.
5. Implement the function to delete a student record from the AVL tree.
6. Implement the function to free memory allocated for AVL tree nodes.
7. In the main function:
   a. Create an AVL tree root node.
   b. Read student information from a text file and build the initial AVL tree.
   c. Perform operations on the AVL tree, such as inserting a new student record.
   d. Display the in-order traversal of the AVL tree.
   e. Perform the delete operation on a specific student record.
   f. Display the in-order traversal of the AVL tree after deletion.
   g. Free memory allocated for AVL tree nodes.

