#include "../matrix.hpp"
#include <string.h>
#include <iostream>
#define ASSERT(a) if (a) { std::cout << #a << std::endl; exit(1); }

void testCVecConstructor();
void testCVecClosure();
void testAdd();
void testSubtract();
void testMultiply();
void testDivide();


void testCVecConstructor() {
    CVec<float, 3> v = {1, 2, 3};
    ASSERT(v(0) != 1 || v(1) != 2 || v(2) != 3);
    CVec<float, 3> v2 = CVec<float, 3>(1.);
    ASSERT(v2(0) != 1. || v2(1) != 1. || v2(2) != 1.);
}

void testCVecClosure() {
    CVec<float, 3> v = {1, 2, 3};
    auto v2 = v.map<float>([](float x) { return x * 2; });
    ASSERT(v(0) != 1 || v(1) != 2 || v(2) != 3);
    ASSERT(v2(0) != 2 || v2(1) != 4 || v2(2) != 6);
    auto v3 = v.map2<float, float>(v2, [](float x, float y) { return x + y; });
    ASSERT(v3(0) != 3 || v3(1) != 6 || v3(2) != 9);
}

void testAdd() {
    CVec<float, 3> v = {1, 2, 3};
    CVec<float, 3> v2 = {4, 5, 6};
    CVec<float, 3> v3 = v + v2;
    ASSERT(v3(0) != 5 || v3(1) != 7 || v3(2) != 9);
    CVec<float, 3> v4 = v + 1;
    ASSERT(v4(0) != 2 || v4(1) != 3 || v4(2) != 4);
    CVec<float, 3> v5 = 1.f + v;
    ASSERT(v5(0) != 2 || v5(1) != 3 || v5(2) != 4);
}

void testSubtract() {
    CVec<float, 3> v = {1, 2, 3};
    CVec<float, 3> v2 = {4, 5, 6};
    CVec<float, 3> v3 = v - v2;
    ASSERT(v3(0) != -3 || v3(1) != -3 || v3(2) != -3);
    CVec<float, 3> v4 = v - 1;
    ASSERT(v4(0) != 0 || v4(1) != 1 || v4(2) != 2);
    CVec<float, 3> v5 = 1.f - v;
    ASSERT(v5(0) != 0 || v5(1) != -1 || v5(2) != -2);
}

void testMultiply() {
    CVec<float, 3> v = {1, 2, 3};
    CVec<float, 3> v2 = {4, 5, 6};
    CVec<float, 3> v3 = v * v2;
    ASSERT(v3(0) != 4 || v3(1) != 10 || v3(2) != 18);
    CVec<float, 3> v4 = v * 2;
    ASSERT(v4(0) != 2 || v4(1) != 4 || v4(2) != 6);
    CVec<float, 3> v5 = 2.f * v;
    ASSERT(v5(0) != 2 || v5(1) != 4 || v5(2) != 6);
}

void testDivide() {
    CVec<float, 3> v = {1, 2, 3};
    CVec<float, 3> v2 = {4, 6, 9};
    CVec<float, 3> v3 = (v2 * v) / v;
    ASSERT(v3(0) != 4 || v3(1) != 6 || v3(2) != 9);
    CVec<float, 3> v4 = (2.f * v) / 2;
    ASSERT(v4(0) != 1 || v4(1) != 2 || v4(2) != 3);
    CVec<float, 3> v5 = 6.f / v;
    ASSERT(v5(0) != 6 || v5(1) != 3 || v5(2) != 2);
}

int main() {
    testCVecConstructor();
    testCVecClosure();
    testAdd();
    testSubtract();
    testMultiply();
    testDivide();
}
