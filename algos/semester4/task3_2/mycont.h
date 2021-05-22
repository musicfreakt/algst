#ifndef MYCONT_H
#define MYCONT_H

using Set = Tree;
using Iterator = tree_iter;
using Seq = std::vector<Iterator>;

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
    static size_t tags;
    char tag;
    Set set_;
    Seq seq_;

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

        // операции над последовательностью
        void merge(const mycont&);
        void concat(const mycont&);
        void erase(int, int);

        // операции над множеством
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
        seq_.push_back(set_.insert(std::rand()%(power*3)).first);
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

void mycont::erase(int left, int right)
{
    int p = power();
    left = std::min(left, p);
    right = std::min(right+1, p);
    if(left <= right)
    {
        Set temp;
        Seq stemp;
        for(int i = 0; i < left; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        for(int i = right; i < p; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        set_.swap(temp);
        seq_.swap(stemp);
    }
}

void mycont::concat(const mycont & other)
{
    for(auto x : other.seq_)
        seq_.push_back(set_.insert(*x).first);
}

void mycont::merge(const mycont & other) 
{ 
	using std::sort;
	Seq temp(other.seq_), res;
	auto le = [ ] (MyIt a, MyIt b)->bool { return *a < *b; };
	std::merge(seq_.begin(), seq_.end(), temp.begin( ), temp.end( ), 
        std::back_inserter(res), le);
    for (auto x : other.set_)
	    set_.insert(x);
	seq_.swap(res);
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
    cout << "\nSET (" << tag << "): ";
    for(auto x : set_)
        cout << x << ' ';
    cout << "\nSEQUENCE: (" << tag << "): < ";
    for (auto i : seq_)
        cout << *i << ' ';
    cout << ">\n";
    if (tree_flag)
        set_.display();
}

#endif
