#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>
#include <chrono>

#include "2-3-Tree.h"
#include "mycont.h"

size_t mycont::tags = 0;

int main()
{
    using namespace std::chrono;
    ofstream fout("in.txt");
    srand((unsigned int)time(nullptr));

    auto MaxMul = 5;
    int middle_power = 0, set_count = 0;
    int iterations = 200;
    int max_ = 2000;
    bool debug = false;

    auto Used = [&] (mycont & t){ middle_power += t.power(); ++set_count;};
    auto rand = [] (int d) { return std::rand()%d; };
    auto DebOut = [debug] (mycont & t) { if(debug) { t.display(); }};

    for (int p = rand(max_) + 5; iterations > 0; --iterations, p = rand(max_) + 5)
    {
        int U = 3*p; // Устанавливаем мощность универсума
        //=== Данные ===
        mycont A(p, U), B(p, U), C(p, U), D(p, U), E(p, U), F(0, U);
        //=== Цепочка операций ===
        // (Операция пропускается (skipped!), если аргументы некорректны)
        // Идёт суммирование мощностей множеств и подсчёт их количества,
        // измеряется время выполнения цепочки
        auto t1 = high_resolution_clock::now();

        int q_and(rand(MaxMul) + 1);
        prepare_and(A, F, q_and, U);
        if (debug) A.display(); Used(A);
        if (debug) F.display(); Used(F);
        if (debug) cout << "\n=== F&=A ===(" << q_and << ") ";
        F&=A; DebOut(F); Used(F);

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
        else a = b = 0;
        if (debug) cout << "\n=== F.erase (" << a << "," << b << ")===";
        if (a>b) swap(a, b);
        F.erase(a, b); DebOut(F); Used(F);

        if (debug) cout << "\n=== F.concat(E) ===";
        if (debug) E.display(), F.display(); Used(E);
        F.concat(E); DebOut(F); Used(F);

        if (debug) cout << "\n=== F.excl(E) ===";
        if(debug && !E.power()) cout << "(skipped)!";
        if(debug) E.display(); Used(E);
        F.excl(E); DebOut(F); Used(F);

        int d = rand(F.power());
	    if (debug) cout << "\n=== F.subst (D, " << d << ") ===";
	    if (debug) D.display(); Used(D);
	    F.subst(D, d); DebOut(F); Used(F);

        auto t2 = high_resolution_clock::now();
        auto dt = duration_cast<duration<double>>(t2-t1);
        middle_power /= set_count;
        if (!debug) fout << middle_power << ' ' << dt.count() << endl;
        cout << "\n=== Конец === (" << p << " : " << set_count << " * " <<
        middle_power << " DT=" << (dt.count()) <<")\n";
        middle_power = 0; set_count = 0;
    }
    return 0;
}
