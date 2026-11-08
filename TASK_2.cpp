#include <iostream>
#include <vector>
using namespace std;

// one node of the linked list
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) {
        val = v;
        next = NULL;
    }
};

// min heap of list nodes, node with smallest value stays on top
class MinHeap {
    vector<ListNode*> arr;

    // keep swapping with parent while node is smaller than parent
    void moveUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (arr[idx]->val < arr[parent]->val) {
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
            if (l < n && arr[l]->val < arr[smallest]->val) smallest = l;
            if (r < n && arr[r]->val < arr[smallest]->val) smallest = r;
            if (smallest == idx) break;
            swap(arr[idx], arr[smallest]);
            idx = smallest;
        }
    }

public:
    bool isEmpty() { return arr.empty(); }

    // add at the end and then move it up
    void push(ListNode* node) {
        arr.push_back(node);
        moveUp(arr.size() - 1);
    }

    // take out the root, put last node on top and fix the heap
    ListNode* pop() {
        ListNode* top = arr[0];
        arr[0] = arr.back();
        arr.pop_back();
        if (!arr.empty()) moveDown(0);
        return top;
    }
};
ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap heap;
    // only the head of each non empty list goes in the heap
    for (int i = 0; i < (int)lists.size(); i++) {
        if (lists[i] != NULL) heap.push(lists[i]);
    }
    // dummy head makes attaching the first node easy
    ListNode dummyHead(0);
    ListNode* tail = &dummyHead;

    while (!heap.isEmpty()) {
        // smallest node among all the lists
        ListNode* smallest = heap.pop();
        tail->next = smallest;
        tail = smallest;
        // next node of that list now enters the heap
        if (smallest->next != NULL) heap.push(smallest->next);
    }
    return dummyHead.next;
}

// reading size numbers and making a linked list from them
ListNode* readList(int size) {
    ListNode* head = NULL;
    ListNode* tail = NULL;
    for (int i = 0; i < size; i++) {
        int x;
        cin >> x;
        ListNode* newNode = new ListNode(x);
        if (head == NULL) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

void printList(ListNode* head) {
    while (head != NULL) {
        cout << head->val;
        if (head->next != NULL) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    int k;
    cout << "Enter number of linked lists: ";
    cin >> k;

    // taking all k lists as input
    vector<ListNode*> lists;
    for (int i = 0; i < k; i++) {
        int size;
        cout << "Enter size of list " << i + 1 << ": ";
        cin >> size;
        cout << "Enter " << size << " sorted elements: ";
        lists.push_back(readList(size));
    }

    cout << "\nInput lists:\n";
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << ": ";
        printList(lists[i]);
    }

    ListNode* mergedHead = mergeKLists(lists);

    cout << "\nMerged list: ";
    printList(mergedHead);

    // deleting every node of the merged list
    while (mergedHead != NULL) {
        ListNode* temp = mergedHead;
        mergedHead = mergedHead->next;
        delete temp;
    }
    return 0;
}