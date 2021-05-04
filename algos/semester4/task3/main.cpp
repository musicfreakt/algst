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
    set_seq A;
    set_seq B;

    A.insert(7);
    A.insert(4);
    A.insert(2);
    A.display();

    B.insert(7);
    B.insert(6);
    B.insert(3);
    B.insert(10);
    B.insert(12);
    B.insert(23);
    B.display();

    A -= B;
    A.display(true);

    screen_delete();
    return 0;
}
