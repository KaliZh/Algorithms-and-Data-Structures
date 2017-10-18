#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <complex>
#include <string>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <cassert>

using namespace std;

int getInt(int *a, int minn, int maxx) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (!n) {
            printf("Enter integer!\n");
            scanf("%*[^\n]");
        } else if (*a < minn || *a > maxx) {
            n = 0;
            printf("Integer should be in range [%d, %d].\n", minn, maxx);
            scanf("%*[^\n]");
        }
    } while (!n);
    scanf("%*c");
    return 1;
}

template <typename T>
struct Node {
    T Data;
    Node *Next;
};

template <typename T>
class List {
    Node<T> *Head, *Tail;
    int SIZE;
    public:
    List() : Head(NULL), Tail(NULL), SIZE(0) {};
    ~List() {
        while (SIZE) {
            Node<T> *temp = Head->Next;
            delete Head;
            Head = temp;
            --SIZE;
        }
    }
    void free();
    int size();
    Node<T>* head();
    Node<T>* tail();
    void insert(T el);
    void sort();
    void mapAdd(T el);
    void whereMore(T el);
    void reduceSubs(T el);
    void listCat(List<T>* list);
    void getSubs();
    bool findSubs(List<T>* list);
    void print();
    T* test();
    void check();
    private:
    void init();
};

template <typename T>
void List<T>::init() {
    Head = Tail = NULL;
    SIZE = 0;
}

template <typename T>
void List<T>::free() {
    while (SIZE) {
        Node<T> *temp = Head->Next;
        delete Head;
        Head = temp;
        --SIZE;
    }
}

template <typename T>
int List<T>::size() {
    return SIZE;
}

template <typename T>
Node<T>* List<T>::head() {
    return Head;
}

template <typename T>
Node<T>* List<T>::tail() {
    return Tail;
}

template <typename T>
void List<T>::insert(const T el) {
    Node<T> *temp = new Node<T>;
    temp->Next = Head;
    temp->Data = el;
    if (Head) {
        Tail->Next = temp;
        Tail = temp;
    } else {
        Head = Tail = temp;
    }
    SIZE++;
}

template <typename T>
Node<T>* merge(Node<T>* a, Node<T>* b) {
    Node<T>* res = NULL;
    if (!a)
        return b;
    else if (!b)
        return a;
    if (a->Data <= b->Data) {
        res = a;
        res->Next = merge(a->Next, b);
    } else {
        res = b;
        res->Next = merge(a, b->Next);
    }
    return res;
}

template <typename T>
void split(Node<T>* p, Node<T>** pfront, Node<T>** pback) {
    Node<T>* fast; 
    Node<T>* slow;
    if (!p || !p->Next) {
        *pfront = p;
        *pback = NULL;
    } else {
        slow = p;
        fast = p->Next;
        while (fast) {
            fast = fast->Next;
            if (fast) {
                slow = slow->Next;
                fast = fast->Next;
            }
        }
        *pfront = p;
        *pback = slow->Next;
        slow->Next = NULL;
    }
}

template <typename T>
void mergeSort(Node<T>** pHead) {
    Node<T>* head = *pHead;
    Node<T>* a;
    Node<T>* b;
    if (!head || !head->Next)
        return;
    split(head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *pHead = merge(a, b);
}

template <typename T>
void List<T>::sort() {
    Tail->Next = NULL;
    mergeSort(&Head);
    Node<T>* temp = Head;
    while (temp->Next) 
        temp = temp->Next;
    temp->Next = Head;
    Tail = temp;
    if (Head == Tail)
        Head->Next = Tail->Next = NULL;
}

template <typename T>
void List<T>::mapAdd(const T el) {
    Node<T> *temp = Head;
    int sz = SIZE;
    while (sz) {
        temp->Data += el;
        temp = temp->Next;
        --sz;
    }
}

template<typename T>
void List<T>::whereMore(const T el) {
    List<T> *list = new List<T>;
    Node<T> *temp = Head;
    int sz = SIZE;
    while (sz) {
        if (temp->Data > el)
            list->insert(temp->Data);
        temp = temp->Next;
        --sz;
    }
    free();
    init();
    Head = list->head();
    Tail = list->tail();
    SIZE = list->size();
}

template <typename T>
T reduce(Node<T>* temp, T el) {
    if (!temp)
        return el;
    return temp->Data - reduce(temp->Next, el);
}

template <typename T>
void List<T>::reduceSubs(const T el) {
    Tail->Next = NULL;
    T temp = reduce(Head, el);
    free();
    init();
    insert(temp);
}

template <typename T>
void List<T>::listCat(List<T>* list) {
    if (!list->size())
        return;
    SIZE += list->size();
    if (SIZE == list->size()) {
        Head = list->head();
        Tail = list->tail();
    } else {
        Tail->Next = list->head();
        Tail = list->tail();
        Tail->Next = Head;
    }
}

template <typename T>
void List<T>::getSubs() {
    print();
    int n, n1;
    cout << "\nEnter first number of element: ";
    cin >> n;
    cout << "Enter last number of element: ";
    cin >> n1;
    Node<T> *temp = Head;
    List<T> *list = new List<T>;
    for (int i = 1; i <= SIZE; i++, temp = temp->Next)
        if (i >= n && i <= n1)
            list->insert(temp->Data);
    free();
    init();
    listCat(list);
}

template <typename T>
bool List<T>::findSubs(List<T> *list) {
    if (SIZE < list->size())
        return false;
    Node<T> *temp = Head, *head = list->head();
    int sz = SIZE, sz1 = list->size();
    bool bl = false;
    while (sz && sz1) {
        if (temp->Data == head->Data)
            bl = true;
        if (bl && temp->Data != head->Data)
            return false;
        if (bl) {
            --sz1;
            head = head->Next;
        }
        temp = temp->Next;
        --sz;
    }
    if (!bl || sz1)
        return false;
    else return true;
}

template <typename T>
void List<T>::print() {
    Node<T> *temp = Head;
    int sz = SIZE;
    cout << "List:\n"<< sz << "\n";
    while (sz) {
        cout << temp->Data << " ";
        temp = temp->Next;
        --sz;
    }
}

template <typename T>
T* List<T>::test() {
    T *list = new T[SIZE];
    int n = 0;
    Node<T> *temp = Head;
    int sz = SIZE;
    while (sz) {
        list[n++] = temp->Data;
        temp = temp->Next;
        --sz;
    }
    return list;
}

template <typename T>
bool getList(List<T> *temp) {
    int n;
    do {
        cout << "insert to list - 1\n"
            "list is ready - 0\n";
        if (!getInt(&n, 0, 1))
            return false;
        if (n) {
            T el;
            cout << "Enter element: ";
            if (!string("St7complexIiE").compare(typeid(el).name()))
                cout << "\nin the form (n, n1) for example (21, 23)\n";
            cin >> el;
            temp->insert(el);
        }
        temp->print();
        cout << endl;
    } while (n);
    return true;
}

void readme() {
    cout << "/---------------------------------------------------/\n"
        " This program can create list of real numbers,\n"
        " complex numbers and strings, then apply to them\n"
        " functions:\n\n"
        " insert - insert new node to list;\n\n"
        " map - add given element to all nodes of your list;\n\n"
        " where - delete all nodes lower than given element\n"
        " (except complex number - they are not comparable);\n\n"
        " reduce - reduce all nodes to one node by function\n"
        " f(x1, x2) = x1 - x2, with given element (except \n"
        " strings - they do not substract);\n\n"
        " sort - sort list;\n\n"
        " listcat - concatenate given list;\n\n"
        " getsubs - return subsequence of list by given\n"
        " numbers of first and last node;\n\n"
        " findsubs - search for the given subsequence in\n"
        " list;\n\n"
        " print - show current list.\n\n"
        "/---------------------------------------------------/\n";
}

void menuFloat() {
    List<float> ls;
    int n;
    do {
        cout << "\ninsert( 'real number' ) - 1\n"
            "map( 'real number' ) - 2\n"
            "where( 'real number' ) - 3\n"
            "reduce( 'real number' ) - 4\n"
            "sort() - 5\n"
            "listcat( 'list<float>' ) - 6\n"
            "getsubs() - 7\n"
            "findsubs( 'list<float>' ) - 8\n"
            "print() - 9\n"
            "help - 10\n"
            "quit - 0\n";
        if (!getInt(&n, 0, 10))
            return;
        if (n == 1) {
            float temp;
            cout << "Enter real number: ";
            cin >> temp;
            ls.insert(temp);
        } else if (n == 2) {
            float temp;
            cout << "Enter real number: ";
            cin >> temp;
            ls.mapAdd(temp);
        } else if (n == 3) {
            float temp;
            cout << "Enter real number: ";
            cin >> temp;
            ls.whereMore(temp);
        } else if (n == 4) {
            float temp;
            cout << "Enter real number: ";
            cin >> temp;
            ls.reduceSubs(temp);
        } else if (n == 5) { 
            ls.sort();
        } else if (n == 6) {
            List<float> *temp = new List<float>;
            cout << "Enter new list:\n";
            if (!getList(temp))
                return;
            ls.listCat(temp);
        } else if (n == 7) {
            ls.getSubs();
        } else if (n == 8) {
            List<float> *temp = new List<float>;
            cout << "Enter list:\n";
            if (!getList(temp))
                return;
            if (ls.findSubs(temp))
                cout << "found!\n";
            else cout << "not found!\n";
            temp->~List();
        } else if (n == 9) {
            ls.print();
            cout << endl;
        } else if (n == 10) {
            readme();
        }
    } while (n);
    ls.~List();
}

void menuComplex() {
    List<complex<int> > ls;
    int n;
    do {
        cout << "\ninsert( 'complex number' ) - 1\n"
            "map( 'complex number' ) - 2\n"
            "!where() - 3\n"
            "reduce( 'complex number' ) - 4\n"
            "!sort() - 5\n"
            "listcat( 'list<complex>' ) - 6\n"
            "getsubs() - 7\n"
            "findsubs( 'list<complex>' ) - 8\n"
            "print() - 9\n"
            "help - 10\n"
            "quit - 0\n";
        if (!getInt(&n, 0, 10))
            return;
        if (n == 1) {
            int n, n1;
            cout << "Enter integer (real part): ";
            cin >> n;
            cout << "Enter integer (imag part): ";
            cin >> n1;
            complex<int> z(n, n1);
            ls.insert(z);
        } else if (n == 2) {
            int n, n1;
            cout << "Enter integer (real part): ";
            cin >> n;
            cout << "Enter integer (imag part): ";
            cin >> n1;
            complex<int> z(n, n1);
            ls.mapAdd(z);
        } else if (n == 3) {
            cout << "function is not allowed!\n";
        } else if (n == 4) {
            int n, n1;
            cout << "Enter integer (real part): ";
            cin >> n;
            cout << "Enter integer (imag part): ";
            cin >> n1;
            complex<int> z(n, n1);
            ls.reduceSubs(z);
        } else if (n == 5) { 
            cout << "function is not allowed!\n";
        } else if (n == 6) {
            List<complex<int> > *temp = new List<complex<int> >;
            cout << "Enter new list:\n";
            if (!getList(temp))
                return;
            ls.listCat(temp);
        } else if (n == 7) {
            ls.getSubs();
        } else if (n == 8) {
            List<complex<int> > *temp = new List<complex<int> >;
            if (!getList(temp))
                return;
            if (ls.findSubs(temp))
                cout << "found!\n";
            else cout << "not found!\n";
            temp->~List();
        } else if (n == 9) {
            ls.print();
            cout << endl;
        } else if (n == 10) {
            readme();
        }
    } while (n);
    ls.~List();
}

void menuString() {
    List<string> ls;
    int n;
    do {
        cout << "\ninsert( 'string' ) - 1\n"
            "map( 'string' ) - 2\n"
            "where( 'string' ) - 3\n"
            "!reduce() - 4\n"
            "sort() - 5\n"
            "listcat( 'string' ) - 6\n"
            "getsubs() - 7\n"
            "findsubs( 'string' ) - 8\n"
            "print() - 9\n"
            "help - 10\n"
            "quit - 0\n";
        if (!getInt(&n, 0, 10))
            return;
        if (n == 1) {
            string temp;
            cout << "Enter string: ";
            cin >> temp;
            ls.insert(temp);
        } else if (n == 2) {
            string temp;
            cout << "Enter string: ";
            cin >> temp;
            ls.mapAdd(temp);
        } else if (n == 3) {
            string temp;
            cout << "Enter string: ";
            cin >> temp;
            ls.whereMore(temp);
        } else if (n == 4) {
            cout << "strings do not substract!\n";
        } else if (n == 5) { 
            ls.sort();
        } else if (n == 6) {
            List<string> *temp = new List<string>;
            cout << "Enter new list:\n";
            if (!getList(temp))
                return;
            ls.listCat(temp);
        } else if (n == 7) {
            ls.getSubs();
        } else if (n == 8) {
            List<string> *temp = new List<string>;
            cout << "Enter list:\n";
            if (!getList(temp))
                return;
            if (ls.findSubs(temp))
                cout << "found!\n";
            else cout << "not found!\n";
            temp->~List();
        } else if (n == 9) {
            ls.print();
            cout << endl;
        } else if (n == 10) {
            readme();
        }
    } while (n);
    ls.~List();
}
    /* "/---------------------------------------------------/\n"
        " This program can create list of real numbers,\n"
        " complex numbers and strings, then apply to them\n"
        " functions:\n\n"
        " insert - insert new node to list;\n\n"
        " map - add given element to all nodes of your list;\n\n"
        " where - delete all nodes lower than given element\n"
        " (except complex number - they are not comparable);\n\n"
        " reduce - reduce all nodes to one node by function\n"
        " f(x1, x2) = x1 - x2, with given element (except \n"
        " strings - they do not substract);\n\n"
        " sort - sort list;\n\n"
        " listcat - concatenate given list;\n\n"
        " getsubs - return subsequence of list by given\n"
        " numbers of first and last node;\n\n"
        " findsubs - search for the given subsequence in\n"
        " list;\n\n"
        " print - show current list.\n\n"
        "/---------------------------------------------------/\n"; */

template <typename T>
void t(T *t, int n, T *ans) {
    for (int i = 0; i < n; i++)
        assert(t[i] == ans[i]);
    cout << "OK!\n";
    delete[] t;
}

void t1() {
    List<float> ls;
    ls.insert(1);
    ls.insert(2);
    ls.insert(3);
    float ans[] = {1, 2, 3};
    t(ls.test(), ls.size(), ans);
    ls.mapAdd(10);
    ans[0] = 11, ans[1] = 12, ans[2] = 13;
    t(ls.test(), ls.size(), ans);
    ls.whereMore(10);
    t(ls.test(), ls.size(), ans);
    ls.reduceSubs(10);
    ans[0] = 2;
    t(ls.test(), ls.size(), ans);
    ls.sort();
    t(ls.test(), ls.size(), ans);
}

void t2() {
    List<complex<int> > ls;
    ls.insert(complex<int>(1, 2));
    ls.insert(complex<int>(1, 3));
    ls.insert(complex<int>(1, 4));
    complex<int> ans[] = {complex<int>(1, 2), complex<int>(1, 3), 
        complex<int>(1, 4)};
    t(ls.test(), ls.size(), ans);
    ls.mapAdd(complex<int>(10, 0));
    ans[0] = complex<int>(11, 2), ans[1] = complex<int>(11, 3), 
        ans[2] = complex<int>(11, 4);
    t(ls.test(), ls.size(), ans);
    ls.reduceSubs(complex<int>(10, 0));
    ans[0] = complex<int>(1, 3);
    t(ls.test(), ls.size(), ans);
}

void t3() {
    List<string> ls;
    ls.insert("1");
    ls.insert("2");
    ls.insert("3");
    string ans[] = {"1", "2", "3"};
    t(ls.test(), ls.size(), ans);
    ls.mapAdd("10");
    string ans1[] = {"110", "210", "310"};
    t(ls.test(), ls.size(), ans1);
    ls.whereMore("0");
    t(ls.test(), ls.size(), ans1);
    ls.sort();
    t(ls.test(), ls.size(), ans1);
}

void test() {
    t1();
    t2();
    t3();
}

void menu() {
    int n;
    do {
        cout << "\nList<float> - 1\n"
            "List<complex> - 2\n"
            "List<string> - 3\n"
            "Test program - 4\n"
            "Bye! - 0\n";
        if (!getInt(&n, 0, 4))
            return;
        if (n == 1) menuFloat();
        else if (n == 2) menuComplex();
        else if (n == 3) menuString();
        else if (n == 4) test();
    } while (n);
}

int main() {
    readme();
    menu();

	return 0;
}
