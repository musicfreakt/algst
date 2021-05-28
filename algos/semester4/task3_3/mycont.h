#ifndef MYCONT_H
#define MYCONT_H

using MySet = Tree;
using Iterator = myiter;
using MySeq = std::vector<Iterator>;

template<typename Container, typename Iter = Iterator>
class outiterator: public iterator<output_iterator_tag, typename Container::value_type>
{
    protected:
        Container& container;
        Iter iter;
    public:
        outiterator(Container &c, Iter it) : container(c), iter(it) {}
        const outiterator<Container>& operator= (const typename Container::value_type& value)
        {
            iter = container.insert(iter, value);
            return *this;
        }

        const outiterator<Container>& operator= (const outiterator<Container> &) { return *this; }
        outiterator<Container>& operator* () {return *this;}
        outiterator<Container>& operator++ () {return *this;}
        outiterator<Container>& operator++ (int) {return *this;}
};

template<typename Container, typename Iter>
inline outiterator<Container, Iter> outinserter(Container& c, Iter It)
{
    return outiterator<Container, Iter>(c, It);
}

class mycont
{
    static size_t tags;
    char tag;
    MySet set_;
    MySeq seq_;

    public:
        using key_type = int;
        using value_type = int;
        using key_compare = less<int>;

        mycont(): tag('A' + tags++) {}
        mycont(int);
        mycont(mycont &&);
        mycont(const mycont &);
        template <typename iter>
        mycont(iter, iter);
        mycont& operator=(const mycont &);
        mycont& operator=(mycont &&);

        pair<Iterator, bool> insert(int, Iterator);
        Iterator insert(const Iterator& where, const int& k) { return insert(k, where).first; }

        void concat(const mycont &);
        void subst (const mycont &, size_t);
        void excl(const mycont &);
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

        void display(bool);
        int power() const { return seq_.size(); }
};

mycont::mycont(int power) : mycont()
{
    for(int i = 0; i < power; ++i)
    {
        int a = std::rand()%(power*3);
        seq_.push_back(set_.insert(a).first);
    }
}

mycont::mycont (mycont && source)
    : tag(source.tag), set_(std::move(source.set_)), seq_(std::move(source.seq_)) {}

mycont::mycont (const mycont & source): tag(source.tag)
{
    for (auto x : source.seq_)
        seq_.push_back(set_.insert(*x).first);
}

template <typename iter>
mycont::mycont (iter b, iter e): mycont()
{
    for (auto i = b; i != e; ++i)
        seq_.push_back(set_.insert(*i).first);
}

pair<Iterator, bool> mycont::insert(int key, Iterator it = nullptr)
{
    auto r = set_.insert(key, it);
    seq_.push_back(r.first);
    return r;
}


void mycont::concat(const mycont & other)
{
    for(auto x : other.seq_)
        seq_.push_back(set_.insert(*x).first);
}


void mycont::subst (const mycont & other, size_t p)
{
	if(p >= power()) 
        concat(other);
	else 
    {
		MySeq stemp(seq_.begin(), seq_.begin() + p);
		std::copy(other.seq_.begin(), other.seq_.end(), back_inserter(stemp));
		std::copy(seq_.begin()+p, seq_.end(), back_inserter(stemp));
		MySet temp;
		seq_.clear();
		for (auto x : stemp)
            seq_.push_back(temp.insert(*x).first);
		set_.swap(temp);
	}
}

void mycont::excl(const mycont & other)
{
    int n = power(),
        m = other.power();
    if(m)
        for (int p = 0; p < n; ++p)
        {
            bool f = true;
            if(*seq_[p] == other.seq_[0].Ptr->key)
            {
                int q = p, r = 0;
                if (m > 1)
                    do
                    {
                        ++q;
                        ++r;
                        int c(*seq_[q]), d(other.seq_[r].Ptr->key);
                        f &= c == d;
                    }
                    while ((r<m-1) && f);
                if(f)
                {
                    MySet temp;
                    MySeq stemp;
                    for(int i = 0; i < p; ++i)
                        stemp.push_back(temp.insert(*seq_[i]).first);
                    for(int i = p+m; i < power(); ++i)
                        stemp.push_back(temp.insert(*seq_[i]).first);
                    set_.swap(temp);
                    seq_.swap(stemp);
                    break;
                }
            }
        }
}

void mycont::erase(int left, int right)
{
    int p = power();
    left = std::min(left, p);
    right = std::min(right+1, p);
    if(left <= right)
    {
        MySet temp;
        MySeq stemp;
        for(int i = 0; i < left; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        for(int i = right; i < p; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        set_.swap(temp);
        seq_.swap(stemp);
    }
}

mycont& mycont::operator= (const mycont & other)
{
    mycont temp;
    for (auto x: other.seq_)
        seq_.push_back(temp.set_.insert(*x).first);
    seq_.swap(temp.seq_);
    set_.swap(temp.set_);
    return *this;
}

mycont& mycont::operator= (mycont && other)
{
    seq_.swap(other.seq_);
    set_.swap(other.set_);
    return *this;
}

mycont& mycont::operator&= (const mycont & other)
{
    mycont temp;
    set_intersection(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

mycont& mycont::operator|= (const mycont & other)
{
    mycont temp;
    set_union(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

mycont& mycont::operator-= (const mycont & other)
{
    mycont temp;
    set_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

mycont& mycont::operator^= (const mycont & other)
{
    mycont temp;
    set_symmetric_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

void mycont::display(bool tree_flag = false)
{
    cout << "\nМножество (" << tag << "): ";
    for(auto x : set_)
        cout << x << ' ';
    cout << "\nПоследовательность (" << tag << "): < ";
    for (auto i : seq_)
        cout << *i << ' ';
    cout << ">\n";
    if (tree_flag)
        set_.Display();
}

#endif
