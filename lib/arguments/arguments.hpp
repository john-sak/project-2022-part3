#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <string>
#include <vector>

class arguments {
    private:
        std::string in_file;
        std::vector<std::pair<float, float>> points;
        void make_vector(void);
    public:
        arguments(char *);
        std::string get_in_file(void) const;
        std::vector<std::pair<float, float>> get_points(void) const;
};

#endif
