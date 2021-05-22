#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>

#include "screen.h"
#include "1-2tree.h"
#include "mycont.h"

const int N = 52;
size_t mycont::tags = 0;

int main()
{
    std::srand(time(nullptr));
    int menu_item;

    std::cout << "1) Test example\n2) Random data\n> ";
    cin >> menu_item;
    if (menu_item == 1)
    {
        // в примере мощность множеств мала для большей наглядности
        std::vector<int> a_ = {3, 2, 1},
        b_ = {1, 3, 4, 5},
        c_ = {1, 3, 8},
        d_ = {14, 1, 3, 8, 23, 14, 12},
        e_ = {1, 1, 0, 3, 6, 5, 4};

        mycont a(a_.begin(), a_.end()),
        b(b_.begin(), b_.end()),
        c(c_.begin(), c_.end()),
        d(d_.begin(), d_.end()),
        e(e_.begin(), e_.end()),
        f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nSet operations example: \n\n";
        f = a - (b & c & d) ^ e;

        f.display(true);
        // cout << "\n\nSequence operations example: \n\n";
        // cout << "Erase elements from 2 to 4 in (e):";
        // e.erase(2, 4);
        // e.display(true);

        // cout << "\nExcl (c) from (d):";
        // d.excl(c);
        // d.display(true);

        // cout << "\nChange (c) in (d) from 3 position:";
        // d.change(c, 3);
        // d.display(true);

    }
    else
    {
        mycont a(N), b(N), c(N), d(N), e(N), f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nSet operations example: \n\n";
        f = (a - b) | c ^ d & e;

        f.display();

		// cout << "\n\nSequence operations example: \n\n";
		// merge

		// concat

		// erase
        
        // cout << "Erase elements from 2 to 4 in (a):";
        // a.display();
        // a.erase(2, 4);
        // a.display();
    }
    return 0;
}