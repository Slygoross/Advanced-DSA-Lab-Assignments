#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
};

class SplayTree {
private:
    Node* root;

    Node* rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    Node* splay(Node* root, int key) {
        if (root == nullptr || root->key == key)
            return root;

        if (root->key > key) {
            if (root->left == nullptr)
                return root;
            if (root->left->key > key) {
                root->left->left = splay(root->left->left, key);
                root = rightRotate(root);
            } else if (root->left->key < key) {
                root->left->right = splay(root->left->right, key);
                if (root->left->right != nullptr)
                    root->left = leftRotate(root->left);
            }
            return (root->left == nullptr) ? root : rightRotate(root);
        } else {
            if (root->right == nullptr)
                return root;
            if (root->right->key > key) {
                root->right->left = splay(root->right->left, key);
                if (root->right->left != nullptr)
                    root->right = rightRotate(root->right);
            } else if (root->right->key < key) {
                root->right->right = splay(root->right->right, key);
                root = leftRotate(root);
            }
            return (root->right == nullptr) ? root : leftRotate(root);
        }
    }

    Node* findMin(Node* node) {
        if (node == nullptr)
            return nullptr;
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* findMax(Node* node) {
        if (node == nullptr)
            return nullptr;
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    Node* deleteNode(Node* root, int key) {
        if (root == nullptr)
            return nullptr;
        root = splay(root, key);
        if (root->key != key)
            return root;
        Node* temp;
        if (root->left == nullptr) {
            temp = root;
            root = root->right;
        } else {
            temp = root;
            root = splay(root->left, key);
            root->right = temp->right;
        }
        delete temp;
        return root;
    }

    void inorder(Node* root) {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->key << " ";
            inorder(root->right);
        }
    }

    void preorder(Node* root) {
        if (root != nullptr) {
            cout << root->key << " ";
            preorder(root->left);
            preorder(root->right);
        }
    }

    void postorder(Node* root) {
        if (root != nullptr) {
            postorder(root->left);
            postorder(root->right);
            cout << root->key << " ";
        }
    }

public:
    SplayTree() {
        root = nullptr;
    }

    void insert(int key) {
        if (root == nullptr) {
            root = new Node;
            root->key = key;
            root->left = root->right = nullptr;
            return;
        }
        root = splay(root, key);
        if (root->key == key)
            return;
        Node* newNode = new Node;
        newNode->key = key;
        if (root->key > key) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }
        root = newNode;
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    void removeMin() {
        if (root != nullptr) {
            Node* minNode = findMin(root);
            if (minNode != nullptr)
                root = deleteNode(root, minNode->key);
        }
    }

    void removeMax() {
        if (root != nullptr) {
            Node* maxNode = findMax(root);
            if (maxNode != nullptr)
                root = deleteNode(root, maxNode->key);
        }
    }

    void traverseInorder() {
        inorder(root);
        cout << endl;
    }

    void traversePreorder() {
        preorder(root);
        cout << endl;
    }

    void traversePostorder() {
        postorder(root);
        cout << endl;
    }
};

int main() {
    SplayTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(20);
    tree.insert(7);
    
    cout << "Inorder traversal: ";
    tree.traverseInorder();

    tree.remove(7);
    cout << "After deleting 7: ";
    tree.traverseInorder();

    cout << "Preorder traversal: ";
    tree.traversePreorder();

    cout << "Postorder traversal: ";
    tree.traversePostorder();

    return 0;
}
