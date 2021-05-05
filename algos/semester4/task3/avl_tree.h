#ifndef AVL_TREE_H
#define AVL_TREE_H

using namespace std;

const int B[] = {-1,1};

// УЗЕЛ ДЕРЕВА
struct node
{
    int key; // ключ узла
    signed char balance_factor; // баланс (разность поддеревьев узла)
    int duplicates; // количество дубликатов ключа
    node* nodes[2]; // левое и правое поддерево

    node(int k): key(k), balance_factor(0), duplicates(0) {nodes[0] = nodes[1] = nullptr;}
    node(const node&) = delete;
    ~node(){delete nodes[1]; delete nodes[0];}
    void display(char**, int, int);
};

void node::display(char** s, int r, int c) //Вывод узла в точку (r,c)
{
    string b = "-o+";
    if (r && c && (c<80))
    {
        setval(s[r-1], c-1, key);
        s[r][c-1] = b[1 + balance_factor];
        setval(s[r+1], c-1, duplicates + 1);
    }
    if (nodes[0])
        nodes[0]->display(s, r+1, c-(OFFSET >> r)+1);
    if (nodes[1])
        nodes[1]->display(s, r+1, c+(OFFSET >> r)-1);
}

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
        for(;ptr->nodes[0];ptr = ptr->nodes[0]) st.push(make_pair(ptr, 0));
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

// АВЛ ДЕРЕВО
class tree
{
    static screen s; // для вывода деревьев
    node *root; // корень дерева
    int h, count; // высота и мощность дерева

    node* find_element(node*, int) const;
    public:
        // конструкторы/деструкторы:
        tree(): root(nullptr), h(0), count(0) {}
        tree(const tree &other): tree() {for(auto x = other.begin(); x != other.end(); ++x) insert(*x);}
        tree(tree && other): tree() {swap(other);}
        ~tree(){ delete root; }
        void swap(tree &);

        // доступ:
        tree_iterator begin() const;
        tree_iterator end() const { return tree_iterator(nullptr); }
        bool empty() const { return (root == nullptr); }
        int size() const { return count; }
        int height() const {return h;}

        // вставка/удаление
        pair<tree_iterator, bool> insert(int, tree_iterator = tree_iterator(nullptr));
        tree_iterator insert(const tree_iterator& where, const int& k) { return insert(k, where).first; }
        bool erase(int);

        // другие методы
        void display();
        tree_iterator find(int) const;
        tree & operator= (const tree &);
        tree & operator= (tree && other) {swap(other); return *this;}
};

void tree::swap(tree & other)
{
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

tree_iterator tree::find(int value) const
{
    node* element = find_element(root, value);
    if (element == nullptr)
        return end();
    else
        return tree_iterator(element);
}

pair<tree_iterator, bool> tree::insert(int k, tree_iterator where)
{
    node *q{root}, *p;
    Stack St;

    if (!where.ptr) // свободная вставка
    {
        if (empty())
        {
            root = new node(k);
            count = h = 1;
            St.push(make_pair(root, 1));
            return make_pair(tree_iterator(root, move(St)), true);
        }
    }
    else // с места предыдущей вствки
    {
        q = where.ptr;
        St = move(where.st);
    }

    while (q) // поиск места вставки...
    {
        if (k == q->key)
        {
            ++(q->duplicates);
            return (make_pair(tree_iterator(q, move(St)), false));
        }
        p = q;
        int a{k < q->key}; //Шаг поиска вниз по дереву
        if (a)
            q = q->nodes[0];
        else
            q = q->nodes[1];
        St.push(make_pair(p, 1 - a));
    }

    q = new node(k); //Вставлен новый узел в точке q
    if (p)
    {
        if (k < p->key)
            p->nodes[0] = q;
        else
            p->nodes[1] = q;
    }
    else
        root = q;

    node *new_node = q;

    while (!St.empty())
    //Движемся к корню, опрокидывая балансы, ищем не нуль
    {
        auto pa = St.top(); St.pop();
        p = pa.first;
        int a = pa.second;

        if (!(p->balance_factor))
        {
            p->balance_factor = B[a]; //Замена 0 на +-1
            if (p == root) //Дерево подросло
            {
                ++h;
                break;
            }
            else
                q = p;
        }
        else if (p->balance_factor == -B[a])
        { // Сбалансировалось
            p->balance_factor = 0; //Замена +-1 на 0
            break;
        }
        // Перебалансировка
        else if (p->balance_factor == q->balance_factor)
        {	//Случай 1: Однократный поворот
            p->nodes[a] = q->nodes[1 - a];
            q->nodes[1 - a] = p;
            p->balance_factor = q->balance_factor = 0;
            if(p == root)
                p = root = q;
            else
                St.top().first->nodes[St.top().second] = p = q;
            break;
        }
        else
        { //Случай 2: Двукратный поворот
            node *r(q->nodes[1 - a]);
            p->nodes[a] = r->nodes[1 - a];
            q->nodes[1 - a] = r->nodes[a];
            r->nodes[1 - a] = p;
            r->nodes[a] = q;
            if (r->balance_factor == B[a])
            {
                p->balance_factor = -B[a];
                q->balance_factor = 0;
            }
            else if (r->balance_factor == -B[a])
            {
                 p->balance_factor = 0;
                 q->balance_factor = B[a];
            }
            else
            {
                p->balance_factor = q->balance_factor = 0;
            }
            r->balance_factor = 0;
            if (p == root)
                p = root = r;
            else
                St.top().first->nodes[St.top().second] = p = r;
            break;
        }
    }

    ++count;
    return make_pair(tree_iterator(new_node, move(St)), true);
}


bool tree::erase(int k)
{
    node *q{root}, *p = nullptr;
    Stack St;
    int a;

    // найти узел
    while (q != nullptr && q->key != k)
    {
        p = q;
        a = (k < q->key);
        q = q->nodes[1 - a];
        St.push(make_pair(p, 1 - a));
    }

    if (q == nullptr)
        return false;
    else
        St.push(make_pair(q, 1 - a));

    // удалить узел
    if (q->duplicates > 0)
        --(q->duplicates);
    else
    {
        if(q->nodes[0] == nullptr && q->nodes[1] == nullptr)
        {
            if (p == nullptr)
                return false;
            if(q == p->nodes[0])
                p->nodes[0] = nullptr;
            else
                p->nodes[1] = nullptr;
            St.pop();
            q->nodes[0] = q->nodes[1] = nullptr;
            delete q;
        }
        else if(q->nodes[1] == nullptr)
        {
            if (p == nullptr)
                return false;
            if (q == p->nodes[0])
                p->nodes[0] = q->nodes[0];
            else
                p->nodes[1] = q->nodes[0];
            St.pop();
            q->nodes[0] = q->nodes[1] = nullptr;
            delete q;
        }
        else if(q->nodes[0] == nullptr)
        {
            if (p == nullptr)
                return false;
            if (q == p->nodes[0])
                p->nodes[0] = q->nodes[1];
            else
                p->nodes[1] = q->nodes[1];
            St.pop();
            q->nodes[0] = q->nodes[1] = nullptr;
            delete q;
        }
        else
        {
            node *succ = q->nodes[1];

            for(p = q; succ->nodes[0]; p = succ, succ = succ->nodes[0]) St.push(make_pair(succ, 0));
            q->key = succ->key;
            q->duplicates = succ->duplicates;

            if (succ == p->nodes[0])
                p->nodes[0] = succ->nodes[1];
            else
                p->nodes[1] = succ->nodes[1];

            succ->nodes[0] = succ->nodes[1] = nullptr;
            delete succ;
        }

        // восстановить баланс
        do //Цикл, пока не дойдём до корня дерева
        {
            auto pp = St.top(); St.pop();	//Поднятие из стека указателя на отца и направления
            p = pp.first; a = pp.second;

            if (!p)
                --h; // Дошли до корня. Уменьшаем высоту дерева
            else if (p->balance_factor) //b != 0
            {
                if (p->balance_factor == B[a]) // Поддерево сбалансировалось. Идем к корню...
                    p->balance_factor = 0;
                else // p->b == -B[a]: Требуется перебалансировка
                {
                    node *r;
                    r = q = p->nodes[1 - a];
                    if (r->balance_factor == -p->balance_factor)  // Случай 2: двукратный поворот
                    {
                        r = r->nodes[a];
                        p->nodes[1 - a] = r->nodes[a];
                        q->nodes[a] = r->nodes[1 - a];
                        r->nodes[a] = p;
                        r->nodes[1 - a] = q;
                        if (r->balance_factor)
                        {
                            if (r->balance_factor == B[a])
                            {
                                p->balance_factor = 0;
                                q->balance_factor = -B[a];
                            }
                            else
                            {
                                p->balance_factor = B[a];
                                q->balance_factor = 0;
                            }
                            r->balance_factor = 0;
                        }
                        else
                            q->balance_factor = p->balance_factor = 0;
                    }
                    else // Случаи 1 и 3: однократный поворот
                    {
                        p->nodes[1 - a] = r->nodes[a];
                        r->nodes[a] = p;
                        if (r->balance_factor) // Случай 1: поддерево стало ниже...
                        {
                            r->balance_factor = p->balance_factor = 0;
                        }
                        else // Случай 3: высота поддерева не изменилась!
                        {
                            r->balance_factor = B[a];
                            break;
                        }
                    }

                    if (St.top().first)
                        St.top().first->nodes[St.top().second] = r;
                    else
                        root = r;
                }
            }
            else
                p->balance_factor = -B[a];
        } while (!St.empty());

        --count;
    }
    return true;
}

tree & tree::operator= (const tree & other)
{
    tree temp;
    for (auto x: other)
        temp.insert(x);
    swap(temp);
    return *this;
}

void tree::display()
{
    s.screen_clear();
    if (root)
    {
        cout << "AVL TREE " << " (HEIGHT: " << height() <<"; SIZE: " << size() << "): ";
        root->display(s.SCREEN, 1, OFFSET);
        s.screen_refresh();
    }
	else
        cout << "Empty!\n";
}

#endif
