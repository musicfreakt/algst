struct bad_init : public std::exception
/*
    Класс ошибки создания фигуры
*/
{
    char id;
    point center;
    bad_init(int id, point center) noexcept : id(id), center(center){}
    const char* what() const noexcept {return " figure out of screen when was initialized. Error figure was created.\0"; }
};

struct out_of_screen : public std::exception
/*
    Класс ошибки перемещения фигуры
*/
{
    const char* what() const noexcept {return " figure out of screen when was transformed.\0"; }
};
