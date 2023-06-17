#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "include/vectornd.h"
#include "include/particle.h"
#include "include/utils.h"
#include <SFML/Graphics.hpp>




class Environment {
    public:
        int nparticles;
        double boxsize;
        double dt;
        Particle* particles;


        Environment(int numParticles, float in_boxsize, float timestep) : nparticles(numParticles), boxsize(in_boxsize), dt(timestep)
        {
            particles = new Particle[numParticles];
        };

        void initialize_particles(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<double> normal(0.0, 1.0);
            std::uniform_real_distribution<double> uniform(0.0, boxsize);
            double vx;
            double vy;
            double x;
            double y;
            for (int i; i < nparticles; i++){
                vx = normal(gen);
                vy = normal(gen);
                x = uniform(gen);
                y = uniform(gen);
                particles[i].vel.x = vx;
                particles[i].vel.y = vy;
                particles[i].pos.x = x;
                particles[i].pos.y = y;
            }
        }
        ~Environment(){
            delete[] particles;
        }
};


int main(){
    int nparticles = 100;
    double boxsize = 100;
    double dt = 0.01;

    Environment env(nparticles, boxsize, dt);
    env.initialize_particles();
    env.particles[0].pos.print();

    return 0;

};
