#include <exception>
#include "errors.h"
#include "screen.h"
#include "shape.h"

using namespace std;

int main()
{
    screen_init();

    // объявление набора фигур

    shape *f;
    try
    {
        f = new face(point(49, 1), point(71, 16));
    }
    catch(bad_init &e)
    {
        // cout << e.what <<
        f = new error_figure(e.center, e.what);
    }

    shape *brim;
    try
    {
        brim = new line(point(5, 18), 17);
    }
    catch (bad_init &e)
    {
        // cout
        brim = new error_figure(e.center, e.what);
    }

    shape *hat;
    try
    {
        hat = new rectangle(point(55, 20), point(69, 25));
    }
    catch (bad_init &e)
    {
        // cout
        hat = new error_figure(e.center, e.what);
    }

    shape *right_horn;
    try
    {
        right_horn = new crossed_trapezoid(point(10, 25), 10, point(10, 28), 6);
    }
    catch (bad_init &e)
    {
        // cout
        right_horn = new error_figure(e.center, e.what);
    }

    shape *left_horn;
    try
    {
        left_horn = new crossed_trapezoid(point(10, 5), 10, point(10, 8), 6);
    }
    catch (bad_init &e)
    {
        // cout
        left_horn = new error_figure(e.center, e.what);
    }

    shape *shishak;
    try
    {
        shishak = new crossed_trapezoid_comb(point(80, 5), 16, point(85, 10), 6);
    }
    catch (bad_init &e)
    {
        // cout
        shishak = new error_figure(e.center, e.what);
    }

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

    cout << "\nScreen contains " << shape::shapes.size() << " elements (nose is not an element!)";
    screen_destroy();
    return 0;
}
