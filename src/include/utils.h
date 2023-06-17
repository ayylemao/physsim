#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <cmath>

namespace utils {
    float ljPot(float eps, float sigma, double r) {
        return 4 * eps * (std::pow(sigma/r, 12) - std::pow(sigma/r, 6));
    }

    float ljFor(float eps, float sigma, double r) {
        return ((24*eps)/r) * (2*std::pow(sigma/r, 12)-std::pow(sigma/r, 6));
    }
}

#endif