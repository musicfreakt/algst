/*
    Пополнение и использование библиотеки фигур

    Добавленные фигуры:
    - trapezoid: трапеция
    - cross: косой крест
    - crossed_trapezoid_comb: трапеция с косым крестом, реализованная наследованием
    предыдущих фигур
    - crossed_trapezoid: трапеция с косым крестом

    Добавленные функции размещения:
    - down(p, q): поместить p над q
    - left_up(p, q): поместить p слева над q
    - right_up(p, q): поместить p справа над q
    - right_down(p, q): поместить p справа под q
    - left_down(p, q): поместить p справа под q
*/

#include "screen.h"
#include "shape.h"

using namespace std;

// ========== ДОБАВКА ============

/*
    Реализация фигуры трапеция
*/

class trapezoid : public rotatable, public reflectable
/*
    nw    b------n------c   ne
         /              \
        /                \
    w  /                  \  e
      /                    \
     /                      \
    a-----------s------------d
    sw                      se
*/
{
    trapezoid(const trapezoid&);
    trapezoid(const trapezoid&&);
    trapezoid& operator=(const trapezoid &) = delete;
    trapezoid& operator=(trapezoid &&) = delete;

    protected:
        point a, b, c, d;
    public:
        trapezoid(point, int, point, int);

        point north() const { return point((swest().x + seast().x) / 2, nwest().y); }
        point south() const { return point((swest().x + seast().x) / 2, swest().y); }
        point east() const { return point(seast().x, (neast().y + seast().y) / 2); }
        point west() const { return point(swest().x, (swest().y + nwest().y) / 2); }
        point neast() const { return point(max(a.x, max(b.x, max(c.x, d.x))), max(a.y, max(b.y, max(c.y, d.y)))); }
        point seast() const { return point(max(a.x, max(b.x, max(c.x, d.x))), min(a.y, min(b.y, min(c.y, d.y)))); }
        point nwest() const { return point(min(a.x, min(b.x, min(c.x, d.x))), max(a.y, max(b.y, max(c.y, d.y))));}
        point swest() const { return point(min(a.x, min(b.x, min(c.x, d.x))), min(a.y, min(b.y, min(c.y, d.y)))); }

        void rotate_left();
        void rotate_right();
        void flip_horisontally();
        void flip_vertically();
        void move(int, int);
        void resize(int);
        void draw();
};

trapezoid :: trapezoid (point a_, int lena, point b_, int lenb)
/*
    Конструктор принимает точку sw и длину основания,
    точку nw и длину основания. Этот набор данных может
    задавать трапецию любого вида.
    Для хранения вычисляются координаты 4 точек.
*/
{
    a = a_;
    b = b_;
    c.x = b.x + lenb; c.y = b.y;
    d.x = a.x + lena; d.y = a.y;
}

void trapezoid::rotate_right()
{
    int x0 = (neast().x + seast().x + nwest().x + swest().x)/4;
    int y0 = (neast().y + seast().y + nwest().y + swest().y)/4;
    int x, y;

    x = a.x; y = a.y;
    a.x = x0 + (y - y0)*2;
    a.y = y0 - (x - x0)/2;

    x = b.x; y = b.y;
    b.x = x0 + (y - y0)*2;
    b.y = y0 - (x - x0)/2;

    x = c.x; y = c.y;
    c.x = x0 + (y - y0)*2;
    c.y = y0 - (x - x0)/2;

    x = d.x; y = d.y;
    d.x = x0 + (y - y0)*2;
    d.y = y0 - (x - x0)/2;
}

void trapezoid::rotate_left()
{
    int x0 = (neast().x + seast().x + nwest().x + swest().x)/4;
    int y0 = (neast().y + seast().y + nwest().y + swest().y)/4;
    int x, y;

    x = a.x; y = a.y;
    a.x = x0 - (y - y0)*2;
    a.y = y0 + (x - x0)/2;

    x = b.x; y = b.y;
    b.x = x0 - (y - y0)*2;
    b.y = y0 + (x - x0)/2;

    x = c.x; y = c.y;
    c.x = x0 - (y - y0)*2;
    c.y = y0 + (x - x0)/2;

    x = d.x; y = d.y;
    d.x = x0 - (y - y0)*2;
    d.y = y0 + (x - x0)/2;
}

void trapezoid :: flip_horisontally()
{
    int dx1 = abs(b.x - nwest().x),
    dx2 = abs(c.x - nwest().x),
    dx3 = abs(a.x - swest().x),
    dx4 = abs(d.x - swest().x);

    a.x = swest().x + dx1;
    b.x = nwest().x + dx3;
    c.x = nwest().x + dx4;
    d.x = swest().x + dx2;
}

void trapezoid :: flip_vertically()
{
    rotate_right();
    rotate_right();
    flip_horisontally();
}

void trapezoid :: move(int dx, int dy)
{
    a.x += dx; a.y += dy;
    b.x += dx; b.y += dy;
    c.x += dx; c.y += dy;
    d.x += dx; d.y += dy;
}

void trapezoid :: resize(int scale)
{
    b.y += (b.y - a.y) * (scale - 1);
    d.x += (d.x - a.x) * (scale - 1);
    c.x += (c.x - b.x) * (scale - 1);
    c.y += (c.y - d.y) * (scale - 1);
}

void trapezoid :: draw()
{
	put_line(a, b);
	put_line(b, c);
	put_line(c, d);
	put_line(a, d);
}

/*
    Реализация фигуры косой крест
*/

class cross : public rectangle
/*
    nw     n     ne
      \        /
        \     /
    w      c      e
        /     \
      /         \
    sw     s     se
*/
{
    cross(const cross&);
    cross(const cross&&);
    cross& operator=(const cross &) = delete;
    cross& operator=(cross &&) = delete;

    public:
        cross(point a, point b) : rectangle (a,b) {}
        void draw();
};

void cross :: draw()
{
    put_line(rectangle::swest(), rectangle::neast());
    put_line(rectangle::nwest(), rectangle::seast());
}

/*
    Реализация трапеции с косым крестом наследованием двух фигур (трапеции и креста)
*/
class crossed_trapezoid_comb : public trapezoid, public cross
{
    crossed_trapezoid_comb(const crossed_trapezoid_comb&);
    crossed_trapezoid_comb(const crossed_trapezoid_comb&&);
    crossed_trapezoid_comb& operator=(const crossed_trapezoid_comb &) = delete;
    crossed_trapezoid_comb& operator=(crossed_trapezoid_comb &&) = delete;

    public:
        crossed_trapezoid_comb(point a, int lena, point b, int lenb):
        trapezoid(a, lena, b, lenb), cross(a, point(a.x + lena, b.y)) {}

        point north() const{ return cross::north(); } // север
        point south() const{ return cross::south(); } // юг
        point east() const{ return cross::east(); } // восток
        point west() const{ return cross::west(); } // запад
        point neast() const{ return cross::neast(); } // северо-восток
        point seast() const{ return cross::seast(); } // юго-восток
        point nwest() const{ return cross::nwest(); } // северо-запад
        point swest() const{ return cross::swest(); } // северо-восток

        void rotate_left();
        void rotate_right();
        void move(int, int);
        void resize(int);
        void draw();
};

void crossed_trapezoid_comb::rotate_left()
{
    trapezoid::rotate_left();
    cross::rotate_left();
}

void crossed_trapezoid_comb::rotate_right()
{
    trapezoid::rotate_right();
    cross::rotate_right();
}

void crossed_trapezoid_comb::move(int dx, int dy)
{
    trapezoid::move(dx, dy);
    cross::move(dx, dy);
}

void crossed_trapezoid_comb::resize(int scale)
{
    trapezoid::resize(scale);
    cross::resize(scale);
}

void crossed_trapezoid_comb::draw()
{
    cross::draw();
    trapezoid::draw();
}


/*
    Реализация трапеции с косым крестом без наследования от отдельных фигур
*/

class crossed_trapezoid : public rotatable, public reflectable
{
    crossed_trapezoid(const crossed_trapezoid&);
    crossed_trapezoid(const crossed_trapezoid&&);
    crossed_trapezoid& operator=(const crossed_trapezoid &) = delete;
    crossed_trapezoid& operator=(crossed_trapezoid &&) = delete;

    protected:
        point a, b, c, d;
    public:
        crossed_trapezoid(point, int, point, int);

        point north() const { return point((swest().x + seast().x) / 2, nwest().y); }
    	point south() const { return point((swest().x + seast().x) / 2, swest().y); }
    	point east() const { return point(seast().x, (neast().y + seast().y) / 2); }
    	point west() const { return point(swest().x, (swest().y + nwest().y) / 2); }
    	point neast() const { return point(max(a.x, max(b.x, max(c.x, d.x))), max(a.y, max(b.y, max(c.y, d.y)))); }
    	point seast() const { return point(max(a.x, max(b.x, max(c.x, d.x))), min(a.y, min(b.y, min(c.y, d.y)))); }
    	point nwest() const { return point(min(a.x, min(b.x, min(c.x, d.x))), max(a.y, max(b.y, max(c.y, d.y))));}
    	point swest() const { return point(min(a.x, min(b.x, min(c.x, d.x))), min(a.y, min(b.y, min(c.y, d.y)))); }

        void rotate_left();
        void rotate_right();
        void flip_horisontally();
        void flip_vertically();
        void move(int, int);
        void resize(int);
        void draw();
};

crossed_trapezoid :: crossed_trapezoid (point a_, int lena, point b_, int lenb)
/*
    Конструктор принимает точку sw и длину основания,
    точку nw и длину основания. Этот набор данных может
    задавать трапецию любого вида.
    Для хранения вычисляются координаты 4 точек.
*/
{
    a = a_;
    b = b_;
    c.x = b.x + lenb; c.y = b.y;
    d.x = a.x + lena; d.y = a.y;
}

void crossed_trapezoid::rotate_right()
{
    int x0 = (neast().x + seast().x + nwest().x + swest().x)/4;
    int y0 = (neast().y + seast().y + nwest().y + swest().y)/4;
    int x, y;

    x = a.x; y = a.y;
    a.x = x0 + (y - y0)*2;
    a.y = y0 - (x - x0)/2;

    x = b.x; y = b.y;
    b.x = x0 + (y - y0)*2;
    b.y = y0 - (x - x0)/2;

    x = c.x; y = c.y;
    c.x = x0 + (y - y0)*2;
    c.y = y0 - (x - x0)/2;

    x = d.x; y = d.y;
    d.x = x0 + (y - y0)*2;
    d.y = y0 - (x - x0)/2;
}


void crossed_trapezoid::rotate_left()
{
    int x0 = (neast().x + seast().x + nwest().x + swest().x)/4;
    int y0 = (neast().y + seast().y + nwest().y + swest().y)/4;
    int x, y;

    x = a.x; y = a.y;
    a.x = x0 - (y - y0)*2;
    a.y = y0 + (x - x0)/2;

    x = b.x; y = b.y;
    b.x = x0 - (y - y0)*2;
    b.y = y0 + (x - x0)/2;

    x = c.x; y = c.y;
    c.x = x0 - (y - y0)*2;
    c.y = y0 + (x - x0)/2;

    x = d.x; y = d.y;
    d.x = x0 - (y - y0)*2;
    d.y = y0 + (x - x0)/2;
}

void crossed_trapezoid :: flip_horisontally()
{
    int dx1 = abs(b.x - nwest().x),
    dx2 = abs(c.x - nwest().x),
    dx3 = abs(a.x - swest().x),
    dx4 = abs(d.x - swest().x);

    a.x = swest().x + dx1;
    b.x = nwest().x + dx3;
    c.x = nwest().x + dx4;
    d.x = swest().x + dx2;
}

void crossed_trapezoid :: flip_vertically()
{
    rotate_right();
    rotate_right();
    flip_horisontally();
}

void crossed_trapezoid :: move(int dx, int dy)
{
    a.x += dx; a.y += dy;
    b.x += dx; b.y += dy;
    c.x += dx; c.y += dy;
    d.x += dx; d.y += dy;
}

void crossed_trapezoid :: resize(int r)
{
    b.y += (b.y - a.y) * (r - 1);
    d.x += (d.x - a.x) * (r - 1);
    c.x += (c.x - b.x) * (r - 1);
    c.y += (c.y - d.y) * (r - 1);
}

void crossed_trapezoid :: draw()
{
	put_line(a, b);
	put_line(b, c);
	put_line(c, d);
	put_line(a, d);

    put_line(swest(), neast());
    put_line(nwest(), seast());
}

/*
    Функции размещения фигур относительно друг друга
*/

void down(shape& p, const shape& q)
// Поместить p над q
{
	p.move(q.south().x - p.north().x, q.south().y - p.north().y - 1);
}

void left_up(shape& p, const shape& q)
// Поместить p слева над q
{
	p.move(q.nwest().x - p.swest().x, q.nwest().y - p.swest().y + 1);
}

void right_up(shape& p, const shape& q)
// Поместить p справа над q
{
	p.move(q.neast().x - p.seast().x, q.nwest().y - p.swest().y + 1);
}

void right_down(shape& p, const shape& q)
// Поместить p справа под q
{
	p.move(q.east().x - p.west().x, q.swest().y - p.nwest().y);
}

void left_down(shape& p, const shape& q)
// Поместить p справа под q
{
	p.move(q.west().x - p.east().x, q.swest().y - p.nwest().y);
}


// ========== КОНЕЦ ДОБАВКИ ============

class face: public rectangle
{
    int w, h;
    line l_eye, r_eye, mouth;
    public:
        face(point, point);
        void draw();
        void move(int, int);
        void resize(int) {};
};

face :: face (point a, point b):
    rectangle(a, b),
    w(neast().x - swest().x + 1),
	h(neast().y - swest().y + 1),
    l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
    r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
    mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{}

void face :: draw()
{
    rectangle :: draw();
    int a = (swest().x + neast().x)/2;
    int b = (swest().y + neast().y)/2;
    put_point(point(a, b));
}

void face :: move(int a, int b)
{
    rectangle :: move(a, b);
    l_eye.move(a, b);
    r_eye.move(a, b);
    mouth.move(a, b);
}

int main()
{
    screen_init();

    // объявление набора фигур
    face f(point(49, 1), point(71, 16));
    line brim(point(5, 18), 17);
    rectangle hat(point(55, 20), point(69, 25));

    crossed_trapezoid right_horn(point(10, 25), 10, point(10, 28), 6);
    crossed_trapezoid left_horn(point(10, 5), 10, point(10, 8), 6);
    crossed_trapezoid_comb shishak(point(80, 5), 16, point(85, 10), 6);

    shape_refresh();
    std::cout << "=== Generated... ===\n";
    std::cin.get();	//Смотреть исходный набор

    // подготовка к сборке
    hat.rotate_right();
    hat.resize(2);

    brim.resize(4);

    right_horn.resize(2);
    right_horn.rotate_left();

    left_horn.resize(2);
    left_horn.flip_vertically();
    left_horn.rotate_right();

    shishak.flip_horisontally();
    shape_refresh();
    std::cout << "=== Prepared... ===\n";
    std::cin.get();	//Смотреть результат поворотов/отражений

    // сборка изображения
    up(brim, f);
    up(hat, brim);
    right_up(right_horn, brim);
    left_up(left_horn, brim);
    up(shishak, hat);
    shape_refresh();
    std::cout << "=== Ready! ===\n";
    std::cin.get();	//Смотреть результат
    screen_destroy();
    return 0;
}
