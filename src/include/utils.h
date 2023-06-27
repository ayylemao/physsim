#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <cmath>

namespace utils {
    double ljPot(float eps, float sigma, double r) {
        return 4 * eps * (std::pow(sigma/r, 12) - std::pow(sigma/r, 6));
    }

    float ljFor(float eps, float sigma, double r) {
        double sr = sigma / r;
        return -(24 * eps / r) * (2 * std::pow(sr, 12) - std::pow(sr, 6));
    }
}

#endif