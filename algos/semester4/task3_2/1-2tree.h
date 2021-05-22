#ifndef TREE_H
#define TREE_H

using namespace std;

struct Node 
{
	int key;
	int N = 1;
	bool next = false;
	Node *left, *right;
	Node() : left(nullptr), right(nullptr) {}
	Node(int k, Node * l = nullptr, Node * r = nullptr) : key(k), left(l), right(r) {}
	~Node() { delete right; delete left; };
	void display(int, int);
	void out(int, int);
};

using MyStack = stack<pair<Node*, int>>;

struct tree_iter : public std::iterator<std::forward_iterator_tag, int>
{
	tree_iter(Node *p) : Ptr(p) {}
	tree_iter(Node *p, MyStack St) : Ptr(p), St(St) {}
	bool operator == (const tree_iter & Other) const { return Ptr == Other.Ptr; }
	bool operator != (const tree_iter & Other) const { return !(*this == Other); }
	tree_iter & operator++();
	tree_iter operator++(int) { tree_iter temp(*this); ++*this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
	Node * Ptr;
	MyStack St;
};

class Tree 
{
	Node * root;
	int count;
	int height;
	public:
		// конструкторы/деструкторы:
		Tree() : root(nullptr), count(0) {}
		~Tree() { delete root; };

		// доступ:
		tree_iter begin() const;
		tree_iter end() const { return tree_iter(nullptr); }
		int size() const { return count; }
		bool empty() const { return (root == nullptr); }
		void swap(tree &);

		// вставка/удаление
		pair<tree_iter, bool> insert(int, tree_iter = tree_iter(nullptr));
		tree_iter insert(const int& k, tree_iter where) { return insert(k, where).first; }
		bool erase(int);
		void clear() { count = 0; delete root; root = nullptr; }

		// другие методы
		void display();	
		Tree & operator= (const Tree &);
        Tree & operator= (Tree && other) {swap(other); return *this;}
};

tree_iter Tree::begin() 
{
	stack<pair<Node*, int>>St;
	Node* p(root);
	if (p) {
		while (p->left) {
			St.push(make_pair(p, 0));
			p = p->left;
		}
	}
	return tree_iter(p, St);
}

tree_iter& tree_iter::operator++()
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
		pair<Node*, int> pp(Ptr, 1);
		while (!St.empty() && pp.second) { pp = St.top(); St.pop(); }
		if (pp.second)
			Ptr = nullptr;
		else Ptr = pp.first;
	}
	return (*this);
}

void tree::swap(tree & other)
{
    std::swap(root, other.root);
    std::swap(height, other.height);
    std::swap(count, other.count);
}

pair<tree_iter, bool> Tree::insert(int k, tree_iter where)
{
	Node *t{ root };
	bool cont{ true }, up{ false };
	stack<pair<Node*, int>> St;
	if (!where.Ptr) {

		if (t == nullptr) {
			root = new Node(k);
			count = height = 1;
			St.push(make_pair(root, 1));
			return make_pair(tree_iter(root, move(St)), true);
		}
		else St.push(make_pair(root, 1));

	}
	else {
		t = where.Ptr;
		St = move(where.St);
	}
	while (cont) {
		if (k == t->key)
			return make_pair(tree_iter(t, move(St)), false);
		if (k < t->key) {
			if (t->left) {
				St.push(make_pair(t, 2));
				t = t->left;
			}
			else {
				t->left = new Node(k, nullptr, t->left);
				cont = false;
			}
		}
		else if (!t->right) {
			t->right = new Node(k);
						t->next = true;
			cont = false;
		}
		else if (t->next) {
			if (k < t->right->key) {
				if (t->right->left) {
					St.push(make_pair(t, 3));
					t = t->right->left;
				}
				else {
					t->right->left = new Node(k, nullptr, t->right->left);
					cont = false;
				}
			}
			else {
				if (t->right->right) {
					St.push(make_pair(t, 4));
					t = t->right->right;
				}
				else {
					t->right->right = new Node(k);
					up = t->right->next = true;
					cont = false;
				}
			}
		}
		else if (t->right) {
			St.push(make_pair(t, 3));
			t = t->right;
		}
		else {
			t->right = new Node(k);
			t->next = true;
			cont = false;
		}
		while (up) {
			swap(t->key, t->right->key);
			Node * t1{ t->right };
			t->next = t1->next = false;
			t->right = t->right->right;
			t1->right = t1->left;
			t1->left = t->left;
			t->left = t1;
			t1 = t;
			t = St.top().first;
			switch (St.top().second) {
			case 1:
				++height;
				up = false;
				break;
			case 2:
				swap(t->key, t1->key);
				t->left = t1->left;
				t1->left = t1->right;
				t1->right = t->right;
				t->right = t1;
				up = t1->next = t->next;
				break;
			case 3:
				if (t->next) {
					t->right->left = t1->right;
					t1->right = t->right;
					t->right = t1;
					t1->next = true;
				}
				else {
					t->next = true;
					up = t1->next = false;
					St.pop();
				}
				break;
			case 4:
				t->right->next = true;
				t1->next = false;
			}
			if (up) St.pop();
		}
	}
	++count;
	return make_pair(tree_iter(t, move(St)), true);
};

bool Tree::erase(int k)
{

	Node *p{ nullptr }, *q{ root };
	int a{ 0 };
	bool cont{ q != nullptr };

	while (cont && (k != q->key)) {
		p = q;
		if (a = k > q->key)
			q = q->right;
		else q = q->left;
		cont = (q != nullptr);
	}
	if (cont) {
		Node *r{ root };

		if (r = q->right) {
			if (r->left) {
				p = q;
				a = 1;
				do {
					p = r;
					r = r->left;
					a = 0;
				} while (r->left);
				q->key = r->key;
				p->left = r->right;
				q = r;
			}
			else {
				(a ? r->right : r->left) = q->left;
				if (p) (a ? p->right : p->left) = r;
				else root = r;
			}
		}
		else {
			if (p) {
				if (a) {
					p->right = q->left; p->next = false;
				}
				else  p->left = q->left;
			}
			else root = q->left;
		}
		q->left = q->right = nullptr;
		--count;
		delete q;
		return true;
	}
	else return false;
}

Tree & Tree::operator= (const Tree & other)
{
    Tree temp;
    for (auto x: other)
        temp.insert(x);
    swap(temp);
    return *this;
}

void Node::out(int row, int col)
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

void Tree::display()
{
	clrscr();
	height = 0;
	if (root) root->display(0, FIRSTCOL - SHIFT);
	SCREEN[0] = "1-2-Tree (n=" + to_string(count) + " h=" + to_string(height) + 
		 + ") -------------------------------------> ";
	SCREEN[0] += (root) ? to_string(root->key) : " <Empty>";
	SCREEN[0].resize(MAXCOL, '.');
	showscr();
}

void Node::display(int level, int col)
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
	if (level > Tree::height) Tree::height = level;
}

#endif