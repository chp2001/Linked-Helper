#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>

using namespace std;
#include <chrono>
#include <ctime>


class Node {
public:
    string name;
    int data;
    Node* next;
    Node(string s);
    Node(int i);
    Node();
};
Node::Node(string s) {
    name = s;
    next = NULL;
}
Node::Node(int i) {
    name = "";
    data = i;
    next = NULL;
}
Node::Node() {
    name = "";
    next = NULL;
}
Node* newNode(int i) {
    Node* newptr = new Node(i);
    return newptr;
}
class Array {
public:
    string name;
    Node* head;
    int cacheLn;
    bool unchanged;
    bool hashed;
    Node** hash;
    Array(string s);
    Array(Node* n);
    Array();
    Node* endOfList();
    Node* index(int i);
    int len();
    void add(Node* n);
    bool del(int i);
    bool del_(int i);//behind scenes, don't use directly
    bool insert(Node* n, int i);
    bool insert_(Node* n, int i);
    bool swapIndexes(int i, int j);
    bool swapIndexes_(int i, int j);
    bool simpleDisconnect_(int i);
    void denote_change();
    void do_hash();
};
Array::Array(string s) {
    name = s;
    head = NULL;
    cacheLn = 0;
    unchanged = true;
    hashed = false;
}
Array::Array(Node* n) {
    name = "";
    head = n;
    cacheLn = 1;
    unchanged = true;
    hashed = false;
}
Array::Array() {
    name = "";
    head = NULL;
    cacheLn = 0;
    unchanged = true;
    hashed = false;
}
Node* Array::endOfList() {
    if (head == NULL) {
        return NULL;
    }
    else {
        Node* handler = head;
        while (handler->next != NULL) {
            handler = handler->next;
        }
        return handler;
    }
}
Node* Array::index(int i) {
    if (head == NULL or len()<i+1) {
        return NULL;
    }
    else if (hashed) {
        return hash[i];
    }
    else {
        Node* handler = head;
        int j = 0;
        while (handler->next != NULL) {
            if (j == i) {
                break;
            }
            handler = handler->next;
            j++;
        }
        if (j == i) {
            return handler;
        }
        else {
            return NULL;
        }
    }
}
void Array::denote_change() {
    unchanged = false;
    if (hashed) {
        delete hash;
    }
    hashed = false;
}
int Array::len() {
    if (head == NULL) {
        return 0;
    }
    else {
        if (unchanged) {
            return cacheLn;
        }
        Node* handler = head;
        int i = 1;
        while (handler->next != NULL) {
            handler = handler->next;
            i++;
        }
        cacheLn = i;
        unchanged = true;
        return i;
    }
}
void Array::add(Node* n) {
    if (head == NULL) {
        head=n;
    }
    else {
        Node* handler = head;
        while (handler->next != NULL) {
            handler = handler->next;
        }
        handler->next=n;
    }
    denote_change();
}
bool Array::del(int i) {
    if (del_(i)) {
        denote_change();
        return true;
    }
    else {
        return false;
    }
}
bool Array::del_(int i) {
    int length = len();
    if (head == NULL or length <= i) {
        return false;
    }
    else {
        if (length == 1) {//if we are deleting the head, and there are no other items, aka none before or after
            delete index(0);
            head = NULL;
            return true;
        }
        Node* before = head;
        Node* deathrow = head;
        Node* handler = head;
        Node* after = NULL;
        if (i == 0) {//if we are deleting the head, aka none before, some after
            deathrow = head;
            handler = head->next;
            delete deathrow;
            head = handler;
            return true;
        }
        if (length == i + 1) {//if our item is at the end of the list, aka none after, some before
            before = index(i - 1);
            deathrow = before->next;
            delete deathrow;
            before->next = NULL;
            return true;
        }
        if (length > i + 1) {//if there is an item both before and after our target
            before = index(i - 1);
            deathrow = before->next;
            handler = deathrow->next;
            delete deathrow;
            before->next = handler;
            return true;
        }
    }
}
bool Array::insert(Node* n, int i) {
    if (insert_(n, i)) {
        denote_change();
        return true;
    }
    else {
        return false;
    }
}
bool Array::insert_(Node* n, int i) {
    int length = len();
    if (length==i) {//adding at end of list is easy!
        add(n);
        return true;
    }
    if (head == NULL or length <= i) {//we won't insert if there aren't enough terms, or we are trying to add at an index beyond the end
        return false;
    }
    else {
        Node* before = head;
        Node* handler = head;
        Node* after = NULL;
        if (i == 0) {//if we are inserting at the head, aka none before, some after
            handler = head;
            head = n;
            head->next = handler;
            return true;
        }
        if (length >= i + 1) {//middle of the list
            before = index(i - 1);
            handler = before->next;
            n->next = handler;
            before->next = n;
            return true;
        }
    }
    return false;
}
bool Array::swapIndexes(int i, int j) {
    if (swapIndexes_(i,j)) {
        denote_change();
        return true;
    }
    else {
        return false;
    }
}
bool Array::swapIndexes_(int i, int j) {
    int length = len();
    if (length <= i or length <= j or i==j) {
        return false;
    }
    if (i > j) {
        int temp = j;
        j = i;
        i = temp;
    }
    Node* n1 = index(i);
    Node* n2 = index(j);
    if (n1!=NULL and n2!=NULL) {
        if (j == i + 1) {
            if (i == 0) {
                head = n2;
                n1->next = n2->next;
                n2->next = n1;
                return true;
            }
            else {
                Node* before = index(i - 1);
                before->next = n2;
                n1->next = n2->next;
                n2->next = n1;
                return true;
            }
        }
        else {
            Node* b1 = index(i - 1);
            Node* b2 = index(j - 1);
            Node* a1 = index(i + 1);
            Node* a2 = index(j + 1);
            if (i > 0) {
                b1->next = n2;
            }
            else {
                head = n2;
            }
            b2->next = n1;
            n2->next = a1;
            n1->next = a2;
            return true;
        }
        return false;
    }
    return false;
}
void Array::do_hash() {
    hash = new Node*[len()];
    Node* handler = head;
    for (int i = 0;i < len();i++) {
        hash[i] = handler;
        handler = handler->next;
    }
    hashed = true;
}
Array* newArray(string s) {
    Array* newptr = new Array(s);
    return newptr;
}
double curtime() {
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return (double)nanoseconds* 1e-9;
}
int main(int argc, char* argv[]) {
    double beforetime = curtime();
    double offset = curtime() - beforetime;
    printf_s("getting curtime took %f seconds\n", offset);
    offset = 0;
    Array* mainList = newArray("main");
    beforetime = curtime();
    for (int i = 0;i < 60000;i++) {
        mainList->add(newNode(60000-i));
    }
    double after = curtime();
    printf_s("adding %d terms took %f seconds\n", mainList->len(), after - beforetime-offset);
    beforetime = curtime();
    for (int i = 0;i < 35;i++) {
        mainList->del(0);
    }
    printf_s("deleting 35 terms took %f seconds\n", curtime() - beforetime-offset);
    beforetime = curtime();
    int curlen = mainList->len();
    for (int i = 0;i < mainList->len()/2;i++) {
        int curlen = mainList->len();
        if (not mainList->insert(newNode(1337), 2 * i + 1)) {
            printf_s("   failed at %d when length was %d, attempted to insert at %d\n", i,curlen,2*i+1);
        }
        else {
            //printf_s("succeeded at %d when length was %d, inserted at %d\n", i, curlen, 2 * i + 1);
        }
    }
    after = curtime();
    printf_s("inserting %d terms took %f seconds\n",mainList->len()-curlen, after - beforetime-offset);
    int j = 0;
    beforetime = curtime();
    for (int i = 0;i < mainList->len();i++,j++) {
        if (j >= 10) {
            //printf_s("\n");
            j = 0;
        }
        auto data = mainList->index(i)->data;
        //printf_s("%d ", mainList->index(i)->data);
    }
    after = curtime();
    printf_s("calling all %d terms took %f seconds\n",mainList->len(), after - beforetime - offset);
    //printf_s("\nvalue at index 3 is %d\n",mainList->index(3)->data);
    curlen = mainList->len();
    beforetime = curtime();
    for (int i = 0;i < curlen;i++) {
        int curlen = mainList->len();
        if (not /*mainList->swapIndexes(i, mainList->len() - i -*/ 1/*)*/) {
            printf_s("   failed at %d when length was %d, attempted to swap at %d\n", i, curlen, mainList->len() - i);
        }
        else {
            //printf_s("succeeded at %d when length was %d, swapped at %d\n", i, curlen, mainList->len() - i);
        }
    }
    after = curtime();
    printf_s("swapping all %d terms took %f seconds\n", mainList->len(), after - beforetime - offset);
    beforetime = curtime();
    mainList->do_hash();
    after = curtime();
    printf_s("hashing all %d terms took %f seconds\n", mainList->len(), after - beforetime - offset);
    beforetime = curtime();
    for (int i = 0;i <mainList->len();i++, j++) {
        if (j >= 10) {
            //printf_s("\n");
            j = 0;
        }
        auto data = mainList->index(i)->data;
        //printf_s("%d ", mainList->index(i)->data);
    }
    after = curtime();
    printf_s("calling all %d terms from hash took %f seconds\n", mainList->len(), after - beforetime - offset);
    return 0;
}