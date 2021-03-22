#include <exception>
#include "errors.h"
#include "screen.h"
#include "shape.h"

using namespace std;

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
