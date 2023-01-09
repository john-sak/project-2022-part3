#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

#include <string.h>

#include <arguments.hpp>

void arguments::make_vector(void) {
    std::ifstream file;
    try {
        file.open(this->in_file);
        if (!file.is_open()) throw std::invalid_argument("Error opening file \'" + this->in_file + "\'");
        std::string line;
        while (std::getline(file, line)) {
            // ignore comments
            if (line.at(0) == '#') continue;
            std::string str = line.substr(line.find("\t") + 1, line.length() - 1);
            // save points as pair of 2 floating point numbers
            this->points.push_back({std::stof(str.substr(0, str.find("\t"))), std::stof(str.substr(str.find("\t") + 1, str.length() - 1))});
        }
    } catch (...) {
        file.close();
        throw;
    }
    file.close();
    return;
}

arguments::arguments(char *file) {
    
    this->in_file = std::string(file);
    try {
        make_vector();
    } catch (...) {
        throw;
    }
    return;
}

std::string arguments::get_in_file(void) const {
    return this->in_file;
}


std::vector<std::pair<float, float>> arguments::get_points(void) const {
    return this->points;
}
