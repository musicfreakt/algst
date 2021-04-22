#include <iostream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <string>

#include "screen.h"
#include "avl_tree.h"

size_t tree::tags = 0;

int main()
{
    tree test_tree;
    // tree_iterator it;
    test_tree.insert(1);
    test_tree.insert(2);
    test_tree.insert(3);
    test_tree.insert(4);
    test_tree.insert(5);

    cout << "Size: " << test_tree.size() << " Set: ";

    test_tree.display();

    test_tree.erase(3);

    cout << "Size: " << test_tree.size() << " Set: ";
    test_tree.display();

    return 0;
}
