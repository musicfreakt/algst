#ifndef SCREEN_H
#define SCREEN_H
/*
    Поддержка работы с экраном
*/


const int FIRSTCOL = 60,
MAXCOL = 120, OFFSET[] = { 60, 30, 12, 5, 2, 1 },
MAXROW = 18, MAXOUT = 13, SHIFT = 2;
string SCREEN[MAXROW];

int setval(char *s, int pos, int val)
{
    std::string t(std::to_string(val));
    for (auto p : t) s[pos++] = p;
    return t.size();
}

void clrscr()
{
	for (auto i = 0; i < MAXROW; ++i) {
		SCREEN[i].clear();
		SCREEN[i].resize(MAXCOL + 10, '.');
	}
	system("cls");
}

void showscr()
{
	for (auto i = 0; i < MAXROW; ++i) {
		SCREEN[i].resize(MAXCOL);
		cout << SCREEN[i] << '\n';
	}
}

#endif