struct bad_init : public std::exception
{
    int id;
    point center;
    bad_init(int id, point center) noexcept : id(id), center(center){}
    const char* what() const noexcept {return " figure out of screen when was initialized. Error figure was created."; }
};

// class out_of_screen : public std::exception
// {
//     public:
//         const char* what() const noexcept { return ""; }
//         point s;
// };
