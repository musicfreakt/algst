#ifndef SHAPE_H
#define SHAPE_H

#include <list>
#include <iostream>

using namespace std;

char screen[YMAX][XMAX];
enum color { black = '*', white = '.', error = '!'};
bool create_error = false;

void screen_init()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])
            x = white;
}

void screen_destroy()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])
            x = black;
}

bool on_screen(int a, int b)
// проверка попадания точки на экран
{
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
    // if(create_error)
    //     screen[(abs(a) % XMAX)][(abs(b) % YMAX)] = error;
    // else
    if (on_screen(a, b))
        screen[a][b] = black;
    else
    {
        out_of_screen e;
        throw e; // бросаем исключение в put_line
    }
}

void put_line(int x0, int y0, int x1, int y1)
/* Алгоритм Брезенхэма для прямой:
рисование отрезка прямой от (x0,y0) до (x1,y1).
Уравнение прямой: b(x-x0) + a(y-y0) = 0.
Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).
*/
{
    try
    {
        int dx = 1;
        int a = x1 - x0; if (a < 0) dx = -1, a = -a;
        int dy = 1;
        int b = y1 - y0; if (b < 0) dy = -1, b = -b;
        int two_a = 2*a;
        int two_b = 2*b;
        int xcrit = -b + two_a;
        int eps = 0;
        for (;;)
        { //Формирование прямой линии по точкам
            put_point(x0, y0);
            if (x0 == x1 && y0 == y1) break;
            if (eps <= xcrit) x0 += dx, eps += two_b;
            if (eps >= a || a < b) y0 += dy, eps -= two_a;
        }
    }
    catch(out_of_screen &e)
    {
        // ничего не можем сделать, передаем исколчение в draw
        throw e;
    }
}

void screen_clear(){ screen_init(); } //Очистка экрана

void screen_refresh() // Обновление экрана
{
    for (int y = YMAX-1; 0 <= y; --y) // с верхней строки до нижней
    {
        for (auto x : screen[y]) // от левого столбца до правого
            cout << x;
        cout << '\n';
    }
}

struct shape
{
    static int id; // уникальный идентификатор фигуры
    static list<shape*> shapes; // Список фигур (один на все фигуры!)
    shape() {shapes.push_back(this); ++id;} //Фигура присоединяется к списку
    virtual point north() const = 0; //Точки для привязки
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0; //Рисование
    virtual void move(int, int) = 0; //Перемещение
    virtual void resize(int) = 0; //Изменение размера
};

int shape::id = 0;
list <shape*> shape::shapes; // Размещение списка фигур
void shape_refresh() // Перерисовка всех фигур на экране
{
    screen_clear();
    for (auto p : shape::shapes)
        p->draw();
    screen_refresh();
}

void shape_refresh()
{
    screen_clear();
    for(auto p : shape::shapes)
    {
        try
        {
            p.front()->draw();
        }
        catch(out_of_screen &e)
        {
            cout << endl << e.what << " went out of screen when was initialized. Garbage was created\n";
            shape *s = new garbage(e.s);
            shape::shapes.remove(p.front());
        }
    }
    screen_refresh();
}


class rotatable : virtual public shape
{
    //Фигуры, пригодные к повороту
    public:
        virtual void rotate_left() = 0;
        virtual void rotate_right() = 0;
};

class reflectable : virtual public shape
{
    //Фигуры пригодные к зеркальному отражению
    public:
        virtual void flip_horisontally() = 0;
        virtual void flip_vertically() = 0;
};


/*
    Специальная фигура - знак ошибки
*/

class error_figure : virtual public shape
{
    point s;
    public:
        point north() const{ return point(s.x, s.y+1); }
        point south() const{ return point(s.x, s.y - 1); }
        point east() const{ return point (s.x+1, s.y); }
        point west() const{ return point(s.x-1, s.y); }
        point neast() const{ return point(s.x + 1, s.y + 1); }
        point seast() const{ return point(s.x + 1, s.y - 1); }
        point nwest() const{ return point(s.x - 1, s.y + 1); }
        point swest() const{ return point(s.x - 1, s.y - 1); }

        void move(int, int);
        void draw();
};


void error_figure::move(int dx, int dy)
{
    s.x += dx;
    s.y += dy;
}

void error_figure::draw()
{
    create_error = true;
    put_line(nwest(), neast());
    put_line(neast(), seast());
    put_line(seast(), swest());
    put_line(swest(), nwest());
    create_error = false;
}

// Линия

class line : public shape
/*
    Отрезок прямой ["w", "e"]
    north() определяет точку "выше центра отрезка и так далеко на север,
    как самая его северная точка", и т.п.
*/
{
    protected:
        point w, e;
    public:
        line(point, point);
    	line(point, int);

    	point north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
    	point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
    	point east() const { return e; }
    	point west() const { return w; }
    	point neast() const { return e; }
    	point seast() const { return e; }
    	point nwest() const { return w; }
    	point swest() const { return w; }

    	void move(int, int);
    	void draw(){put_line(w, e);}
        void resize(int);

        ~line(){shape::shapes.remove(this);}
};

line::line (point a, point b) : w(a), e(b)
{
    if((!on_screen(w.x, w.y)) || (!on_screen(e.x,e.y)))
    {
        bad_init i;
        // i.what = name;
        i.center = point((west().x - east().x)/2, (north().y - south().y)/2);
        this->~line();
        throw i;
    }
};

line::line(point a, int l): w(point(a.x + l - 1, a.y)), e(a)
{
    if((!on_screen(w.x, w.y)) || (!on_screen(e.x,e.y)))
    {
        bad_init i;
        // i.what = name;
        i.center = point((west().x - east().x)/2, (north().y - south().y)/2);
        this->~line();
        throw i;
    }
};

void line::move(int dx, int dy)
{
    w.x += dx; w.y += dy; e.x += dx; e.y += dy;
    if(!on_screen(w.x, w.y) || !on_screen(e.x, e.y))
    {
        out_of_screen e;
        // e.what = name;
        e.s = point(0,0);
        this->~line();
        throw e;
    }
}

void resize(int d)
{
    e.x += (e.x - w.x) * (d - 1);
    e.y += (e.y - w.y) * (d - 1);
}

// Прямоугольник

class rectangle: public rotatable
/*
    nw-----n-----ne
    |             |
    |             |
    w      c      e
    |             |
    |             |
    sw-----s-----se
*/
{
    protected:
        point sw, ne;
    public:
        rectangle(point, point);

    	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
    	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
    	point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
    	point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
    	point neast() const { return ne; }
    	point seast() const { return point(ne.x, sw.y); }
    	point nwest() const { return point(sw.x, ne.y); }
    	point swest() const { return sw; }

    	void rotate_right();
    	void rotate_left();
    	void move(int, int);
        void resize(int);
    	void draw();

        ~rectangle(){shape::shapes.remove(this);}
};

rectangle::rectangle(point a, point b)
{
    if (a.x <= b.x)
		if (a.y <= b.y) {
			sw = a;
			ne = b;
		}
		else {
			sw = point(a.x, b.y);
			ne = point(b.x, a.y);
		}
	else
		if (a.y <= b.y) {
			sw = point(b.x, a.y);
			ne = point(a.x, b.y);
		}
		else {
			sw = b;
			ne = a;
		}

    if((!on_screen(sw.x, sw.y)) || (!on_screen(ne.x,ne.y)))
    {
        bad_init i;
        // i.what = name;
        i.center = point(west().y, north().x);
        this->~rectangle();
        throw i;
    }
}

void rectangle::draw()
{
    try
    {
    	point nw(sw.x, ne.y);
    	point se(ne.x, sw.y);
    	put_line(nw, ne);
    	put_line(ne, se);
    	put_line(se, sw);
    	put_line(sw, nw);
    }
    catch(out_of_screen &e)
    {
        // e.what = name;
        e.s = point((sw.x+ne.x)/2, (sw.y+ne.y)/2);
        this->~rectangle();
        throw e; // кидаем его выше в цикл
    }
}

void rectangle::resize(int d)
{
    ne.x += (ne.x - sw.x) * (d - 1);
    ne.y += (ne.y - sw.y) * (d - 1);
}

void rectangle::move(int a, int b)
{
    sw.x += a;
    sw.y += b;
    ne.x += a;
    ne.y += b;
}

void rectangle::rotate_right()
{
    int w = ne.x - sw.x, h = ne.y - sw.y;
    sw.x = ne.x - h * 2;
    ne.y = sw.y + w / 2;
}

void rectangle::rotate_left()
{
    int w = ne.x - sw.x, h = ne.y - sw.y;
    ne.x = sw.x + h * 2;
    ne.y = sw.y + w / 2;
}

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
    // trapezoid(const trapezoid&);
    // trapezoid(const trapezoid&&);
    // trapezoid& operator=(const trapezoid &) = delete;
    // trapezoid& operator=(trapezoid &&) = delete;

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
    // cross(const cross&);
    // cross(const cross&&);
    // cross& operator=(const cross &) = delete;
    // cross& operator=(cross &&) = delete;

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
    // crossed_trapezoid_comb(const crossed_trapezoid_comb&);
    // crossed_trapezoid_comb(const crossed_trapezoid_comb&&);
    // crossed_trapezoid_comb& operator=(const crossed_trapezoid_comb &) = delete;
    // crossed_trapezoid_comb& operator=(crossed_trapezoid_comb &&) = delete;

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
    // crossed_trapezoid(const crossed_trapezoid&);
    // crossed_trapezoid(const crossed_trapezoid&&);
    // crossed_trapezoid& operator=(const crossed_trapezoid &) = delete;
    // crossed_trapezoid& operator=(crossed_trapezoid &&) = delete;

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

void up(shape& p, const shape& q)
{
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

#endif
