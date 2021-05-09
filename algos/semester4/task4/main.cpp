#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <vector>
#include <chrono>

#include "avl_tree.h"

using Set = tree;
using Iterator = tree_iterator;
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

class set_seq
/*
    Комбинированная структура, хранящая множество с последовательностью.

    Множество реализовано при помощи структуры АВЛ-Дерево.
    Последовательность реализована при помощи вектора итераторов
    на элементы множества.
*/
{
    static size_t tags; // количество тегов
    char tag; // тег струтуры
    Set set_; // множество
    Seq seq_; // последовательность

    public:
        using key_type = int;
        using value_type = int;
        using key_compare = less<int>;

        set_seq(): tag('A' + tags++) {}
        set_seq(int, int);
        set_seq(set_seq &&);
        set_seq(const set_seq &);
        template <typename iter>
        set_seq(iter, iter);
        set_seq& operator=(const set_seq &);
        set_seq& operator=(set_seq &&);

        pair<Iterator, bool> insert(int, Iterator);
        Iterator insert(const Iterator& where, const int& k) { return insert(k, where).first; }

        // операции над последовательностью
        void erase(int, int);
        void prepare_excl(const set_seq&);
        void excl(const set_seq&);
        void concat(const set_seq&);
        void change(const set_seq&, int);

        // операции над множеством
        set_seq& operator &= (const set_seq &);
        set_seq operator & (const set_seq & other) const
        { set_seq result(*this); return (result &= other); }
        set_seq& operator -= (const set_seq &);
        set_seq operator - (const set_seq & other) const
        {set_seq result(*this); return (result -= other);}
        set_seq& operator ^= (const set_seq &);
        set_seq operator ^ (const set_seq & other) const
        {set_seq result(*this); return (result ^= other);}

        void display();
        int power() const { return seq_.size(); }

        friend void prepare_and(set_seq&, set_seq&, const int, int);
};

set_seq::set_seq(int power, int lim) : set_seq()
{
    for(int i = 0; i < power; ++i)
        seq_.push_back(set_.insert(std::rand()%lim).first);
}

set_seq::set_seq (set_seq && source)
    : tag(source.tag), set_(std::move(source.set_)), seq_(std::move(source.seq_)) {}

set_seq::set_seq (const set_seq & source): tag(source.tag)
{
    for (auto x : source.seq_)
        seq_.push_back(set_.insert(*x).first);
}

template <typename iter>
set_seq::set_seq (iter b, iter e): set_seq()
{
    for (auto i = b; i != e; ++i)
        seq_.push_back(set_.insert(*i).first);
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

void set_seq::prepare_excl( const set_seq& other )
{
    if (other.power())
    {
    	int a = rand()%other.power(),
        b = rand()%other.power();
    	if (b>a)
        {
    		for (int x = a; x <= b; ++x)
            {
    			auto y = other.seq_[x];
                insert(*y);
    		}
    	}
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
                    }
                    while ((r<m-1) && f);
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
    Вторая последовательность сцепляется с первой.
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

set_seq& set_seq::operator= (const set_seq & other)
{
    set_seq temp;
    for (auto x: other.seq_)
        seq_.push_back(temp.set_.insert(*x).first);
    seq_.swap(temp.seq_);
    set_.swap(temp.set_);
    return *this;
}

set_seq& set_seq::operator= (set_seq && other)
{
    seq_.swap(other.seq_);
    set_.swap(other.set_);
    return *this;
}

set_seq& set_seq::operator&= (const set_seq & other)
{
    set_seq temp;
    set_intersection(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}


set_seq& set_seq::operator-= (const set_seq & other)
{
    set_seq temp;
    set_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

set_seq& set_seq::operator^= (const set_seq & other)
{
    set_seq temp;
    set_symmetric_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

void set_seq::display()
{
    std::cout << "\nSET (" << tag << "): ";
    for(auto x : set_)
        std::cout << x << ' ';
    std::cout << "\nSEQUENCE: (" << tag << "): < ";
    for (auto i : seq_)
        std::cout << *i << ' ';
    std::cout << ">\n";
}

void prepare_and(set_seq& one, set_seq& two, const int quantity, int lim)
{
	for (int i = 0; i < quantity; ++i)
    {
		int x = rand()%lim;
        one.insert(x);
        two.insert(x);
	}
}

size_t set_seq::tags = 0;

int main()
{
    using namespace std::chrono;
    // srand((unsigned int)7);
    srand((unsigned int)time(nullptr));
    bool debug = false; //false, чтобы запретить отладочный вывод
    auto MaxMul = 10;
    int middle_power = 0, set_count = 0;
    auto Used = [&] (set_seq & t){ middle_power += t.power(); ++set_count;};
    auto DebOut = [debug] (set_seq & t) { if(debug) { t.display(); }};
    auto rand = [] (int d) { return std::rand( )%d; };
    ofstream fout("in.txt");

    // int p = rand(20) + 1; //Текущая мощность (место для цикла по p)
    int iterations = 1000;
    int max = 200;

    for (int p = rand(max) + 2; iterations > 0; --iterations, p = rand(max) + 2)
    {
            int U = 3*p; // Устанавливаем мощность универсума
            // для того, чтоб диапазон случайных чисел не был очень большим
            // в этом случае будет больше шанс того, что будет пересечение множеств
            // но и будет не так много дупликатов.
            //=== Данные ===
            set_seq A(p, U), B(p, U), C(p, U), D(p, U), E(0, U), F(p, U);
            int q_and(rand(MaxMul) + 1);
            prepare_and(A, F, q_and, U);
            if (debug) A.display(); Used(A);
            if (debug) F.display(); Used(F);
            //=== Цепочка операций ===
            // (Операция пропускается (skipped!), если аргументы некорректны)
            // Идёт суммирование мощностей множеств и подсчёт их количества,
            // измеряется время выполнения цепочки
            auto t1 = high_resolution_clock::now();
            if (debug) cout << "\n=== F&=A ===(" << q_and << ") ";
            F&=A; DebOut(F); Used(F);

            int e = rand(F.power());
            if (debug) cout << "\n=== F.change (D, " << e << ") ===";
            if (debug) D.display(); Used(D);
            F.change(D, e); DebOut(F); Used(F);

            int q_sub(rand(MaxMul) + 1);
            prepare_and(B, F, q_sub, U);
            if (debug) F.display(), B.display(); middle_power += q_sub; Used(B);
            if (debug) cout << "\n=== F-=B ===(" << q_sub << ") ";
            F-=B; DebOut(F); Used(F);

            int q_s_sub(rand(MaxMul) + 1);
            prepare_and(C, F, q_s_sub, U);
            if (debug) F.display(), C.display(); middle_power += q_s_sub; Used(C);
            if (debug) cout << "\n=== F^=C ===(" << q_s_sub << ") ";
            F^=C; DebOut(F); Used(F);

            int a, b;
            if (F.power()) a = rand(F.power()), b = rand(F.power());
            else a = 10, b = 0;
            if (debug) cout << "\n=== F.erase (" << a << "," << b << ")===";
            if (debug && a>b) cout << "(skipped!)";
            F.erase(a, b); DebOut(F); Used(F);

            if (debug) cout << "\n=== F.excl(E) ===";
            E.prepare_excl(F);
            if(debug && !E.power()) cout << "(skipped)!";
            if(debug) E.display(); Used(E);
            F.excl(E); DebOut(F); Used(F);

            auto t2 = high_resolution_clock::now();
            auto dt = duration_cast<duration<double>>(t2-t1);
            middle_power /= set_count;
            fout << p << ' ' << dt.count() << endl; //Выдача в файл
            cout << "\n=== Конец === (" << p << " : " << set_count << " * " <<
            middle_power << " DT=" << (dt.count()) <<")\n";
    }
    return 0;
}
