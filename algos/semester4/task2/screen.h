/*
    Поддержка работы с экраном
*/
const int XMAX = 120;
const int YMAX = 50;

struct point
{
    int x, y;
    point(int a = 0, int b = 0) : x(a), y(b) {}
};

void put_point(int a, int b); // Вывод точки (2 варианта)
void put_point(point p) { put_point(p.x, p.y); } //
void put_line(int, int, int, int); // Вывод линии (2 варианта)
void put_line(point a, point b) { put_line(a.x, a.y, b.x, b.y); }
void screen_init(); // Создание экрана
void screen_destroy(); // Удаление
void screen_refresh(); // Обновление
void screen_clear(); // Очистка
