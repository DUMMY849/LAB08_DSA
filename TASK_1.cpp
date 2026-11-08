#include <iostream>
#include <vector>
#include <string>
using namespace std;
// one node of the huffman tree
struct HuffNode {
    char letter;
    int freq;
    int order;
    HuffNode* leftChild;
    HuffNode* rightChild;
    HuffNode(char l, int f, int o) {
        letter = l;
        freq = f;
        order = o;
        leftChild = NULL;
        rightChild = NULL;
    }
};
// min heap made using vector, smallest freq stays on top
class MinHeap {
    vector<HuffNode*> arr;

    // smaller freq wins, tie goes to older node
    bool isSmaller(HuffNode* a, HuffNode* b) {
        if (a->freq != b->freq) return a->freq < b->freq;
        return a->order < b->order;
    }

    // keep swapping with parent while node is smaller than parent
    void moveUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (isSmaller(arr[idx], arr[parent])) {
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else break;
        }
    }

    // keep swapping with smaller child while node is bigger than child
    void moveDown(int idx) {
        int n = arr.size();
        while (true) {
            int smallest = idx;
            int l = 2 * idx + 1;
            int r = 2 * idx + 2;
            // check left and right child one by one
            if (l < n && isSmaller(arr[l], arr[smallest])) smallest = l;
            if (r < n && isSmaller(arr[r], arr[smallest])) smallest = r;
            if (smallest == idx) break;
            swap(arr[idx], arr[smallest]);
            idx = smallest;
        }
    }

public:
    int size() { return arr.size(); }

    // add at the end and then move it up
    void push(HuffNode* node) {
        arr.push_back(node);
        moveUp(arr.size() - 1);
    }

    // take out the root, put last node on top and fix the heap
    HuffNode* pop() {
        HuffNode* top = arr[0];
        arr[0] = arr.back();
        arr.pop_back();
        if (!arr.empty()) moveDown(0);
        return top;
    }
};

HuffNode* buildHuffmanTree(const string& chars, const vector<int>& freqs) {
    MinHeap heap;
    int n = chars.size();
    // put every character in the heap as a leaf node
    for (int i = 0; i < n; i++) heap.push(new HuffNode(chars[i], freqs[i], i));

    int orderCounter = n;
    // join two smallest nodes till only one node is left
    while (heap.size() > 1) {
        HuffNode* first = heap.pop();
        HuffNode* second = heap.pop();
        // $ is just a dummy letter for the internal node
        HuffNode* merged = new HuffNode('$', first->freq + second->freq, orderCounter++);
        merged->leftChild = first;
        merged->rightChild = second;
        heap.push(merged);
    }
    return heap.pop();
}

// preorder: first node, then left, then right
void printCodesPreorder(HuffNode* node, string currentCode, vector<string>& codeList) {
    if (node == NULL) return;
    // only leaf nodes have a code to print
    if (node->leftChild == NULL && node->rightChild == NULL) {
        // if only one character is there, give it code 0
        if (currentCode == "") currentCode = "0";
        cout << node->letter << " : " << currentCode << endl;
        codeList.push_back(currentCode);
        return;
    }
    // left edge adds 0 and right edge adds 1
    printCodesPreorder(node->leftChild, currentCode + "0", codeList);
    printCodesPreorder(node->rightChild, currentCode + "1", codeList);
}

// free the memory of whole tree
void deleteTree(HuffNode* node) {
    if (node == NULL) return;
    deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    delete node;
}

int main() {
    int n;
    cout << "Enter number of characters: ";
    cin >> n;

    // reading the characters one by one
    string chars;
    cout << "Enter " << n << " distinct characters (space separated): ";
    for (int i = 0; i < n; i++) {
        char c;
        cin >> c;
        chars += c;
    }
    // reading the frequency of each character
    vector<int> freqs(n);
    cout << "Enter " << n << " frequencies: ";
    for (int i = 0; i < n; i++) cin >> freqs[i];

    HuffNode* root = buildHuffmanTree(chars, freqs);

    cout << "\nHuffman codes in preorder:\n";
    vector<string> codeList;
    printCodesPreorder(root, "", codeList);

    // printing all codes in a single line also
    cout << "\nCodes in one line: ";
    for (int i = 0; i < (int)codeList.size(); i++) cout << codeList[i] << " ";
    cout << endl;

    deleteTree(root);
    return 0;
}