#include <iostream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <vector>
#include <string>
#include <cstring>

#include "screen.h"
#include "avl_tree.h"
#include "set_seq.h"

screen tree::s;
size_t set_seq::tags = 0;

int main()
{
    std::srand(time(nullptr));
    int menu_item;

    std::cout << "1) Test example\n2) Random data\n> ";
    cin >> menu_item;
    if (menu_item == 1)
    {
        std::vector<int> a_ = {1, 2, 3},
        b_ = {1, 3, 4, 5},
        c_ = {1, 3, 8},
        d_ = {1, 3, 7, 16},
        e_ = {0, 1, 2};

        set_seq a(a_.begin(), a_.end()),
        b(b_.begin(), b_.end()),
        c(c_.begin(), c_.end()),
        d(d_.begin(), d_.end()),
        e(e_.begin(), e_.end()),
        f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        f = a - (b & c & d) ^ e;

        f.display(true);
    }
    else
    {
        set_seq a(10), b(10), c(10), d(10), e(10), f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        f = a - (b & c & d) ^ e;

        f.display(true);
    }
    return 0;
}
