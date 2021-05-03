using Set = tree;
using Iterator = tree_iterator;
using Seq = std::vector<Iterator>;

const int U = 26;

class set_seq // todo: норм название
/*
    Комбинированная структура,
    хранящая множество с последовательностью.

    Множество реализовано при помощи структуры АВЛ-Дерево.
    Последовательность реализована при помощи вектора итераторов
    на элементы множества.
*/
{
    static size_t tags;
    char tag;
    Set set_; // множество
    Seq seq_; // последовательность
    set_seq& operator=(const set_seq &) = delete;
    set_seq& operator=(set_seq &&) = delete;

    public:
        set_seq() = default;
        set_seq(int);
        set_seq (set_seq &&);
        set_seq (const set_seq &);

        pair<Iterator, bool> insert(int, Iterator);

        // операции над последовательностью
        void erase(int, int);
        void excl(const set_seq&);
        void concat(const set_seq&);
        void change(const set_seq&, int);

        // операции над множеством
        // set_seq& operator &= (const set_seq &);
        // set_seq operator & (const set_seq & other) const
        // { set_seq result(*this); return (result &= other); }
        // set_seq& operator -= (const set_seq &);
        // set_seq operator - (const set_seq & other) const
        // {set_seq result(*this); return (result -= other);}


        void display(bool);
        int power() const { return seq_.size(); }
};

set_seq::set_seq(int power) : tag('A' + tags++)
{
    for(int i = 0; i < power; ++i)
        seq_.push_back(set_.insert(std::rand()%U).first);
}

set_seq::set_seq (set_seq && source)
    : tag(source.tag), set_(std::move(source.set_)), seq_(std::move(source.seq_)) {}

set_seq::set_seq (const set_seq & source): tag(source.tag)
{
    for (auto x : source.seq_)
        seq_.push_back(set_.insert(*x).first);
}

pair<Iterator, bool> set_seq::insert(int key, Iterator it = nullptr)
{
    auto r = set_.insert(key, it);
    seq_.push_back(r.first);
    return r;
}


void set_seq::erase(int left, int right)
/*
    Из последовательности исключается часть,
    ограниченная порядковыми номерами от left до right.
*/
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

void set_seq::excl(const set_seq & other)
/*
    Вторая последовательность исключается из
    первой, если она является ее частью.
*/
{
    int n = power(),
        m = other.power();
    if(m)
        for (int p = 0; p < n; ++p)
        {
            bool f = true;
            if(*seq_[p] == other.seq_[0].ptr->key)
            {
                int q = p, r = 0;
                if (m > 1)
                    do
                    {
                        ++q;
                        ++r;
                        int c(*seq_[q]), d(other.seq_[r].ptr->key);
                        f &= c == d;
                    }while ((r<m-1) && f);
                if(f)
                {
                    Set temp;
                    Seq stemp;
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


void set_seq::concat(const set_seq & other)
/*
    Сцепление
*/
{
    for(auto x : other.seq_)
        seq_.push_back(set_.insert(*x).first);
}


void set_seq::change(const set_seq & other, int pos = 0)
/*
    Вторая последовательность заменяет элементы
    первой, начиная с заданной позиции pos.
*/
{
    if(pos >= power())
        concat(other);
    else
    {
        Seq stemp(seq_.begin(), seq_.begin() + pos);
        std::copy(other.seq_.begin(), other.seq_.end(), back_inserter(stemp));

        int q = pos + other.power();
        if (q < power())
            std::copy(seq_.begin() + q, seq_.end(), back_inserter(stemp));

        Set temp;
        seq_.clear();
        for (auto x : stemp)
            seq_.push_back(temp.insert(*x).first);
        set_.swap(temp);
    }
}


// set_seq& set_seq::operator&= (const set_seq & other)
// {
//     set_seq temp;
//     set_intersection(begin(), end(),
//         other.begin(), other.end(),
//         outinserter(temp, Iterator()));
//     set_.swap(temp.set_);
//     seq_.swap(temp.seq_);
// 	return *this;
// }

// tree & tree::operator-= (const tree & other)
// {
//     tree temp;
//     set_difference(begin(), end(),
//         other.begin(), other.end(),
//         outinserter<tree, tree_iterator>(temp,tree_iterator()));
//     swap(temp);
// 	return *this;
// }
//
// tree & tree::operator^= (const tree & other)
// {
//         tree temp;
//         set_symmetric_difference(begin(), end(),
//             other.begin(), other.end(),
//             outinserter<tree, tree_iterator>(temp,tree_iterator()));
//         swap(temp);
// 	return *this;
// }


void set_seq::display(bool tree_flag = false)
{
    if (tree_flag)
        set_.display();

    std::cout << "SET: \n";
    for(auto x : set_)
        std::cout << x << " ";

    std::cout << "\nSEQUENCE: \n< ";
    for (auto i : seq_)
        std::cout << *i << " ";
    std::cout << ">\n";
}
