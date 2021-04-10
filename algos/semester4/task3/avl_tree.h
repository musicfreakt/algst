#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iterator>
#include <stack>
#include <string>

using namespace std;

int setval(string &s, int pos, int val)
{
    string t(to_string(val));
    for (auto p: t) s[pos++] = p;
    return t.size();
}

// УЗЕЛ ДЕРЕВА
struct node
{
    int key; // ключ узла
    signed char balance_factor; // баланс (разность поддеревьев узла)
    node* nodes[2]; // левое и правое поддерево

    void display(int, int, int);
    node(int k): key(k), delta(0){nodes[0] = nodes[1] = nullptr;}
    node(const node& ) = delete;
    ~node(){delete nodes[1]; delete nodes[0];}
};


void node::display(int row, int col, int depth)
{
    string sb = "-0+";
    if ((row > MAXROW) || (col < 0) || (col > MAXCOL)) return;
    if (row > MAXOUT)
    {
        screen[row].replace(col, 3, "+++");
        return;
    }
    setval(screen[row], col, key);
    screen[row+1][col] = sb[balance_factor + 1];
    if (nodes[0])
        nodes[0]->display(row + 1, col - OFFSET[depth], depth+1);
    if (nodes[1])
        nodes[1]->display(row + 1, col + OFFSET[depth], depth+1);

}


using Stack = stack<pair<Node*, int>>;

//ИТЕРАТОР ЧТЕНИЯ (нужны сравнения, разыменования, инкремент)
struct tree_iterator: public std::iterator<std::forward_iterator_tag, int>
{
    node* ptr; //Реальный указатель на элемент контейнера
    Stack st; // стек с путем от корня дерева

    tree_iterator(node *p = nullptr) : ptr(p) {}
    tree_iterator(node *p = nullptr, const Stack &&St) : ptr(p), st(move(St)) {}

    bool operator== (const tree_iterator & other) const { return ptr == other.ptr; }
    bool operator!= (const tree_iterator & other) const { return !(*this == other); }
    tree_iterator operator++(); // Инкремент по контейнеру
    tree_iterator operator++(int) { tree_iterator temp(*this); ++*this; return temp; }
    pointer operator->() { return &ptr->key; } //Разыменование косвенное
    reference operator*(){ return ptr->key; } //Разыменование прямое
};

tree_iterator& tree_iterator::operator++()
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

// ИТЕРАТОР ВСТАВКИ (нужны присваивание, разыменование , инкремент)
template<typename Container, typename Iter = tree_iterator>
class outiterator: public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
    protected:
        Container& container; // Контейнер для вставки элементов
        Iter iter; // текущее значение итератора чтения
    public:
        outiterator(Container &c, Iter it) : container(c), iter(it) {}
        const outiterator<Contaiter>& operator= (const typename Contaiter::value_type& value)
        {
            iter = container.insert(iter, value).first;
            return *this;
        }

        // Присваивание копии фиктивное
        const outiterator<Contaiter>& operator= (const outiterator<Contaiter> &) { return *this }
        outiterator<Contaiter>& operator* () {return *this} // Разыменование - пустая операция
        outiterator<Contaiter>& operator++ () {return *this} // Инкремент - пустая операция
        outiterator<Contaiter>& operator++ (int) {return *this} // Инкремент - пустая операция
};

// функция для создания итератора вставки
template<typename Container, typename Iter>
inline outiterator<Container, Iter> out_inserter(Container& c, Iter It)
{
    return outiterator<Container, Iter>(c, it);
}

// АВЛ ДЕРЕВО
class tree
{
    static size_t tags;
    char tag;
    node *root;
    size_t h, count; // высота и мощность дерева

    node* find_element(node*, int) const;
    public:
        using key_type = int;
        using value_type = int;
        using key_compare = less<int>;

        // allocation/deallocation:
        tree(): tag('A' + tags++), root(nullptr), h(0), count(0) {}
        template<typename it>
        tree(it ibegin, it iend): tree() {for(auto x = ibegin; x != iend; ++x) insert(*x);}
        tree(const tree &other): tree() {for(auto x = other.begin(); x != other.end(); ++x) insert(*x);}
        tree(tree && other): tree() { swap(other); }
        ~tree(){ delete root; }
        void swap(tree &);

        // accessors:
        tree_iterator begin() const;
        tree_iterator end() const { return tree_iterator(nullptr); }
        bool empty() const { return (root == nullptr); }
        // bool empty() const { return (count == 0); }
        int size() const { return count; }

        // insert/erase
        pair<tree_iterator, bool> insert(int, tree_iterator = tree_iterator(nullptr))
        tree_iterator insert(const tree_iterator&, const int&);
        tree_iterator insert(const int& k, const tree_iterator& where = tree_iterator(nullptr))
        { return insert(where, k).first; }
        pair<tree_iterator, bool> erase(int);

        // set operations:
        void display();
        tree_iterator find(int);
        tree & operator= (const tree &);
        tree & operator= (tree && other) {swap(other); return *this;}
        tree & operator|= (const tree &);
        tree operator| (const tree & other) const {tree res(*this); return (res |= other);}
        tree & operator&= (const tree &);
        tree operator& (const tree & other) const {tree res(*this); return (res &= other);}
        tree & operator-= (const tree &);
        tree operator- (const tree & other) const {tree res(*this); return (res -= other);}
        tree & operator^= (const tree &);
        tree operator^ (const tree & other) const {tree res(*this); return (res ^= other);}
}

void tree::swap(tree & other)
{
    std::swap(tag, other.tag);
    std::swap(root, other.root);
    std::swap(h, other.h);
    std::swap(count, other.count);
}

tree_iterator tree::begin() const
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

    return tree_iterator(p, move(st));
}

node* tree::find_element(node* _node, int key) const
{
    if (_node == nullptr ||
        (!(key < _node->key) && !(_node->key < key)))
        return _node;
    else if (key < _node->key)
        return find_element(_node->left, key);
    else
        return find_element(_node->right, key);
}

tree_iterator tree::find(int value)
{
    node* element = find_element(root, value);
    if (element == nullptr)
        return end();
    else
        return tree_iterator(element);
}

pair<tree_iterator, bool> tree::insert(int k, tree_iterator where = tree_iterator(nullptr))
{

}

tree_iterator insert(const tree_iterator& where, const int& k)
{

}

pair<tree_iterator, bool> tree::erase(int k)
{

}


tree & tree::operator= (const tree & other)
{
    tree temp;
    for (auto x: other)
        temp.insert(x);
    swap(temp);
    return *this;
}


tree & tree::operator |= (const tree & other) {
        tree temp;
        set_union(begin(), end(),
            other.begin(), other.end(),
            outinserter<tree, tree_iterator>(temp,tree_iterator())); 
        swap(temp);
	return *this;
}

tree & tree::operator &= (const tree & other) {
        tree temp;
        set_intersection(begin(), end(),
            other.begin(), other.end(),  
            outinserter<tree, tree_iterator>(temp,tree_iterator())); 
        swap(temp);
	return *this;
}

tree & tree::operator -= (const tree & other) {
        tree temp;
        set_difference(begin(), end(),
            other.begin(), other.end(),  
            outinserter<tree, tree_iterator>(temp,tree_iterator())); 
        swap(temp);
	return *this;
}
tree & tree::operator ^= (const tree & other) {
        tree temp;
        set_symmetric_difference(begin(), end(),
            other.begin(), other.end(),  
            outinserter<tree, tree_iterator>(temp,tree_iterator())); 
        swap(temp);
	return *this;
}

// tree & tree::operator|= (const tree & other)
// {
//     for (auto x: other)
//         insert(x);
//     return *this;
// }

// tree& tree::operator &= (const tree & other){ 
// 	tree temp;
// 	for (auto x : *this) 
//         if(other.find(x) != other.end()) 
//             temp.insert(x);
// 	temp.swap(*this);
// 	return *this;
// }
// tree& tree::operator -= (const tree & other){
// 	tree temp;
// 	for (auto x : *this) 
//         if(other.find(x) == other.end()) 
//             temp.insert(x);
// 	temp.swap(*this);
// 	return *this;
// }
// tree& tree::operator ^= (const tree & other){
// 	tree temp;
// 	for (auto x : *this) 
//         if(other.find(x) != other.end( ))
//             temp.insert(x);
// 	for (auto x : other) 
//         if(find(x) != other.end( )) 
//             temp.insert(x);
// 	temp.swap(*this);
// 	return *this;
// }

void tree::display()
{
    screen_clear();

    if (root)
    {
        root->display(0, FIRSTCOL - SHIFT, 0);
        screen_refresh();
    }
	else
        cout << "Empty!\n"
}

#endif
