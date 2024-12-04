#include "Console.h"

void Console::log_matrix(Matrix4f matrix) {
    std::cout << matrix.format(Eigen::IOFormat(4, 0, ", ", "\n", "[", "]")) << std::endl;
}

void Console::log_vector(Vector4f vector) {
    std::cout << vector.transpose().format(Eigen::IOFormat(4, 0, ", ", " ", "[", "]")) << std::endl;
}