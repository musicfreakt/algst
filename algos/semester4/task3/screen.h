#ifndef SCREEN_H
#define SCREEN_H
/*
    Поддержка работы с экраном
*/

const int OFFSET = 40;
const int MAXROW = 8;

int setval(char *s, int pos, int val)
{
    std::string t(std::to_string(val));
    for (auto p : t) s[pos++] = p;
    return t.size();
}

struct screen
{
    char **SCREEN;
    screen();
    ~screen();
    void screen_clear();
    void screen_refresh();
};

screen::screen()
{
    SCREEN = new char*[MAXROW];
    for (int i = 0; i < MAXROW; ++i)
        SCREEN[i] = new char[80];
}

void screen::screen_clear()
{
    for (int i=0; i < MAXROW; ++i)
        memset(SCREEN[i], '.', 80);
}

void screen::screen_refresh()
{
    for (int i=0; i < MAXROW; ++i)
    {
        SCREEN[i][79]=0;
        std::cout << '\n' << SCREEN[i];
    }
    std::cout << '\n';
}

screen::~screen()
{
    for (int i = 0; i < MAXROW; ++i)
        delete[] SCREEN[i];
    delete[] SCREEN;
}

#endif
