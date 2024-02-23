#include <iostream>
using namespace std;

class RBTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        int data;
        Node* parent;
        Node* left;
        Node* right;
        Color color;
    };

    Node* root;

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
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

    Node* rotateRight(Node* y) {
        Node* x = y->left;
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

    void insertFixup(Node* z) {
        while (z != nullptr && z->parent != nullptr && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        root = rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    root = rotateRight(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        root = rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    root = rotateLeft(z->parent->parent);
                }
            }
        }

        if (z != nullptr)
            z->color = BLACK;
    }

    Node* insert(Node* root, int data) {
        Node* z = new Node;
        z->data = data;
        z->left = z->right = nullptr;
        z->color = RED;

        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else if (z->data > x->data)
                x = x->right;
            else {
                delete z;
                return root;
            }
        }

        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;

        insertFixup(z);

        return root;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* root, int data) {
        Node* z = nullptr;
        Node* x, * y;

        z = root;
        while (z != nullptr) {
            if (data < z->data)
                z = z->left;
            else if (data > z->data)
                z = z->right;
            else
                break;
        }

        if (z == nullptr)
            return root;

        Color originalColor = z->color;

        if (z->left == nullptr) {
            x = z->right;
            root = transplant(root, z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            root = transplant(root, z, z->left);
        } else {
            y = minValueNode(z->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                root = transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            root = transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (originalColor == BLACK)
            deleteFixup(root, x);

        return root;
    }

    Node* transplant(Node* root, Node* u, Node* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;

        return root;
    }

    void deleteFixup(Node*& root, Node*& x) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    root = rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        root = rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    root = rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    root = rotateRight(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        root = rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    root = rotateRight(x->parent);
                    x = root;
                }
            }
        }

        if (x != nullptr)
            x->color = BLACK;
    }

    void inOrderTraversal(Node* root) {
        if (root != nullptr) {
            inOrderTraversal(root->left);
            std::cout << root->data << " ";
            inOrderTraversal(root->right);
        }
    }

    void preOrderTraversal(Node* root) {
        if (root != nullptr) {
            std::cout << root->data << " ";
            preOrderTraversal(root->left);
            preOrderTraversal(root->right);
        }
    }

    void postOrderTraversal(Node* root) {
        if (root != nullptr) {
            postOrderTraversal(root->left);
            postOrderTraversal(root->right);
            std::cout << root->data << " ";
        }
    }

public:
    RBTree() {
        root = nullptr;
    }

    void insert(int data) {
        root = insert(root, data);
        if (root != nullptr)
            root->color = BLACK;
    }

    void remove(int data) {
        root = deleteNode(root, data);
        if (root != nullptr)
            root->color = BLACK;
    }

    void traverseInOrder() {
        inOrderTraversal(root);
        std::cout << std::endl;
    }

    void traversePreOrder() {
        preOrderTraversal(root);
        std::cout << std::endl;
    }

    void traversePostOrder() {
        postOrderTraversal(root);
        std::cout << std::endl;
    }
};

int main() {
    RBTree rb;

    rb.insert(10);
    rb.insert(20);
    rb.insert(30);
    rb.insert(40);
    rb.insert(50);
    rb.insert(25);

    std::cout << "In-order traversal: ";
    rb.traverseInOrder();

    std::cout << "Pre-order traversal: ";
    rb.traversePreOrder();

    std::cout << "Post-order traversal: ";
    rb.traversePostOrder();

    rb.remove(30);

    std::cout << "In-order traversal after deleting 30: ";
    rb.traverseInOrder();

    return 0;
}
