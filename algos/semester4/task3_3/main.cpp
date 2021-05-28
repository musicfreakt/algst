#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>

#include "screen.h"
#include "2-3-Tree.h"
#include "mycont.h"

const int N = 16;
size_t mycont::tags = 0;

int main()
{
    std::srand(time(nullptr));
    int menu_item;

    std::cout << "1. Тестовый пример\n2. Случайные данные\n> ";
    cin >> menu_item;
    if (menu_item == 1)
    {
        std::vector<int> a_ = {1, 2, 3},
        b_ = {1, 3, 4, 5},
        c_ = {1, 3, 8},
        d_ = {1, 3, 8, 12, 14, 14, 27},
        e_ = {0, 0, 1, 3, 4, 5, 6};

        mycont a(a_.begin(), a_.end()),
        b(b_.begin(), b_.end()),
        c(c_.begin(), c_.end()),
        d(d_.begin(), d_.end()),
        e(e_.begin(), e_.end()),
        f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nПример операций над множеством: \n\n";
        f = a - (b ^ c & d) | e;
        f.display(true);

        cout << "\n\nПример операций над последовательностью: \n\n";
        e.display();
        cout << "Erase (от 1 до 3) в (e):";
        e.erase(1, 3);
        e.display();

        c.display();
        cout << "\nConcat c и d:";
        c.concat(d);
        c.display();

        cout << "\nExcl d из с:";
        c.excl(d);
        c.display();

        d.display();
        cout << "\nSubst d в b с 2:";
        d.subst(b, 2);
        d.display();
    }
    else
    {
        mycont a(N), b(N), c(N), d(N), e(N), f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nПример операций над множеством: \n\n";
        f = (a - b) | c ^ d & e;

        f.display();

		cout << "\n\nПример операций над последовательностью: \n\n";
        e.display();
        cout << "Erase (от 1 до 3) в (e):";
        e.erase(1, 3);
        e.display();

        c.display();
        cout << "\nConcat c и d:";
        c.concat(d);
        c.display();

        cout << "\nExcl d из с:";
        c.excl(d);
        c.display();

        d.display();
        cout << "\nSubst d в b с 2:";
        d.subst(b, 2);
        d.display();
    }
    return 0;
}
