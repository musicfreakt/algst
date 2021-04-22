/*
    Поддержка работы с экраном
*/
const int FIRSTROW = 0, FIRSTCOL = 40,
MAXCOL = 80, OFFSET[] = {40, 24, 10, 4, 1},
MAXROW = FIRSTROW + 18,
MAXOUT = FIRSTROW + 14, SHIFT = 2;
char screen[MAXROW][MAXCOL];

//----------------------------------------------------------------------

void screen_clear()  //Очистка экрана
{
    for (auto y = 0; y < MAXROW; ++y)
        for (auto &x : screen[y])
            x = ' ';
}

void screen_refresh() // Обновление экрана
{
    for (auto y = 0; y < MAXROW; ++y)
    {
        for (auto &x : screen[y])
            std::cout << x << " ";
        std::cout<<std::endl;
    }
}
