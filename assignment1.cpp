#include <iostream>
using namespace std;

class Node
{
  public:
   int key;
   Node *left;
   Node *right;

   Node(int key){
       this->key = key;
       left = right  = NULL;
   }
};

Node* insert(Node * root, int key){
	if(root==NULL){
		return new Node(key);
	}

	//rec case 
	if(key < root->key){
		root->left = insert(root->left,key);
	}
	else if(key > root->key){
		root->right = insert(root->right,key);
	}
	return root;

}

//O(H)
bool search(Node * root, int key){
		if(root==NULL){
			return false;
		}
		if(root->key==key){
			return true;
		}
		if(key < root->key){
			return search(root->left,key);
		}
		return search(root->right,key);
}


void printInOrder(Node *root){
	if(root==NULL){
		return;
	}
	//left, root, right
	printInOrder(root->left);
	cout << root-> key <<", ";
	printInOrder(root->right);
}

void printPreOrder(Node *root){
	if(root==NULL){
		return;
	}
	//root, left, right
	cout << root-> key <<", ";
	printPreOrder(root->left);
	printPreOrder(root->right);
}

void printPostOrder(Node *root){
	if(root==NULL){
		return;
	}
	//left, right, root
	printPostOrder(root->left);
	printPostOrder(root->right);
	cout << root-> key <<", ";
}

Node* findMin(Node* root){

		while(root->left!=NULL){
			root = root->left;
		}
		return root;

}

//BST Deletion
Node* remove(Node* root,int key){
		if(root==NULL){
			return NULL;
		}
		else if(key < root->key){
			root->left = remove(root->left,key);
		}
		else if(key > root->key){
			root->right = remove(root->right,key);
		}
		else{
			//when the current node matches with the key
			// No children 
			if(root->left==NULL && root->right==NULL){
				delete root;
				root = NULL;
			}
			// Single Child
			else if(root->left==NULL){
				Node* temp = root;
				root = root->right;
				delete temp;
			}
			else if(root->right==NULL){
				Node*temp = root;
				root = root->left;
				delete temp;
			}
			//2 Children
			else{
				Node* temp = findMin(root->right);
				root->key = temp->key;
				root->right = remove(root->right,temp->key);
			}

		}
		return root;
}

int maxDepth(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    return 1 + max(leftDepth, rightDepth);
}

int minDepth(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftDepth = minDepth(root->left);
    int rightDepth = minDepth(root->right);

    return 1 + min(leftDepth, rightDepth);
}



int main(){
	cout<<"Pranav Modgil"<<endl;
	cout<<"102115009, 3NC1"<<endl;
	Node * root = NULL;
	int arr[] = {8,3,10,1,6,14,4,7,13};

	for(int x : arr){
		root = insert(root,x);
	}


	cout<<"Inorder Traversal :";
	printInOrder(root);
    cout<<endl;
    cout<<"Inorder Traversal :";
	printPreOrder(root);
    cout<<endl;
    cout<<"Inorder Traversal :";
	printPostOrder(root);
	cout<<endl;

	cout<<"After Removing 10"<<endl;
	remove(root,10);
	cout<<"Inorder Traversal :";
	printInOrder(root);
    cout<<endl;
    cout<<"Inorder Traversal :";
	printPreOrder(root);
    cout<<endl;
    cout<<"Inorder Traversal :";
	printPostOrder(root);
	cout<<endl;

	int maxd = maxDepth(root);
	int mind = minDepth(root);
	cout<<endl;

	cout<<"Max Depth: "<<maxd<<endl;
	cout<<"Min Depth: "<<mind<<endl;

	int key;
	return 0;
}