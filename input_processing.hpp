#ifndef MOLDYN_INPUT_PROCESSING_HPP
#define MOLDYN_INPUT_PROCESSING_HPP

#include "matrix.hpp"
#include <vector>
#include <iostream>

CVec<double, 3> read_row(std::istream& is);
std::vector<CVec<double, 3>> read(std::istream& is);

#endif
