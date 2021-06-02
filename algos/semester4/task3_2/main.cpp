#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>

#include "screen.h"
#include "mycont.h"

const int N = 10; // 52
int mycont::height = 0;
int mycont::tags = 0;

void createkeys(vector<int>& a, int n) 
{
    int x;

    for (int i = 0; i < n; i++)
    {
        x = { rand() % (n + 20) + 1 };
        while (find(a.begin(), a.end(), x) != a.end())
            x = { rand() % (n + 20) + 1 };
        a.push_back(x);

    }
}

int main()
{
    std::srand(time(nullptr));
    
    vector<int> A, B, C, D, E;
    createkeys(A, N);
    createkeys(B, N);
    createkeys(C, N);
    createkeys(D, N);
    createkeys(E, N);

    mycont a(A.begin(), A.end()),
    b(B.begin(), B.end()),
    c(C.begin(), C.end()),
    d(D.begin(), D.end()),
    e(E.begin(), E.end()),
    f;

    cout << "ПРИМЕР РАБОТЫ С МНОЖЕСТВОМ:\n";
    a.display(); b.display(); c.display(); d.display(); e.display();

    cout << "ВЫЧИСЛЕНИЕ (a - b) | c ^ d & e";
    f = (a - b) | c ^ d & e;

    f.display(true);
    cout << "ПРИМЕР РАБОТЫ С ПОСЛЕДОВАТЕЛЬНОСТЬЮ:\n";

    a.display();
    b.display();
    cout << "СЦЕПЛЕНИЕ a.concat(b)\n";
    a.concat(b);
    a.display();

    c.display();
    d.display();
    cout << "СЛИЯНИЕ с.merge(d)\n";
    c.merge(d);
    c.display();

    e.display();
    cout << "УКОРАЧИВАНИЕ e.erase(1, 3)\n";
    e.erase(1, 3);
    e.display();

    return 0;
}
