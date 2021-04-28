using Set = tree;
using Iterator = tree_iterator;
using Seq = std::vector<Iterator>;

class set_seq // todo: норм название
/*
    Комбинированная структура,
    хранящая множество с последовательностью.

    Множество реализовано при помощи структуры АВЛ-Дерево.
    Последовательность реализована при помощи вектора итераторов
    на элементы множества.
*/
{
    Set set_; // множество
    Seq seq_; // последовательность

    public:
        pair<Iterator, bool> insert(int, Iterator);
        void erase(int, int);
        void excl(const set_seq&);
        void change(const set_seq&, int);
        void display();
};


pair<Iterator, bool> set_seq::insert(int key, Iterator it = nullptr)
{
    auto r = set_.insert(key, it);
    seq_.push_back(r.first);
    return r;
}


void set_seq::erase (int left, int right)
/*
    Из последовательности исключается часть,
    ограниченная порядковыми номерами от left до right.
*/
{
    // for (int i = left; i < right; ++i)
    // {
    //     set.erase(seq[i]->key);
    //     seq.erase(seq.begin()+i);
    // }
}

void set_seq::excl (const set_seq& other)
/*
    Вторая последовательность исключается из
    первой, если она является ее частью.
*/
{

}

void set_seq::change (const set_seq& other, int pos = 0)
/*
    Вторая последовательность заменяет элементы
    первой, начиная с заданной позиции pos.
*/
{

}

void set_seq::display()
{
    set_.display();
    std::cout << "SEQUENCE: ";
    for (auto i : seq_)
        std::cout << *i << " ";
    std::cout << std::endl;
}
