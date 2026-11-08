#include <iostream>
#include <vector>
#include <string>
using namespace std;

// one node of the avl tree, count is used for duplicate values
struct AvlNode {
    int value;
    int count;
    int height;
    AvlNode* leftChild;
    AvlNode* rightChild;
    AvlNode(int v) {
        value = v;
        count = 1;
        height = 1;
        leftChild = NULL;
        rightChild = NULL;
    }
};

class AvlTree {
    AvlNode* root;

    // height of empty tree is 0
    int getHeight(AvlNode* node) {
        if (node == NULL) return 0;
        return node->height;
    }


    int getBalance(AvlNode* node) {// balance factor = left height - right height

        if (node == NULL) return 0;
        return getHeight(node->leftChild) - getHeight(node->rightChild);
    }

    // height is 1 + height of the taller child
    void fixHeight(AvlNode* node) {
        node->height = 1 + max(getHeight(node->leftChild), getHeight(node->rightChild));
    }

    // y goes down to the right and its left child x comes up
    AvlNode* rotateRight(AvlNode* y) {
        AvlNode* x = y->leftChild;
        AvlNode* middle = x->rightChild;
        x->rightChild = y;
        y->leftChild = middle;
        // lower node first, then the upper node
        fixHeight(y);
        fixHeight(x);
        return x;
    }

    // x goes down to the left and its right child y comes up
    AvlNode* rotateLeft(AvlNode* x) {
        AvlNode* y = x->rightChild;
        AvlNode* middle = y->leftChild;
        y->leftChild = x;

        x->rightChild = middle;
        fixHeight(x);
        fixHeight(y);
        return y;
    }
    AvlNode* rebalance(AvlNode* node) {// checks the balance of node and does rotation if needed

        fixHeight(node);
        int balance = getBalance(node);

        // left side is heavy
        if (balance > 1) {
            if (getBalance(node->leftChild) >= 0) {
                cout << "  [LL case at " << node->value << ", right rotation]" << endl;
            } else {
                // left child is right heavy, so first rotate it to the left
                cout << "  [LR case at " << node->value << ", left-right rotation]" << endl;
                node->leftChild = rotateLeft(node->leftChild);
            }
            return rotateRight(node);
        }

        // right side is heavy
        if (balance < -1) {
            if (getBalance(node->rightChild) <= 0) {
                cout << "  [RR case at " << node->value << ", left rotation]" << endl;
            } else {
                // right child is left heavy, so first rotate it to the right
                cout << "  [RL case at " << node->value << ", right-left rotation]" << endl;
                node->rightChild = rotateRight(node->rightChild);
            }
            return rotateLeft(node);
        }
        return node;
    }

    AvlNode* insertNode(AvlNode* node, int value) {
        // empty place found, make a new node here
        if (node == NULL) return new AvlNode(value);
        if (value < node->value) node->leftChild = insertNode(node->leftChild, value);
        else if (value > node->value) node->rightChild = insertNode(node->rightChild, value);
        else {
            // same value already there, just increase the count
            node->count++;
            return node;
        }
        // rebalance on the way back up
        return rebalance(node);
    }
    // leftmost node is the smallest one
    AvlNode* smallestNode(AvlNode* node) {
        while (node->leftChild != NULL) node = node->leftChild;
        return node;
    }

    // wholeNode true means delete the node even if count is more than 1
    AvlNode* removeNode(AvlNode* node, int value, bool wholeNode) {
        if (node == NULL) return NULL;

        if (value < node->value) node->leftChild = removeNode(node->leftChild, value, wholeNode);
        else if (value > node->value) node->rightChild = removeNode(node->rightChild, value, wholeNode);
        else {
            // duplicate value, so only reduce the count
            if (!wholeNode && node->count > 1) {
                node->count--;
                return node;
            }
            // zero or one child, the child takes its place
            if (node->leftChild == NULL || node->rightChild == NULL) {
                AvlNode* child = node->leftChild != NULL ? node->leftChild : node->rightChild;
                delete node;
                return child;
            }
            // two children, so copy the inorder successor and delete that one
            AvlNode* successor = smallestNode(node->rightChild);
            node->value = successor->value;
            node->count = successor->count;
            node->rightChild = removeNode(node->rightChild, successor->value, true);
        }
        return rebalance(node);
    }

    // reverse inorder gives largest values first
    void collectLargest(AvlNode* node, int k, vector<int>& result) {
        if (node == NULL || (int)result.size() >= k) return;
        collectLargest(node->rightChild, k, result);
        for (int i = 0; i < node->count && (int)result.size() < k; i++) result.push_back(node->value);
        collectLargest(node->leftChild, k, result);
    }

    // normal inorder gives smallest values first
    void collectSmallest(AvlNode* node, int k, vector<int>& result) {
        if (node == NULL || (int)result.size() >= k) return;
        collectSmallest(node->leftChild, k, result);
        for (int i = 0; i < node->count && (int)result.size() < k; i++) result.push_back(node->value);
        collectSmallest(node->rightChild, k, result);
    }

    // prints the tree sideways, more spaces means deeper level
    void showTree(AvlNode* node, int depth) {
        if (node == NULL) return;
        showTree(node->rightChild, depth + 1);
        cout << string(depth * 6, ' ') << node->value;
        if (node->count > 1) cout << "(x" << node->count << ")";
        cout << " [h=" << node->height << ", bf=" << getBalance(node) << "]" << endl;
        showTree(node->leftChild, depth + 1);
    }

    void clearTree(AvlNode* node) {
        if (node == NULL) return;
        clearTree(node->leftChild);
        clearTree(node->rightChild);
        delete node;
    }

public:
    AvlTree() { root = NULL; }
    ~AvlTree() { clearTree(root); }

    bool isEmpty() { return root == NULL; }

    void insertValue(int value) { root = insertNode(root, value); }

    // removes only one occurrence of the value
    void deleteOne(int value) { root = removeNode(root, value, false); }

    vector<int> topKLargest(int k) {
        vector<int> result;
        collectLargest(root, k, result);
        return result;
    }

    vector<int> topKSmallest(int k) {
        vector<int> result;
        collectSmallest(root, k, result);
        return result;
    }

    void printTree() {
        if (root == NULL) {
            cout << "  (tree is empty)" << endl;
            return;
        }
        cout << "  Tree (right side on top, left side on bottom):" << endl;
        showTree(root, 0);
    }
};

void printVector(const vector<int>& v) {
    for (int i = 0; i < (int)v.size(); i++) cout << v[i] << " ";
    cout << endl;
}

// asks the user which criteria to use
int askCriteria() {
    int choice;
    cout << "Criteria: 1. Maximum  2. Minimum  3. Sum of top k maximum -> ";
    cin >> choice;
    return choice;
}

int main() {
    AvlTree tree;
    int option;

    do {
        cout << "\n1. Insert multiple elements\n2. Query top k\n3. Delete top k\n4. Print tree\n0. Exit\nChoose: ";
        cin >> option;

        if (option == 1) {
            int howMany;
            cout << "How many elements to insert: ";
            cin >> howMany;
            for (int i = 0; i < howMany; i++) {
                int x;
                cout << "Enter element " << i + 1 << ": ";
                cin >> x;
                tree.insertValue(x);
                // tree is printed after every insertion
                cout << "After inserting " << x << ":" << endl;
                tree.printTree();
            }
        }
        else if (option == 2) {
            int k;
            cout << "Enter k: ";
            cin >> k;
            int criteria = askCriteria();
            if (criteria == 1) {
                cout << "Top " << k << " maximum: ";
                printVector(tree.topKLargest(k));
            } else if (criteria == 2) {
                cout << "Top " << k << " minimum: ";
                printVector(tree.topKSmallest(k));
            } else if (criteria == 3) {
                // sum of the k largest elements
                vector<int> top = tree.topKLargest(k);
                long long total = 0;
                for (int i = 0; i < (int)top.size(); i++) total += top[i];
                cout << "Elements: ";
                printVector(top);
                cout << "Sum = " << total << endl;
            } else cout << "Wrong criteria" << endl;
        }
        else if (option == 3) {
            int k;
            cout << "Enter k: ";
            cin >> k;
            int criteria = askCriteria();
            if (criteria < 1 || criteria > 3) {
                cout << "Wrong criteria" << endl;
                continue;
            }
            // first collect the top k, then delete them one by one
            vector<int> toDelete = (criteria == 2) ? tree.topKSmallest(k) : tree.topKLargest(k);
            for (int i = 0; i < (int)toDelete.size(); i++) {
                tree.deleteOne(toDelete[i]);
                cout << "After deleting " << toDelete[i] << ":" << endl;
                tree.printTree();
            }
            if ((int)toDelete.size() < k) cout << "Only " << toDelete.size() << " elements were present" << endl;
        }
        else if (option == 4) {
            tree.printTree();
        }
    } while (option != 0);

    return 0;
}