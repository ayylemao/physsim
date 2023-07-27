#ifndef ENERGY_H
#define ENERGY_H

#include "environment.h"

class GradientDescent {
    public:
        Environment* env;
        double* grad;
        double h;

    GradientDescent(Environment* env_in, double h_in)
    : env(env_in), h(h_in) 
    {
        grad = new double[2*(env->nparticles)];
        std::fill(grad, grad + 2*(env->nparticles), 0);
    }

    void calc_grad() {
        double e_plus;
        double e_minus;
        env->calcDistances();
        for (int i = 0; i<env->nparticles; i++){
            // x_n dir
            env->particles[i].pos.x = env->particles[i].pos.x + h;
            env->enforcePBC();
            e_plus = env->calcPotEnergy();
            env->particles[i].pos.x = env->particles[i].pos.x - 2*h;
            env->enforcePBC();
            e_minus = env->calcPotEnergy();
            env->particles[i].pos.x = env->particles[i].pos.x + h;
            env->enforcePBC();

            grad[i] = (e_plus - e_minus) / (2 * h);

            env->particles[i].pos.y = env->particles[i].pos.y + h;
            env->enforcePBC();
            e_plus = env->calcPotEnergy();
            env->particles[i].pos.y = env->particles[i].pos.y - 2*h;
            env->enforcePBC();
            e_minus = env->calcPotEnergy();
            env->particles[i].pos.y = env->particles[i].pos.y + h;
            env->enforcePBC();

            grad[i+1] = (e_plus - e_minus) / (2 * h);
        }

    }

    void steepest_descent(double step_size, int maxsteps, double tol){
        double dE = 1000;
        tol = tol * env->nparticles;
        int n = 0;
        while ((dE > tol) | (n >= maxsteps)){
            dE = env->calcPotEnergy();
            calc_grad();
            for (int i = 0; i<env->nparticles; i++){
                env->particles[i].pos.x = env->particles[i].pos.x - grad[i]*step_size;
                env->particles[i].pos.y = env->particles[i].pos.y - grad[i+1]*step_size;
            }
            dE = std::abs(dE - env->calcPotEnergy());

            printf("delta E: %.5lf ==== System Energy: %.5lf\n", dE/env->nparticles, env->calcPotEnergy()/env->nparticles);
            n += 1;

        }
    }



};
#endif