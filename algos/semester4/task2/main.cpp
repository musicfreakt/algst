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

    // shape *f;
    // try
    // {
    //     f = new face(point(49, 0), point(71, 16));
    // }
    // catch(bad_init &e)
    // {
    //     f = new error_figure(e.center);
    // }

    // shape *brim;
    // try
    // {
    //     brim = new line(point(5, 18), 10);
    // }
    // catch (bad_init &e)
    // {
    //     brim = new error_figure(e.center);
    // }

    // rotatable *hat;
    // try
    // {
    //     hat = new rectangle(point(55, 20), point(69, 25));
    // }
    // catch (bad_init &e)
    // {
    //     hat = new error_figure(e.center);
    // }

    shape_refresh();
    std::cout << "=== Generated... ===\n";
    std::cin.get();	//Смотреть исходный набор

    // подготовка к сборке
    // hat->rotate_right();
    // hat->resize(2);
    //
    // brim->resize(2);
    //
    // right_horn->resize(2);
    // right_horn->rotate_left();
    //
    // left_horn->resize(2);
    // left_horn->flip_vertically();
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
