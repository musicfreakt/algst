#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iterator>
#include <stack>
#include <string>

using namespace std;

// УЗЕЛ ДЕРЕВА
struct node
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

myiter& myiter::operator++()
{
    if (!ptr)
        return *this;

    if (ptr->nodes[1])
    {
        st.push(make_pair(ptr, 1));
        ptr = ptr->nodes[1];
        while (ptr->nodes[0])
        {
            st.push(make_pair(ptr, 0));
            ptr = ptr->nodes[0];
        }
        // for(;ptr->nodes[0];ptr = ptr->nodes[0]) st.push(make_pair(ptr, 0));
    }
    else
    {
        pair <node*, int> pp(ptr, 1);
        while (!st.empty() && pp.second) {pp = st.top(); st.pop();}
        if (pp.second)
            ptr = nullptr;
        else
            ptr = pp.first;
    }

    return *this;
}

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

// АВЛ ДЕРЕВО
class tree
{
    static size_t tags;
    char tag;
    node *root;
    size_t h, count; // высота и мощность дерева

    public:
        using key_type = int;
        using value_type = int;
        using key_compare = less<int>;

        void swap(tree & other)
        {
            std::swap(tag, other.tag);
            std::swap(root, other.root);
            std::swap(h, other.h);
            std::swap(count, other.count);
        }

        myiter insert(const myiter& where, const int& k);
        myiter insert(const int& k, const myiter& where = myiter(nullptr))
        {return insert(where, k).first;}
        void display(int = 1);
        myiter begin() const;
        myiter end() const {return myiter(nullptr)}
        pair<myiter, bool> insert(int, myiter = myiter(nullptr))
        pair<myiter, bool> remove(int);
        int size(){return count;}
        myiter find(int) const;
        bool empty() { return(root == nullptr); }

        tree & operator= (const tree & other)
        {
            tree temp;
            for (auto x: other)
                temp.insert(x);
            swap(temp);
            return *this;
        }
        tree & operator= (tree && other) {swap(other); return *this;}
        tree & operator|= (const tree &);
        tree operator| (const tree & other) const {tree res(*this); return (res |= other);}
        tree & operator&= (const tree &);
        tree operator& (const tree & other) const {tree res(*this); return (res &= other);}
        tree & operator-= (const tree &);
        tree operator- (const tree & other) const {tree res(*this); return (res -= other);}
        tree & operator^= (const tree &);
        tree operator^ (const tree & other) const {tree res(*this); return (res ^= other);}

        tree(): tag('A' + tags++), root(nullptr), h(0), n(0) {}
        template<typename it>
        tree(it, it); // формирование ддп из отрезка
        tree(const tree &other): tree() {for(auto x = other.begin(); x != other.end(); ++x) insert(*x);}
        tree(tree && other): tree() {swap(other);}
        ~tree(){delete root;}
}

myiter tree::begin() const
{
    Stack st;
    Node *p(root);
    if (p)
    {
        while (p->nodes[0])
        {
            st.push(make_pair(p, 0));
            p = p->nodes[0];
        }
        // for(;p->nodes[0];p = p->nodes[0]) st.push(make_pair(p, 0));
    }

    return myiter(p, move(st));
}

#endif
