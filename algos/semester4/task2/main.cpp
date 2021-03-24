#include <iostream>
#include <string>
#include <exception>
#include "screen.h"
#include "errors.h"
#include "shape.h"

int main()
{
    screen_init();

    // объявление набора фигур

    // rotatable *f;
    // try
    // {
    //     f = new face(point(-10, 0), point(71, 16));
    // }
    // catch(bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     f = new error_figure(e.center);
    // }

    // shape *brim;
    // try
    // {
    //     brim = new line(point(5, 18), 200);
    // }
    // catch (bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     brim = new error_figure(e.center);
    // }
    //
    // rotatable *hat;
    // try
    // {
    //     hat = new rectangle(point(55, 20), point(69, 25));
    // }
    // catch (bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     hat = new error_figure(e.center);
    // }

    // rotatable *right_horn;
    // try
    // {
    //     right_horn = new crossed_trapezoid(point(10, 25), 10, point(10, 28), 6);
    // }
    // catch (bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     right_horn = new error_figure(e.center);
    // }
    //
    // rotatable *left_horn;
    // try
    // {
    //     left_horn = new crossed_trapezoid(point(10, 5), 10, point(10, 8), 6);
    // }
    // catch (bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     left_horn = new error_figure(e.center);
    // }

    // reflectable *shishak;
    // try
    // {
    //     shishak = new crossed_trapezoid(point(80, 5), 16, point(85, 10), 6);
    // }
    // catch (bad_init &e)
    // {
    //     std::cout << e.id << e.what() << "\n";
    //     shishak = new error_figure(e.center);
    // }

    shape_refresh();
    std::cout << "=== Generated... ===\n";
    std::cin.get();	//Смотреть исходный набор

    // подготовка к сборке
    // hat->rotate_right();
    // hat->resize(100);
    //
    // brim->resize(2);
    //
    // right_horn->resize(2);
    // right_horn->rotate_left();
    //
    // left_horn->resize(2);
    // left_horn->rotate_right();
    //
    // shishak->flip_horisontally();
    // shape_refresh();
    // std::cout << "=== Prepared... ===\n";
    // std::cin.get();	//Смотреть результат поворотов/отражений

    // сборка изображения
    // up(brim, f);
    // up(hat, brim);
    // right_up(right_horn, brim);
    // left_up(left_horn, brim);
    // up(shishak, hat);
    shape_refresh();
    std::cout << "=== Ready! ===\n";
    std::cin.get();	//Смотреть результат

    // delete brim;
    std::cout << "\nScreen contains " << shape::shapes.size() << " elements (nose is not an element!)\n";
    screen_destroy();
    return 0;
}
