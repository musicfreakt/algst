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
    bool operator== (const myiter & other) const { return ptr == other.ptr; }
    bool operator!= (const myiter & other) const { return !(*this == other); }
    // bool operator != (const myiter & other) const { return ptr != other.ptr;}
    myiter operator++(); //Ключевая операция — инкремент по контейнеру
    myiter operator++(int) { myiter temp(*this); ++*this; return temp;}
    pointer operator->() { return &ptr->key; } //Разыменование косвенное
    reference operator*(){ return ptr->key; } //Разыменование прямое
};

//ИТЕРАТОР ВСТАВКИ
template<typename Container, typename Iter = myiter>
struct outiter: public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
    protected:
        Container& container; // Контейнер для вставки элементов
        Iter iter; // текущее значение итератора чтения
    public:
        outiter(Container &c, Iter it) : container(c), iter(it) {}
        const outiter<Contaiter>& operator= (const typename Contaiter::value_type& value)
        {
            iter = container.insert(iter, value).first;
            return *this;
        }

        const outiter<Contaiter>& operator= (const outiter<Contaiter> &) // Присваивание копии фиктивное
        {
            return *this
        }
        outiter<Contaiter>& operator* () {return *this} // Разыменование - пустая операция
        outiter<Contaiter>& operator++ () {return *this} // Инкремент - пустая операция
        outiter<Contaiter>& operator++ (int) {return *this} // Инкремент - пустая операция
};

// функция для создания итератора вставки
template<typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container& c, Iter It)
{
    return outiter<Container, Iter>(c, it);
}

#endif
