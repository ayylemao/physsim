#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>
#include "vectornd.h"


class Particle {
    public:
        Vector2d pos;
        Vector2d vel;
        float eps = 1;
        float sig = 1;
        float mass = 1;

    Particle(double x = 0, double y = 0, double vx = 0, double vy = 0) : pos(x, y), vel(vx, vy)
    {
    };

    void setParams(float epsilon, float sigma, float in_mass) {
        eps = epsilon;
        sig = sigma;
        mass = in_mass;
    }
};

#endif