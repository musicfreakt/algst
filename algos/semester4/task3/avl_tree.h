#ifndef AVL_TREE_H
#define AVL_TREE_H

using namespace std;

const int B[] = {0,-1,1};

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
    node(int k): key(k), balance_factor(0) {nodes[0] = nodes[1] = nullptr;}
    node(const node& ) = delete;
    ~node(){delete nodes[1]; delete nodes[0];}
};


// void node::display(int row, int col, int depth)
// {
//     string sb = "-0+";
//     if ((row > MAXROW) || (col < 0) || (col > MAXCOL)) return;
//     if (row > MAXOUT)
//     {
//         screen[row].replace(col, 3, "+++");
//         return;
//     }
//     setval(screen[row], col, key);
//     screen[row+1][col] = sb[balance_factor + 1];
//     if (nodes[0])
//         nodes[0]->display(row + 1, col - OFFSET[depth], depth+1);
//     if (nodes[1])
//         nodes[1]->display(row + 1, col + OFFSET[depth], depth+1);
//
// }


using Stack = stack<pair<node*, int>>;

//ИТЕРАТОР ЧТЕНИЯ (нужны сравнения, разыменования, инкремент)
struct tree_iterator: public std::iterator<std::forward_iterator_tag, int>
{
    node* ptr; //Реальный указатель на элемент контейнера
    Stack st; // стек с путем от корня дерева

    tree_iterator(node *p = nullptr) : ptr(p) {}
    tree_iterator(node *p, Stack s) : ptr(p), st(s) {}
    bool operator== (const tree_iterator & other) const { return ptr == other.ptr; }
    bool operator!= (const tree_iterator & other) const { return !(*this == other); }
    tree_iterator& operator++(); // Инкремент по контейнеру
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
class outiterator: public iterator<output_iterator_tag, typename Container::value_type>
{
    protected:
        Container& container; // Контейнер для вставки элементов
        Iter iter; // текущее значение итератора чтения
    public:
        outiterator(Container &c, Iter it) : container(c), iter(it) {}
        const outiterator<Container>& operator= (const typename Container::value_type& value)
        {
            iter = container.insert(iter, value);
            return *this;
        }

        // Присваивание копии фиктивное
        const outiterator<Container>& operator= (const outiterator<Container> &) { return *this; }
        outiterator<Container>& operator* () {return *this;} // Разыменование - пустая операция
        outiterator<Container>& operator++ () {return *this;} // Инкремент - пустая операция
        outiterator<Container>& operator++ (int) {return *this;} // Инкремент - пустая операция
};

// функция для создания итератора вставки
template<typename Container, typename Iter>
inline outiterator<Container, Iter> outinserter(Container& c, Iter It)
{
    return outiterator<Container, Iter>(c, It);
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
        pair<tree_iterator, bool> insert(int, tree_iterator = tree_iterator(nullptr));
        tree_iterator insert(const tree_iterator& where, const int& k) { return insert(k, where).first; }
        // pair<tree_iterator, bool> erase(int);

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
};

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
    node *p(root);
    if (p)
        for(;p->nodes[0];p = p->nodes[0]) st.push(make_pair(p, 0));

    // while (p->nodes[0])
    // {
    //     st.push(make_pair(p, 0));
    //     p = p->nodes[0];
    // }

    return tree_iterator(p, move(st));
}

node* tree::find_element(node* _node, int key) const
{
    if (_node == nullptr ||
        (!(key < _node->key) && !(_node->key < key)))
        return _node;
    else
        return find_element(_node->nodes[!(key < _node->key)], key);
}

tree_iterator tree::find(int value)
{
    node* element = find_element(root, value);
    if (element == nullptr)
        return end();
    else
        return tree_iterator(element);
}

pair<tree_iterator, bool> tree::insert(int k, tree_iterator where)
{
    node *q{root};
    Stack St;

    if (!where.ptr) // свободная вставка
    {
        if (q == nullptr)
        {
            root = new node(k);
            count = h = 1;
            St.push(make_pair(root, 1));
            return make_pair(tree_iterator(root, move(St)), true);
        }
        else
            St.push(make_pair(root, 1));
    }
    else // с места предыдущей вствки
    {
        q = where.ptr;
        St = move(where.st);
    }

    while (q) // поиск места вставки...
    {
		if (k == q->key)
            return (make_pair(tree_iterator(q, move(St)), true));
		int a{k < q->key}; //Шаг поиска вниз по дереву
        q = q->nodes[1 - a];
        St.push(make_pair(q, a));
	}

    q = new node(k);

    //Вставлен новый узел в точке q
	while (!St.empty())
    //Движемся к корню, опрокидывая балансы, ищем не нуль
	{
		auto pa = St.top(); St.pop();
		node *p = pa.first;
        int a = pa.second;
		if (!p->balance_factor)
        {
			p->balance_factor = B[a]; //Замена 0 на +-1
			if (p == root) //Дерево подросло
			{
                ++h;
                break;
            }
			else q = p, p = St.top().first;  //Шаг вверх, цикл продолжается
		}
		else if (p->balance_factor == -B[a])
        { //Сбалансировалось
			p->balance_factor = 0; //Замена +-1 на 0
			break;
		}
		//===== Перебалансировка =====
		else if (p->balance_factor == q->balance_factor)
        {	//Случай 1: Однократный поворот
			p->nodes[a] = q->nodes[1 - a];
			q->nodes[1 - a] = p;
			p->balance_factor = q->balance_factor = 0;
			if(p == root) p = root = q;
			else St.top().first->nodes[St.top().second] = p = q;
		    break;
		}
		else
        {	//Случай 2: Двукратный поворот
			node *r(q->nodes[1-a]);
			p->nodes[a] = r->nodes[1 - a];
			q->nodes[1-a] = r->nodes[a];
			r->nodes[1 - a] = p;
		    r->nodes[a] = q;
			if (r->balance_factor == B[a]) { p->balance_factor = -B[a]; q->balance_factor = 0; }
			else if (r->balance_factor == -B[a]) { p->balance_factor = 0; q->balance_factor = B[a]; }
			else { p->balance_factor = q->balance_factor = 0; }
			r->balance_factor = 0;
			if (p == root)
                p = root = r;
			else
                St.top().first->nodes[St.top().second] = p = r;
		    break;
		}
	}

    ++count;
    return make_pair(tree_iterator(q, move(St)), true);
}

pair<tree_iterator, bool> tree::erase(int k) // зачем pair?
{

    //Проверяем, существует ли данный узел
    node* remove_node = find_element(k); // todo: заменить на итераторы??!!!
    if(remove_node == nullptr)
        return false;

    node * current_node;
    //Если удаляем корень
    if(remove_node == root){
        if()
            current_node = remove_node.nodes[];
        else curTree = tree.left;
        while (curTree.left != null) {
            curTree = curTree.left;
        }
        T temp = curTree.val;
        this.remove(temp);
        tree.val = temp;
        return true;
    }
    //Удаление листьев
    if(tree.left==null && tree.right==null && tree.parent != null){
        if(tree == tree.parent.left)
            tree.parent.left = null;
        else {
            tree.parent.right = null;
        }
        return true;
    }
    //Удаление узла, имеющего левое поддерево, но не имеющее правого поддерева
    if(tree.left != null && tree.right == null){
        //Меняем родителя
        tree.left.parent = tree.parent;
        if(tree == tree.parent.left){
            tree.parent.left = tree.left;
        }
        else if(tree == tree.parent.right){
            tree.parent.right = tree.left;
        }
        return true;
    }
    //Удаление узла, имеющего правое поддерево, но не имеющее левого поддерева
    if(tree.left == null && tree.right != null){
        //Меняем родителя
        tree.right.parent = tree.parent;
        if(tree == tree.parent.left){
            tree.parent.left = tree.right;
        }
        else if(tree == tree.parent.right){
            tree.parent.right = tree.right;
        }
        return true;
    }
    //Удаляем узел, имеющий поддеревья с обеих сторон
    if(tree.right!=null && tree.left!=null) {
        curTree = tree.right;
        while (curTree.left != null) {
            curTree = curTree.left;
        }
        //Если самый левый элемент является первым потомком
        if(curTree.parent == tree) {
            curTree.left = tree.left;
            tree.left.parent = curTree;
            curTree.parent = tree.parent;
            if (tree == tree.parent.left) {
                tree.parent.left = curTree;
            } else if (tree == tree.parent.right) {
                tree.parent.right = curTree;
            }
            return true;
        }
        //Если самый левый элемент НЕ является первым потомком
        else {
            if (curTree.right != null) {
                curTree.right.parent = curTree.parent;
            }
            curTree.parent.left = curTree.right;
            curTree.right = tree.right;
            curTree.left = tree.left;
            tree.left.parent = curTree;
            tree.right.parent = curTree;
            curTree.parent = tree.parent;
            if (tree == tree.parent.left) {
                tree.parent.left = curTree;
            } else if (tree == tree.parent.right) {
                tree.parent.right = curTree;
            }
            return true;
        }
    }
    return false;



    // удалить узел k

    while (cont) //Цикл, пока не дойдём до корня дерева
    {
        cont = false;
        if (!p)
            --h; // Дошли до корня. Уменьшаем высоту дерева
        else if (p->b) //b != 0
        {
            cont = true;
            if (p->b == B[a]) // Поддерево сбалансировалось. Идем к корню...
            {
                p->b = 0;
                auto pp = St.top(); St.pop();	//Поднятие из стека указателя на отца и направления
                p = pp.first; a = pp.second;
            }
            else // p->b == -B[a]: Требуется перебалансировка
            {
                q = r = p->L[1 - a];
                if (r->b == -p->b)  // Случай 2: двукратный поворот
                {
                    r = r->L[a];
                    p->L[1 - a] = r->L[a];
                    q->L[a] = r->L[1 - a];
                    r->L[a] = p;
                    r->L[1 - a] = q;
                    if (r->b)
                    {
                        if (r->b == B[a]) { p->b = 0; q->b = -B[a]; }
                        else { p->b = B[a]; q->b = 0; }
                        r->b = 0;
                    }
                    else  q->b = p->b = 0;
                }
                else // Случаи 1 и 3: однократный поворот
                {
                    p->L[1 - a] = r->L[a];
                    r->L[a] = p;
                    if (r->b) // Случай 1: поддерево стало ниже...
                    {
                        r->b = p->b = 0;
                    }
                    else // Случай 3: высота поддерева не изменилась!
                    {
                        r->b = B[a];
                        cont = false;     //...Балансировка закончена. Выход
                    }
                }
                auto pp = St.top(); St.pop();     //Шаг вверх по дереву
                p = pp.first; a = pp.second;
                if (p) p->link[a] = r;     // Завершение поворотов
                else root = r;
            }
        }
        else
        {
            p->b = -B[a]; // (p->b == 0): Алгоритм завершён.
        }
    } //while(cont)...
}

tree & tree::operator= (const tree & other)
{
    tree temp;
    for (auto x: other)
        temp.insert(x);
    swap(temp);
    return *this;
}

tree & tree::operator|= (const tree & other)
{
        tree temp;
        set_union(begin(), end(),
            other.begin(), other.end(),
            outinserter<tree, tree_iterator>(temp,tree_iterator()));
        swap(temp);
	return *this;
}

tree & tree::operator&= (const tree & other)
{
        tree temp;
        set_intersection(begin(), end(),
            other.begin(), other.end(),
            outinserter<tree, tree_iterator>(temp,tree_iterator()));
        swap(temp);
	return *this;
}

tree & tree::operator-= (const tree & other)
{
        tree temp;
        set_difference(begin(), end(),
            other.begin(), other.end(),
            outinserter<tree, tree_iterator>(temp,tree_iterator()));
        swap(temp);
	return *this;
}

tree & tree::operator^= (const tree & other)
{
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

// void tree::display()
// {
//     screen_clear();
//
//     if (root)
//     {
//         root->display(0, FIRSTCOL - SHIFT, 0);
//         screen_refresh();
//     }
// 	else
//         cout << "Empty!\n";
// }

#endif
