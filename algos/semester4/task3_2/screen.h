#ifndef SCREEN_H
#define SCREEN_H
/*
    Поддержка работы с экраном
*/

const int FIRSTCOL = 60,
MAXCOL = 120, OFFSET[] = { 60, 30, 12, 5, 2, 1 },
MAXROW = 18, MAXOUT = 13, SHIFT = 2;
std::string SCREEN[MAXROW];

void clrscr()
{
	for (auto i = 0; i < MAXROW; ++i) 
    {
		SCREEN[i].clear();
		SCREEN[i].resize(MAXCOL + 10, '.');
	}
	// system("cls");
}

void showscr()
{
	for (auto i = 0; i < MAXROW; ++i) 
    {
		SCREEN[i].resize(MAXCOL);
		std::cout << SCREEN[i] << '\n';
	}
}

#endif