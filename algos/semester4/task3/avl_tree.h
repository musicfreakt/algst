#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iterator>
#include <stack>
#include <string>

using namespace std;

struct node // Структура узла дерева
{
    char delta; // баланс (разность поддеревьев узла)
    int key; // ключ узла
    node *nodes[2]; // левое и правое поддерево

    node(int k): key(k), delta(0){nodes[0] = nodes[1] = nullptr;}
    node(const node &) = delete;
    node(){delete nodes[1]; delete nodes[0];}
};

using Stack = stack<pair<Node*, int>>;

//ИТЕРАТОР ЧТЕНИЯ — нужны сравнения, разыменования, инкремент
struct myiter: public std::iterator<std::forward_iterator_tag, int>
{
    node * ptr; //Реальный указатель на элемент контейнера
    Stack st; // стек с путем от корня дерева

    myiter(node *p = nullptr) : ptr(p) {}
    myiter(node *p = nullptr, const Stack &&St) : ptr(p), st(move(St)) {}
    bool operator == (const myiter & other) const { return ptr == other.ptr; }
    bool operator != (const myiter & other) const { return !(*this == other); }
    // bool operator != (const myiter & other) const { return ptr != other.ptr;}
    myiter operator++(); //Ключевая операция — инкремент по контейнеру
    myiter operator++(int) { myiter temp(*this); ++*this; return temp;}
    pointer operator->() { return &ptr->key; } //Разыменование косвенное
    reference operator*() { return ptr->key; } //Разыменование прямое
};


#endif
