#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <random>
#include "vectornd.h"
#include "particle.h"
#include "utils.h"

class Environment {
    public:
        int nparticles;
        double boxsize;
        double dt;
        double inela;
        Particle* particles;
        double **distances;
        Vector2d* forces;


        Environment(int numParticles, float in_boxsize, float timestep, float inelasticity) 
        : nparticles(numParticles), boxsize(in_boxsize), dt(timestep), inela(inelasticity)
        {
            particles = new Particle[numParticles];
            distances = new double*[nparticles];
            forces = new Vector2d[nparticles];
            for (int i = 0; i < nparticles; i++) {
                distances[i] = new double[nparticles];
                std::fill(distances[i], distances[i] + nparticles, 0.0);
            }
        }

        void initialize_particles(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<double> normal(0.0, 10.0);
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

        void enforceBoundary(){
            for (int i = 0; i < nparticles; i++){
                if (particles[i].pos.x > boxsize) {
                    if (particles[i].vel.x > 0.0) {
                        particles[i].vel.x = -inela*particles[i].vel.x;
                    }
                } else if (particles[i].pos.x < 0.0) {
                    if (particles[i].vel.x < 0.0) {
                        particles[i].vel.x = -inela*particles[i].vel.x;
                    }
                }
                if (particles[i].pos.y > boxsize) {
                    if (particles[i].vel.y > 0.0) {
                        particles[i].vel.y = -inela*particles[i].vel.y;
                    }
                } else if (particles[i].pos.y < 0.0) {
                    if (particles[i].vel.y < 0.0) {
                        particles[i].vel.y = -inela*particles[i].vel.y;
                    }
                }
            }
        }

        void calcDistances(){
            for (int i = 0; i < nparticles; i++) {
                for (int j = i; j < nparticles; j++) {
                    if (i != j) {
                        double dist = particles[i].pos.dist(particles[j].pos);
                        distances[i][j] = dist;
                        distances[j][i] = dist;
                    }
                } 
            }
        }

        void calcForces(){
            double inv_norm;
            double ind_force;
            auto dist_vec = Vector2d();
            std::fill(forces, forces + nparticles, Vector2d(0, 0));
            for (int i = 0; i < nparticles; i++){
                for (int j = i; j < nparticles; j++){
                    if (i != j){
                        inv_norm = 1.0f / distances[i][j];
                        dist_vec = particles[i].pos.dist_vec(particles[j].pos);
                        ind_force = utils::ljFor(particles[i].eps, particles[i].sig, distances[i][j]);
                        dist_vec *= inv_norm;
                        dist_vec *= ind_force;
                        forces[i] += dist_vec;
                        dist_vec *= -1;
                        forces[j] += dist_vec;
                    }
                }
            }
        }

        void integrateEuler(){

            for (int i = 0; i < nparticles; i ++){
                // F/m = a
                // v = F/m * t + v0
                // s = s0 + v * t
                forces[i].y += 50000.0;
                forces[i] *= ((1 / particles[i].mass) * dt); // a*t
                particles[i].vel += forces[i]; // v
                particles[i].pos += ((forces[i] += particles[i].vel)*=dt); 
            }
        }


        ~Environment(){
            for (int i = 0; i < nparticles; i++){
                delete[] distances[i];
            }
            delete[] distances;
            delete[] particles;
            delete[] forces;
        }
};