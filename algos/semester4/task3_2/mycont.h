#ifndef MYCONT_H
#define MYCONT_H

using namespace std;

struct node 
{
	int key;
	int N = 1;
	bool next = false;
	node *left, *right;

	node() : left(nullptr), right(nullptr) {}
	node(int k, node * l = nullptr, node * r = nullptr) : key(k), left(l), right(r) {}
	~node() { delete right; delete left; };
	void out(int, int);
	void display(int, int);
};

void node::out(int row, int col)
{
	if ((row > MAXROW) || (col < 0) || (col > MAXCOL)) return;
	try {
		if (row > MAXOUT)
			SCREEN[row].replace(col, 3, "+++");
		else 
		{
			SCREEN[row].replace(col, 3, to_string(key));
			if (next) 
			{
				if (right)SCREEN[row + 1].replace(col + 1, 3, to_string(right->key));
				else SCREEN[row + 1].replace(col + 1, 3, "***");
			}
		}
	}
	catch (out_of_range) { cout << "Out: row=" << row << " col=" << col; }
}

using MyStack = stack<pair<node*, int>>;

struct Iterator : public std::iterator<std::forward_iterator_tag, int>
{
	Iterator(node *p) : Ptr(p) {}
	Iterator(node *p, MyStack St) : Ptr(p), St(St) {}
	bool operator == (const Iterator & Other) const { return Ptr == Other.Ptr; }
	bool operator != (const Iterator & Other) const { return !(*this == Other); }
	Iterator & operator++();
	Iterator operator++(int) { Iterator temp(*this); ++*this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
	node * Ptr;
	MyStack St;
};

// ИТЕРАТОР ВСТАВКИ (нужны присваивание, разыменование , инкремент)
template<typename Container, typename Iter = Iterator>
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

class mycont 
{
	static int tags;
	char tag;
	node * root;
	int count;

	vector <int> keys;
	public:
	 	using key_type = int;
        using value_type = int;
        using key_compare = less<int>;
		static int height;
		
		mycont() : root(nullptr), count(0), tag('A' + tags++) {}
		mycont(const mycont &other): mycont() {for(auto x : other) insert(x);}
        mycont(mycont && other): mycont() {swap(other);}
		template <typename iter>
        mycont(iter, iter);
		~mycont() { delete root; };

		Iterator begin() const;
		Iterator end() const { return Iterator(nullptr); }
		int size() const { return count; }
		bool empty() const { return (root == nullptr); }
		size_t power() const { return keys.size(); } 
		void swap(mycont &);

		pair<Iterator, bool> insert(int, Iterator = Iterator(nullptr));
		Iterator insert(const Iterator where, const int& k) { return insert(k, where).first; }
		bool erase(int);
		void clear() { count = 0; delete root; root = nullptr; }

		void display(bool);	
		mycont & operator= (const mycont &);
        mycont & operator= (mycont &&);

        void merge(const mycont&);
        void concat(const mycont&);
        void erase(int, int);

        mycont& operator &= (const mycont &);
        mycont operator & (const mycont & other) const
        { mycont result(*this); return (result &= other); }
        mycont& operator |= (const mycont &);
        mycont operator | (const mycont & other) const
        { mycont result(*this); return (result |= other); }
        mycont& operator -= (const mycont &);
        mycont operator - (const mycont & other) const
        {mycont result(*this); return (result -= other);}
        mycont& operator ^= (const mycont &);
        mycont operator ^ (const mycont & other) const
        {mycont result(*this); return (result ^= other);}
};

template <typename iter>
mycont::mycont(iter b, iter e): mycont()
{
    for (auto i = b; i != e; ++i)
        insert(*i);
}

Iterator mycont::begin() const
{
	stack<pair<node*, int>>St;
	node* p(root);
	if (p) {
		while (p->left) {
			St.push(make_pair(p, 0));
			p = p->left;
		}
	}
	return Iterator(p, St);
}

Iterator& Iterator::operator++()
{
	if (!Ptr) 
	{
		return *this;
	}
	if (Ptr->right) {
		St.push(make_pair(Ptr, 1));
		Ptr = Ptr->right;
		while (Ptr->left)
		{
			St.push(make_pair(Ptr, 0));
			Ptr = Ptr->left;
		}
	}
	else {
		pair<node*, int> pp(Ptr, 1);
		while (!St.empty() && pp.second) { pp = St.top(); St.pop(); }
		if (pp.second)
			Ptr = nullptr;
		else Ptr = pp.first;
	}
	return (*this);
}

void mycont::swap(mycont & other)
{
    std::swap(root, other.root);
    std::swap(height, other.height);
    std::swap(count, other.count);
	std::swap(keys, other.keys);
}

pair<Iterator, bool> mycont::insert(int k, Iterator where)
{
	node *t{ root }, *new_node{nullptr};
	bool cont{ true }, up{ false };
	stack<pair<node*, int>> St;

	if (!where.Ptr) 
	{
		if (t == nullptr) 
		{
			root = new node(k);
			keys.push_back(k);
			count = height = 1;
			St.push(make_pair(root, 1));
			return make_pair(Iterator(root, move(St)), true);
		}
		else St.push(make_pair(root, 1));
	}
	else 
	{
		t = where.Ptr;
		St = move(where.St);
	}

	while (cont) 
	{
		if (k == t->key)
		{
			keys.push_back(k);
			return make_pair(Iterator(t, move(St)), false);
		}
		if (k < t->key) 
		{
			if (t->left) 
			{
				St.push(make_pair(t, 2));
				t = t->left;
			}
			else 
			{
				t->left = new node(k, nullptr, t->left);
				keys.push_back(k);
				new_node = t->left;
				cont = false;
			}
		}
		else if (!t->right) 
		{
			t->right = new node(k);
			keys.push_back(k);
			t->next = true;
			new_node = t->right;
			cont = false;
		}
		else if (t->next) 
		{
			if (k < t->right->key) 
			{
				if (t->right->left) 
				{
					St.push(make_pair(t, 3));
					t = t->right->left;
				}
				else 
				{
					t->right->left = new node(k, nullptr, t->right->left);
					keys.push_back(k);
					new_node = t->right->left;
					cont = false;
				}
			}
			else 
			{
				if (t->right->right) 
				{
					St.push(make_pair(t, 4));
					t = t->right->right;
				}
				else 
				{
					t->right->right = new node(k);
					keys.push_back(k);
					up = t->right->next = true;
					new_node = t->right->right;
					cont = false;
				}
			}
		}
		else if (t->right) 
		{
			St.push(make_pair(t, 3));
			t = t->right;
		}
		else 
		{
			t->right = new node(k);
			keys.push_back(k);
			t->next = true;
			new_node = t->right;
			cont = false;
		}

		while (up) 
		{
			std::swap(t->key, t->right->key);
			node * t1{ t->right };
			t->next = t1->next = false;
			t->right = t->right->right;
			t1->right = t1->left;
			t1->left = t->left;
			t->left = t1;
			t1 = t;
			t = St.top().first;

			switch (St.top().second) 
			{
				case 1:
					++height;
					up = false;
					break;
				case 2:
					std::swap(t->key, t1->key);
					t->left = t1->left;
					t1->left = t1->right;
					t1->right = t->right;
					t->right = t1;
					up = t1->next = t->next;
					break;
				case 3:
					if (t->next) 
					{
						t->right->left = t1->right;
						t1->right = t->right;
						t->right = t1;
						t1->next = true;
					}
					else 
					{
						t->next = true;
						up = t1->next = false;
						St.pop();
					}
					break;
				case 4:
					t->right->next = true;
					t1->next = false;
			}
			if (up) 
				St.pop();
		}
	}

	++count;
	return make_pair(Iterator(new_node, move(St)), true);
};


mycont & mycont::operator= (const mycont & other)
{
    mycont temp;
	std::copy(other.begin(), other.end(), outinserter(temp, Iterator(nullptr)));
    swap(temp);
    return *this;
}

mycont& mycont::operator=(mycont&& other)
{
	swap(other);
	for (auto x : other.keys)
		keys.push_back(x);
	return *this;
}

void mycont::display(bool flag = false)
{
	if (flag)
	{
		clrscr();
		cout << endl;
		height = 0;
		if (root) root->display(0, FIRSTCOL - SHIFT);
		SCREEN[0] = "1-2-mycont (n=" + to_string(count) + " h=" + to_string(height) + 
			+ ") -------------------------------------> ";
		SCREEN[0] += (root) ? to_string(root->key) : " <Empty>";
		SCREEN[0].resize(MAXCOL, '.');
		showscr();
	}

	cout << "МНОЖЕСТВО( " << tag << " ) : ";
	for (auto x = begin(); x != end(); ++x)
		cout << *x << " ";

	cout << "\nПОСЛЕДОВАТЕЛЬНОСТЬ ( " << tag << " ): < "; for (auto x : keys) cout << x << " "; cout << ">\n\n"; 
}

void node::display(int level, int col)
{
	int row{ level * 2 };
	this->out(row, col);
	if (left) left->display(level + 1, col - (OFFSET[level + 1]));
	if (next) 
	{
		if (right->left) right->left->display(level + 1, col);
		if (right->right) right->right->display(level + 1, col + (OFFSET[level + 1]));
	}
	else
		if (right) right->display(level + 1, col + (OFFSET[level + 1]));
	if (level > mycont::height) mycont::height = level;
}

mycont& mycont::operator&= (const mycont & other)
{
    mycont temp;
    set_intersection(begin(), end(),
        other.begin(), other.end(),
        outinserter(temp, Iterator(nullptr)));
    swap(temp);
	return *this;
}

mycont& mycont::operator|= (const mycont & other)
{
    mycont temp;
    set_union(begin(), end(),
        other.begin(), other.end(),
        outinserter(temp, Iterator(nullptr)));
    swap(temp);
	return *this;
}

mycont& mycont::operator-= (const mycont & other)
{
    mycont temp;
    set_difference(begin(), end(),
        other.begin(), other.end(),
        outinserter(temp, Iterator(nullptr)));
    swap(temp);
	return *this;
}

mycont& mycont::operator^= (const mycont & other)
{
    mycont temp;
    set_symmetric_difference(begin(), end(),
        other.begin(), other.end(),
        outinserter(temp, Iterator(nullptr)));
    swap(temp);
	return *this;
}

void mycont::erase(int left, int right)
{
    int p = power();
    left = std::min(left, p);
    right = std::min(right+1, p);
    if(left <= right)
    {
        mycont temp;
        for(int i = 0; i < left; ++i)
			temp.insert(keys[i]);
        for(int i = right; i < p; ++i)
            temp.insert(keys[i]);
        swap(temp);
    }
}

void mycont::concat(const mycont & other)
{
    for(auto x : other.keys)
        insert(x);
}

void mycont::merge(const mycont & other) 
{ 
	using std::sort;
	vector <int> temp(other.keys), res;
	std::merge(keys.begin(), keys.end(), temp.begin(), temp.end(), 
        std::back_inserter(res));
    for (auto x : other.keys)
	    insert(x);
	keys.swap(res);
}

#endif