
class out_of_screen : public std::exception
{
    public:
        // string m_error;
        out_of_screen() {std::cout << "\nout of screen\n";}
        // const char* what() const noexcept { return m_error.c_str(); }
        const char* what() const noexcept {return "1\0";}
        point s;
};

class bad_init : public std::exception
{
    public:
        // string m_error;
        bad_init() {std::cout << "\nbad init\n";}
        // const char* what() const noexcept { return m_error.c_str(); }
        const char* what() const noexcept {return "1\0";}
        point center;
};
