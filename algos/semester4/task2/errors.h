class out_of_screen : public std::exception
/*
    Класс ошибки непопадания фигуры на экран
*/
{
    std::string what_str;

    public:
        out_of_screen(const std::string &what_str) noexcept : what_str(what_str) {}
        const char* what() const noexcept {return what_str.c_str();}
};
