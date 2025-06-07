#include <string>
#include "../fwd.hpp"

template <typename T, size_t N>
std::string toString(const bMath::Vector<T, N> &v) {
    std::string result = "(";
    for (int i = 0; i < N; i++) {
        result += std::to_string(v[i]);
        result += ((i != N - 1) ? ", " : "");
    }
    result += ")";
    return result;
}