// Вычислить: A \ (B ∩ C ∩ D) ⊕ E
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
    // std::srand(std::time(nullptr));

    set_seq a(10), b(10), c(10), d(10), e(10), f;

    a.display(); b.display(); c.display(); d.display(); e.display();

    f = a - ((b & c & d) ^ e);

    f.display(true);
    return 0;
}
