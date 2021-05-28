#ifndef SCREEN_H
#define SCREEN_H
/*
    Поддержка работы с экраном
*/

const int FIRSTROW = 0, FIRSTCOL = 60,
MAXCOL = 120, OFFSET[100] = { 60, 29, 12, 5, 2, 1 },
MAXROW = FIRSTROW + 18,
MAXOUT = FIRSTROW + 14, SHIFT = 2;
std::string SCREEN[MAXROW];

void clrscr()
{
	for (auto i = 0; i < MAXROW; ++i) {
		SCREEN[i].clear();
		SCREEN[i].resize(MAXCOL * 2, '.');
	}
}

void showscr()
{
	for (auto i = 0; i < MAXROW; ++i) 
	{
		SCREEN[i].resize(MAXCOL);
		std::cout << SCREEN[i] << std::endl;
	}
}

int setval(std::string &s, int pos, int val) 
{ 
	std::string t(std::to_string(val));
	for (auto p : t) s[pos++] = p;
	return t.size();
}

#endif