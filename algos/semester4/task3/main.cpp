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

size_t set_seq::tags = 0;

int main()
{
    screen_init();
    set_seq a;
    set_seq b;

    a.insert(7);
    a.insert(4);
    a.insert(2);
    a.display(true);

    b.insert(7);
    b.insert(6);
    b.insert(3);
    b.insert(10);
    b.insert(12);
    b.insert(23);
    b.display(true);

    // b -= a;
    // b.display(true);

    screen_delete();
    return 0;
}
