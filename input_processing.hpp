#ifndef MOLDYN_INPUT_PROCESSING_HPP
#define MOLDYN_INPUT_PROCESSING_HPP

#include "vec.hpp"
#include <vector>
#include <iostream>

Vec<double, 3> read_row(std::istream& is);
std::vector<Vec<double, 3>> read(std::istream& is);

#endif
