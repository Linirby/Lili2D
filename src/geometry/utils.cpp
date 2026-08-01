#include "lili2d/geometry/utils.hpp"

namespace lili {

float
degToRad(float degrees) {
    return degrees * 3.14159265359f / 180.0f;
}

float
radToDeg(float radians) {
    return radians * 180.0f / 3.14159265359f;
}

}  // namespace lili
