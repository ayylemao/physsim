#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>
#include "vectornd.h"


class Particle {
    protected:
        Vector2d* pos;
        Vector2d* vel;

    public:
        Particle() {
        }
        
        ~Particle() {
            delete pos;
            delete vel;
        }

    void init_pos(Vector2d* r_vec){
        pos = r_vec;
    }

    void init_vel(Vector2d* v_vec){
        vel = v_vec;
    }

    void mod_pos(Vector2d* new_pos){
        pos->x = new_pos->x;
        pos->y = new_pos->y;
    }

    void mod_vel(Vector2d* new_vel){
        vel->x = new_vel->x;
        vel->y = new_vel->y;
    }

    Vector2d getPos(){
        return *pos;
    }

    Vector2d getVel(){
        return *vel;
    }


    void printRV(){
        std::cout << '\n';
        std::cout << "r: ";
        pos->printVector();
        std::cout << "v: ";
        vel->printVector();
        std::cout << '\n';
    }
};


class LJParticle : public Particle {
    public:
        const float eps;
        const float sigma;

        LJParticle(float init_eps, float init_sigma) : eps(init_eps), sigma(init_sigma) {

        }

        LJParticle() : eps(1), sigma(1) {

        }
};

#endif