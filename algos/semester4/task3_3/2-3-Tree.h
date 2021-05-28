#ifndef TREE_H
#define TREE_H

using namespace std;

struct Node 
{
	int key;
	Node * next, *down;
	Node() : down(nullptr), next(nullptr) {}
	Node(int k) : key(k), down(nullptr), next(nullptr) {}
	void Clear();
	void Display(int, int);
	void Out(int, int);
	void swap(Node &);
};

void Node::Out(int row, int col)
{
	if ((row > MAXROW) || (col < 0) || (col > MAXCOL)) return;
	if (row > MAXOUT) 
	{
		SCREEN[row].replace(col, 3, "+++");
		return;
	}
	setval(SCREEN[row], col, key);
	if (next) 
	{
		setval(SCREEN[row + 1], col + 1, (next->key));
		if (next->next)
			setval(SCREEN[row + 2], col + 2, (next->next->key));
		else
			SCREEN[row + 2].replace(col + 3, 1, "@");
	}
}

void Node::Display(int level, int col)
{
	int row = FIRSTROW + level * 4;
	this->Out(row, col);
	if (down) {
		down->Display(level + 1, col - (OFFSET[level + 1]));
		next->down->Display(level + 1, col);
		if ((next->next) && (next->next->down))
			next->next->down->Display(level + 1, col + (OFFSET[level + 1]));
	}
}

void Node::Clear()
{
	if (down) 
	{
		if (next->next) next->next->down->Clear();
		next->down->Clear();
		down->Clear();
	}
	else
	{
		if (next) 
		{
			if (next->next) delete next->next;
			delete next;
		}
		delete this;
	}
}

struct myiter : public std::iterator<std::forward_iterator_tag, int>
{
	myiter(Node *p) : Ptr(p) {}
	myiter(Node *p, stack<pair<Node*, int> > St) : Ptr(p), St(St) {}
	bool operator == (const myiter & Other) const { return Ptr == Other.Ptr; }
	bool operator != (const myiter & Other) const { return !(*this == Other); }
	myiter & operator++();
	myiter operator++(int) { myiter temp(*this); ++*this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
	Node * Ptr;
	stack<pair<Node*, int>> St;
};

class Tree 
{
	Node *root;
	int h, count;

	public:
		Tree() : root(nullptr), h(0), count(0) {}
		Tree(const Tree &other): Tree() {for(auto x = other.begin(); x != other.end(); ++x) insert(*x);}
        Tree(Tree && other): Tree() {swap(other);}
		~Tree();
		void clear() { root->Clear(); root = nullptr; h = count = 0; }
		void swap(Tree &);

		myiter begin() const;
		myiter end() const { return myiter(nullptr); }       
		bool empty() const { return (root == nullptr); }
        int size() const { return count; }
        int height() const {return h;}

		pair<myiter, bool> insert(int, myiter = myiter(nullptr));
		myiter insert(myiter where, const int& k) { return insert(k, where).first; }
		bool erase(int);

		void Display();
};

void Tree::swap(Tree & other)
{
    std::swap(root, other.root);
    std::swap(h, other.h);
    std::swap(count, other.count);
}

myiter Tree::begin() const 
{
	stack<pair<Node*, int>>St;
	Node* p = root;
	if (p) {
		St.push(make_pair(p, 1));
		while (p->down) {
			St.push(make_pair(p, 2));
			p = p->down;
		}
	}
	return myiter(p, St);
}
myiter& myiter::operator++()
{
	if (!Ptr)
		return *this;
	else
	{
		if (Ptr->next) 
		{ 
			Ptr = Ptr->next;
			return (*this);
		}
		while (St.size()) 
		{
			Ptr = St.top().first;
			int a = St.top().second;
			St.pop();
			switch (a) 
			{
			case 1:
				Ptr = nullptr;
				return (*this);
			case 2:
				St.push(make_pair(Ptr, 3));
				Ptr = Ptr->next->down;
				while (Ptr->down) 
				{
					St.push(make_pair(Ptr, 2));
					Ptr = Ptr->down;
				}
				return (*this);
			case 3:
				if (Ptr->next->next) 
				{
					St.push(make_pair(Ptr, 4));
					Ptr = Ptr->next->next->down;
					while (Ptr->down) 
					{
						St.push(make_pair(Ptr, 2));
						Ptr = Ptr->down;
					}
					return (*this);
				}
			case 4:;
			}
		}
		Ptr = nullptr;
		return (*this);
	}
}


Tree :: ~Tree()
{
	if (root) root->Clear();
}


void Tree::Display()
{
	clrscr();
	if (root) root->Display(0, FIRSTCOL - SHIFT);
	SCREEN[FIRSTROW] = "2-3-Tree (h=" + to_string(h) + ", n="
		+ to_string(count) + ") ------------------------------------> ";
	SCREEN[FIRSTROW] += (root)? to_string(root->key) : "<Empty!>";
	SCREEN[FIRSTROW].resize(MAXCOL, '.');
	showscr();
}


pair<myiter, bool> Tree::insert(int k, myiter where)
{
	Node *t(root);
	int up = 0;
	stack<pair<Node*, int>> St;
	if (!where.Ptr) 
	{
		if (t == nullptr)
		{	
			root = new Node(k);
			count = h = 1;
			St.push(make_pair(root, 1));
			return make_pair(myiter(root, move(St)), true);
		}
		else St.push(make_pair(root, 1));
	}
	else 
	{
		t = where.Ptr;
		St = move(where.St);
	}
	while (1) 
	{
		if (k == t->key)
			return make_pair(myiter(t, move(St)), false);
		else if ((t->next) && (k == t->next->key))
			return make_pair(myiter(t->next, move(St)), false);
		else if ((t->next) && (t->next->next) && (k == t->next->next->key))
			return make_pair(myiter(t->next->next, move(St)), false);
		
		if (t->down) 
		{
			if (k < t->next->key) 
			{
				if (k < t->key) t->key = k;
				St.push(make_pair(t, ((t->next->next) ? 4 : 2)));
				t = t->down;
			}
			else if (!(t->next->next) || t->next->next && k < t->next->next->key) 
			{ 
				St.push(make_pair(t, (t->next->next ? 5 : 3)));
				t = t->next->down;
			}
			else 
			{
				St.push(make_pair(t, 6));
				t = t->next->next->down;
			}
		}
		else break;
	}


	Node *p = new Node(k); 
	++count;
	
	if (!t->next || t->next && k < t->next->key)
	 {
		p->next = t->next;
		t->next = p;
		if (k < t->key) 
			std::swap(t->key, p->key);
	}
	else if (!t->next->next || k < t->next->next->key) 
	{ 
		p->next = t->next->next;
		t->next->next = p;
	}
	else t->next->next->next = p;
	if (t->next && t->next->next && t->next->next->next) up = 1;

	Node *new_node = p;

	while (up) 
	{
		p = St.top().first;
		int a = St.top().second;
		Node * q = new Node(t->next->next->key);
		switch (a) {
		case 1:
			up = 0;
			h++;
			root = St.top().first = new Node(root->key);
			root->down = p;
			root->next = q;
			St.push(make_pair(root, 3));
			break;
		case 2:
			up = 0;
		case 4:
			q->next = p->next; 
			p->next = q;
			break;
		case 3:	
			up = 0;
		case 5:
			q->next = p->next->next;
			p->next->next = q;
			St.top().second = 6;
			break;
		case 6:
			p->next->next->next = q;
		}
		q->down = t->next->next;
		t->next->next = nullptr;
		t = q->down;
		if (up)
			St.pop(), t = p;
	}
	
	return make_pair(myiter(new_node, move(St)), true);
};

# endif