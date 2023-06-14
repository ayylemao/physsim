#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <cmath>

namespace utils {
    
    float randUniform(float lower, float upper) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(lower, upper);
        return dis(gen);
    }

    float ljPot(float eps, float sigma, float r) {
        return 4 * eps * (std::pow(sigma/r, 12) - std::pow(sigma/r, 6));
    }
};

#endif