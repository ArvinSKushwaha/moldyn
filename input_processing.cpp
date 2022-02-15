#include "input_processing.hpp"
#include <string>

Vec<double, 3> read_row(std::istream& is) {
    double x, y, z;
    is >> x >> y >> z;
    return {x, y, z};
}

std::vector<Vec<double, 3>> read(std::istream& is) {
    unsigned long vecCount;
    is >> vecCount;
    std::vector<Vec<double, 3>> result(vecCount);
    for (auto& vec : result) {
        vec = read_row(is);
    }
    return result;
}
