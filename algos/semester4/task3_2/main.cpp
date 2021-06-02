#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>

#include "screen.h"
#include "1-2tree.h"

const int N = 10; // 52
int Tree::height = 0;
int Tree::tags = 0;

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
    
    return 0;
}

// // Выражение A & B ^ C & D & E

// int k = 0, l = 0, N = 0;

// vector<int> A, B, C, D, E;

// int p = rand() % (10) + 5;

// cout << "Подготовка множеств длины " << p << endl;

// createkeys(A, p);

// createkeys(B, p);

// createkeys(C, p);

// createkeys(D, p);

// createkeys(E, p);

// cout << "Множества успешно созданы" << endl;

// system("pause");

// Tree first, second, third, fourth, fifth, answer1, answer2, answer3, answer4;

// for (auto x : A) {

// first.insert(x);

// }

// first.Display("keys A");

// system("pause");

// for (auto x : B) {

// second.insert(x);

// }

// second.Display("keys B");

// system("pause");

// for (auto x : C) {

// third.insert(x);

// }

// third.Display("keys C");

// system("pause");

// for (auto x : D) {

// fourth.insert(x);

// }

// fourth.Display("keys D");

// system("pause");

// for (auto x : E) {

// fifth.insert(x);

// }

// fifth.Display("keys E");

// system("pause");

// answer1 = first & second;

// answer1.Display("A & B");

// system("pause");

// answer2 = third & fourth;

// answer2.Display("C & D");

// system("pause");

// answer3 = answer2 & fifth;

// answer3.Display("C & D & E");

// system("pause");

// answer4 = answer1 ^ answer3;

// answer4.Display("A & B ^ C & D & E");

// system("pause");

// size_t pos;

// pos = rand() % (first.Power() + 1 ) + 1;

// cout << "операция CHANGE будет выполняться с позиции: " << pos << endl;

// first.CHANGE(second, pos);

// first.Display("A CHANGE B");

// system("pause");

// cout << "Операция EXCL будет использовать CHANGE вместо множества B: " << endl;

// first.EXCL(second);

// first.Display("A EXCL B");

// system("pause");

// pos = rand() % (third.Power() + 1) + 0;

// cout << "операция SUBST будет выполняться с позиции: " << pos << endl;

// third.SUBST(fourth, pos);

// string subs = "C SUBST D (" + to_string(pos) + ")";

// third.Display(subs);

// system("pause");

// pos = rand() % (4) + 1;

// cout << "операция MUL будет выполняться " << pos << " раз(а)" << endl;

// fifth.MUL(pos);

// string mul = "E MUL (" + to_string(pos) + ")";

// fifth.Display(mul);

// return 0;

// } 