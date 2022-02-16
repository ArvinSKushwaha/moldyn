#include "../input_processing.hpp"
#include <fstream>
#include <string.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"

int main(int argc, const char **argv) {
    if (argc != 2) {
        std::cerr << RED << "[Error]" << RESET << "Did not receive expected inputs." << std::endl;
        std::cout << "Usage: ./" << argv[0] << " <input_file>" << std::endl;
    }
    std::istream *input_stream;
    std::ifstream input_file;
    if (strcmp(argv[1], "-") == 0) {
        input_stream = &std::cin;
    }
    else {
        input_file.open(argv[1]);
        input_stream = &input_file;
    }
    if (!input_stream->good()) {
        std::cerr << RED << "[Error]" << RESET << "Could not open file " << argv[1] << std::endl;
        return 1;
    }
    std::vector<CVec<double, 3>> data = read(*input_stream);
    printf("%ld\n", data.size());
    for (auto &point : data) {
        printf("%f %f %f\n", point(0), point(1), point(2));
    }
    return 0;
}
