#include "utils.hpp"

float fisqrt(float x) {
    union {
        float f;
        long l;
    } u;
    float x2;
    const float threehalfs = 1.5F;

    x2 = x * 0.5F;
    u.f = x;
    u.l = 0x5f3759df - ( u.l >> 1 );
    u.f = u.f * ( threehalfs - ( x2 * u.f * u.f ) );

    return u.f;
}
