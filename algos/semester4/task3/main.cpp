#include <iostream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <string>
#include <cstring>

#include "screen.h"
#include "avl_tree.h"

size_t tree::tags = 0;

int main()
{
    screen_init();
    tree test_tree;
    // tree_iterator it;
    test_tree.insert(1);
    test_tree.insert(2);
    test_tree.insert(3);
    test_tree.insert(4);
    test_tree.insert(5);

    test_tree.display();

    test_tree.erase(3);

    test_tree.display();

    screen_delete();
    return 0;
}
