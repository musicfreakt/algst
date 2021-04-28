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

size_t tree::tags = 0;

int main()
{
    screen_init();
    set_seq test;
    // tree_iterator it;
    // test_tree.insert(30);
    // test_tree.insert(40);
    // test_tree.insert(20);
    // test_tree.insert(25);
    // test_tree.insert(10);
    // test_tree.insert(15);

    test.insert(4);
    test.insert(7);
    test.insert(4);
    test.insert(2);
    test.insert(1);
    test.insert(3);
    test.insert(6);
    test.insert(5);
    test.insert(8);
    test.insert(9);
    test.display();

    // test.erase(1);
    // test.display();

    screen_delete();
    return 0;
}
