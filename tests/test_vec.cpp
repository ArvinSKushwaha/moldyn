#include "../vec.hpp"
#include <string.h>
#include <iostream>
#define ASSERT(a) if (a) { std::cout << #a << std::endl; exit(1); }

void testVecConstructor();
void testVecClosure();
void testAdd();
void testSubtract();
void testMultiply();
void testDivide();


void testVecConstructor() {
    Vec<float, 3> v = {1, 2, 3};
    ASSERT(v[0] != 1 || v[1] != 2 || v[2] != 3);
    Vec<float, 3> v2 = Vec<float, 3>(1.);
    ASSERT(v2[0] != 1. || v2[1] != 1. || v2[2] != 1.);
}

void testVecClosure() {
    Vec<float, 3> v = {1, 2, 3};
    auto v2 = v.map<float>([](float x) { return x * 2; });
    ASSERT(v[0] != 1 || v[1] != 2 || v[2] != 3);
    ASSERT(v2[0] != 2 || v2[1] != 4 || v2[2] != 6);
    auto v3 = v.map2<float>(v2, [](float x, float y) { return x + y; });
    ASSERT(v3[0] != 3 || v3[1] != 6 || v3[2] != 9);
}

void testAdd() {
    Vec<float, 3> v = {1, 2, 3};
    Vec<float, 3> v2 = {4, 5, 6};
    Vec<float, 3> v3 = v + v2;
    ASSERT(v3[0] != 5 || v3[1] != 7 || v3[2] != 9);
    Vec<float, 3> v4 = v + 1;
    ASSERT(v4[0] != 2 || v4[1] != 3 || v4[2] != 4);
    Vec<float, 3> v5 = 1.f + v;
    ASSERT(v5[0] != 2 || v5[1] != 3 || v5[2] != 4);
}

void testSubtract() {
    Vec<float, 3> v = {1, 2, 3};
    Vec<float, 3> v2 = {4, 5, 6};
    Vec<float, 3> v3 = v - v2;
    ASSERT(v3[0] != -3 || v3[1] != -3 || v3[2] != -3);
    Vec<float, 3> v4 = v - 1;
    ASSERT(v4[0] != 0 || v4[1] != 1 || v4[2] != 2);
    Vec<float, 3> v5 = 1.f - v;
    ASSERT(v5[0] != 0 || v5[1] != -1 || v5[2] != -2);
}

void testMultiply() {
    Vec<float, 3> v = {1, 2, 3};
    Vec<float, 3> v2 = {4, 5, 6};
    Vec<float, 3> v3 = v * v2;
    ASSERT(v3[0] != 4 || v3[1] != 10 || v3[2] != 18);
    Vec<float, 3> v4 = v * 2;
    ASSERT(v4[0] != 2 || v4[1] != 4 || v4[2] != 6);
    Vec<float, 3> v5 = 2.f * v;
    ASSERT(v5[0] != 2 || v5[1] != 4 || v5[2] != 6);
}

void testDivide() {
    Vec<float, 3> v = {1, 2, 3};
    Vec<float, 3> v2 = {4, 6, 9};
    Vec<float, 3> v3 = (v2 * v) / v;
    ASSERT(v3[0] != 4 || v3[1] != 6 || v3[2] != 9);
    Vec<float, 3> v4 = (2.f * v) / 2;
    ASSERT(v4[0] != 1 || v4[1] != 2 || v4[2] != 3);
    Vec<float, 3> v5 = 6.f / v;
    ASSERT(v5[0] != 6 || v5[1] != 3 || v5[2] != 2);

    // Test Division By 0
    try {
        Vec<float, 3> v6 = v / 0;
        ASSERT(true);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        ASSERT(strcmp(e.what(), "Division by zero") != 0);
    }
    Vec<float, 3> zVec = {1., 2., 0.};
    try {
        Vec<float, 3> v7 = 2.f / zVec;
        ASSERT(true);
    } catch (const std::exception& e) {
        ASSERT(strcmp(e.what(), "Division by zero") != 0);
    }
    Vec<float, 3> zVec2 = {1., 0., 1.};
    try {
        Vec<float, 3> v8 = zVec2 / zVec;
        ASSERT(true);
    } catch (const std::exception& e) {
        ASSERT(strcmp(e.what(), "Division by zero") != 0);
    }
}

int main() {
    testVecConstructor();
    testVecClosure();
    testAdd();
    testSubtract();
    testMultiply();
    testDivide();
}
