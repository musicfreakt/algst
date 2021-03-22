class out_of_screen : std::exception
{
    public:
        string what;
        out_of_screen() : std::exception("\nout of screen\n") {}
        point s;
};

class bad_init : std::exception
{
    public:
        string what;
        bad_init() : std::exception("\nbad init\n") {}
        point center;
};
