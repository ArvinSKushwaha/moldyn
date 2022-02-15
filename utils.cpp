#include "utils.hpp"

float fisqrt(float x) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = x * 0.5F;
    y = x;
    i = *reinterpret_cast<long *>(&y);
    i = 0x5f3759df - ( i >> 1 );
    y = *reinterpret_cast<float *>(&i);
    y = y * ( threehalfs - ( x2 * y * y ) );

    return y;
}
