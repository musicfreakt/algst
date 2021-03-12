/*
    Пополнение и использование библиотеки фигур
*/
#include "screen.h"
#include "shape.h"

using namespace std;

// ========== ДОБАВКА ============

// Трапеция

class trapezium : public rotatable, public reflectable
/*
          nw-----n-----ne
         /              \
        /                \
       w                  e
      /                    \
     /                      \
    sw----------s-----------se
*/
{
    trapezium(const trapezium&);
    trapezium(const trapezium&&);
    // trapezium& operator=(const trapezium &) {}
    // trapezium& operator=(trapezium &&) {}

    protected:
        point sw, nw, ne, se;
    public:
        trapezium(point, int, point, int);

        point north() const { return point((sw.x + ne.x) / 2, ne.y); }
        point south() const { return point((sw.x + se.x) / 2, sw.y); }
        point east() const { return point((ne.x + se.x) / 2, (ne.y + se.y) / 2); }
        point west() const { return point((nw.x + sw.x) / 2, (nw.y + sw.y) / 2); }
        point neast() const { return ne; }
        point seast() const { return se; }
        point nwest() const { return nw; }
        point swest() const { return sw; }

        void rotate_left();
        void rotate_right();
        void flip_horisontally();
        void flip_vertically();
        void move(int, int);
        void resize(int);
        void draw();
};

trapezium :: trapezium (point a, int lena, point b, int lenb)
{
    sw = a;
    nw = b;
    ne.x = nw.x + lenb; ne.y = nw.y;
    se.x = sw.x + lena; se.y = sw.y;
}

void trapezium :: rotate_right()
{
    int x, y;

    x = nw.x; y = nw.y;
    nw.x = se.x + (y - se.y)*2;
    nw.y = se.y + (se.x - x)/2;

    x = ne.x; y = ne.y;
    ne.x = se.x + (y - se.y)*2;
    ne.y = se.y + (se.x - x)/2;

    x = sw.x; y = sw.y;
    sw.x = se.x + (y - se.y)*2;
    sw.y = se.y + (se.x - x)/2;
}

void trapezium :: rotate_left()
{
    int x, y;

    x = nw.x; y = nw.y;
    nw.x = sw.x + (sw.y - y)*2;
    nw.y = sw.y + (x - sw.x)/2;

    x = ne.x; y = ne.y;
    ne.x = sw.x + (sw.y - y)*2;
    ne.y = sw.y + (x - sw.x)/2;

    x = se.x; y = se.y;
    se.x = sw.x + (sw.y - y)*2;
    se.y = sw.y + (x - sw.x)/2;
}

void trapezium :: flip_horisontally()
{

}

void trapezium :: flip_vertically()
{

}

void trapezium :: move(int a, int b)
{
    sw.x += a; sw.y += b;
    nw.x += a; nw.y += b;
    ne.x += a; ne.y += b;
    se.x += a; se.y += b;
}

void trapezium :: resize(int d)
{
    nw.y += (nw.y - sw.y) * (d - 1);
    se.x += (se.x - sw.x) * (d - 1);
    ne.x += (ne.x - nw.x) * (d - 1);
    ne.y += (ne.y - se.y) * (d - 1);
}

void trapezium :: draw()
{
	put_line(nw, ne);
	put_line(ne, se);
	put_line(se, sw);
	put_line(sw, nw);
}

// Косой крест

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
    // cross& operator=(const cross &) {}
    // cross& operator=(cross &&) {}

    public:
        cross(point a, point b) : rectangle (a,b) {}
        void draw();
};

void cross :: draw()
{
    put_line(rectangle::swest(), rectangle::neast());
    put_line(rectangle::nwest(), rectangle::seast());
}

// Трапеция с косым крестом

class crossed_trapezium : public trapezium, public cross
{
    crossed_trapezium(const cross&);
    crossed_trapezium(const cross&&);

    public:
        crossed_trapezium(point a, int lena, point b, int lenb):
        trapezium(a, lena, b, lenb), cross(a, point(a.x + lena, b.y)) {}

        point north() const{ return trapezium::north(); } // север
        point south() const{ return trapezium::south(); } // юг
        point east() const{ return trapezium::east(); } // восток
        point west() const{ return trapezium::west(); } // запад
        point neast() const{ return trapezium::neast(); } // северо-восток
        point seast() const{ return trapezium::seast(); } // юго-восток
        point nwest() const{ return trapezium::nwest(); } // северо-запад
        point swest() const{ return trapezium::swest(); } // северо-восток

        void rotate_left();
        void rotate_right();
        void move(int, int);
        void resize(int);
        void draw();
};

void crossed_trapezium::rotate_left()
{
    trapezium::rotate_left();
    cross::rotate_left();
}

void crossed_trapezium::rotate_right()
{
    trapezium::rotate_right();
    cross::rotate_right();
}

void crossed_trapezium::move(int a, int b)
{
    trapezium::move(a, b);
    cross::move(a, b);
}

void crossed_trapezium::resize(int d)
{
    trapezium::resize(d);
    cross::resize(d);
}

void crossed_trapezium::draw()
{
    cross::draw();
    trapezium::draw();
}

void down(shape& p, const shape& q)
// Поместить p над q
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
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

    crossed_trapezium left_horn(point(30, 20), 10, point(30, 23), 5);
    left_horn.resize(3);
    shape_refresh();
    std::cin.get();
    left_horn.rotate_right();
    shape_refresh();
    std::cin.get();
    left_horn.rotate_right();
    shape_refresh();
    std::cin.get();
    left_horn.rotate_right();
    shape_refresh();
    std::cin.get();
    left_horn.rotate_right();
    shape_refresh();
    screen_destroy();

    // объявление набора фигур
    // face f(point(49, 1), point(71, 16));
    // line brim(point(55, 18), 17);
    // rectangle hat(point(55, 20), point(69, 25));
    //
    // // crossed_trapezium left_horn(point(0, 30), 10, point(0, 33), 5);
    // // crossed_trapezium right_horn();
    // // crossed_trapezium shishak();
    //
    // shape_refresh();
    // std::cout << "=== Generated... ===\n";
    // std::cin.get();	//Смотреть исходный набор
    //
    // // подготовка к сборке
    // hat.rotate_right();
    // hat.resize(2);
    // brim.resize(3);
    // shape_refresh();
    // std::cout << "=== Prepared... ===\n";
    // std::cin.get();	//Смотреть результат поворотов/отражений
    //
    // // сборка изображения
    // up(brim, f);
    // up(hat, brim);
    // shape_refresh();
    // std::cout << "=== Ready! ===\n";
    // std::cin.get();	//Смотреть результат
    // screen_destroy();
    return 0;
}
