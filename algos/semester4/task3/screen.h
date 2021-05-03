/*
    Поддержка работы с экраном
*/

char **SCREEN;
const int OFFSET = 40;
const int MAXROW = 8;

void screen_init()
{
    SCREEN = new char*[MAXROW];
    for (int i = 0; i < MAXROW; ++i)
        SCREEN[i] = new char[80];
}

void screen_clear()
{
    for (int i=0; i < MAXROW; ++i)
        memset(SCREEN[i], '.', 80);
}

void screen_refresh()
{
    for (int i=0; i < MAXROW; ++i)
    {
        SCREEN[i][79]=0;
        std::cout << '\n' << SCREEN[i];
    }
    std::cout << '\n';
}

void screen_delete()
{
    for (int i = 0; i < MAXROW; ++i)
            delete[] SCREEN[i];
        delete[] SCREEN;
}
