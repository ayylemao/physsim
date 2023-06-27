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
        double **distances;
        double energy = 0;
        Particle* particles;
        Vector2d* forces;
        Vector2d* forces_old;


        Environment(int numParticles, float in_boxsize, float timestep, float inelasticity) 
        : nparticles(numParticles), boxsize(in_boxsize), dt(timestep), inela(inelasticity)
        {
            particles = new Particle[numParticles];
            distances = new double*[nparticles];
            forces = new Vector2d[nparticles];
            forces_old = new Vector2d[nparticles];
            std::fill(forces, forces + nparticles, Vector2d(0, 0));
            std::fill(forces_old, forces_old + nparticles, Vector2d(0, 0));

            for (int i = 0; i < nparticles; i++) {
                distances[i] = new double[nparticles];
                std::fill(distances[i], distances[i] + nparticles, 0.0);
            }
        }

        void initialize_particles(double vel_scale){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<double> normal(0.0, 10.0);
            std::uniform_real_distribution<double> uniform(0.0, boxsize);
            double vx;
            double vy;
            double x;
            double y;
            for (int i = 0; i < nparticles; i++){
                vx = normal(gen)*vel_scale;
                vy = normal(gen)*vel_scale;
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
            double dist;
            std::fill(forces, forces + nparticles, Vector2d(0, 0));
            for (int i = 0; i < nparticles; i++){
                for (int j = i; j < nparticles; j++){
                    if (i != j){
                        dist = particles[i].pos.dist(particles[j].pos);
                        inv_norm = 1.0f / dist;
                        dist_vec = particles[i].pos.dist_vec(particles[j].pos);
                        ind_force = utils::ljFor(particles[i].eps, particles[i].sig, dist);
                        dist_vec *= inv_norm;
                        dist_vec *= ind_force;
                        forces[i] += dist_vec;
                        dist_vec *= -1;
                        forces[j] += dist_vec;
                    }
                }
            }
        }        

        void calcEnergy(){
            energy = 0;
            for (int i = 0; i < nparticles; i++){
                energy += 0.5 * particles[i].mass * (particles[i].vel.x*particles[i].vel.x + particles[i].vel.x*particles[i].vel.x);
                for (int j = i; j < nparticles; j++){
                    if (i != j){
                        energy += 2*utils::ljPot(particles[i].eps, particles[i].sig, distances[i][j]);
                    }
                }
            }
        }


        void integrateVerlet(){
            auto accel0 = Vector2d(0, 0);
            auto accel1 = Vector2d(0, 0);
            calcForces();
            for (int i = 0; i<nparticles; i++){
                accel0.x = forces[i].x / particles[i].mass;
                accel0.y = forces[i].y / particles[i].mass;

                particles[i].pos.x = particles[i].pos.x + particles[i].vel.x * dt + 0.5 * accel0.x * dt * dt;
                particles[i].pos.y = particles[i].pos.y + particles[i].vel.y * dt + 0.5 * accel0.y * dt * dt;
                forces_old[i] = forces[i];
            }
            calcForces();
            for (int i = 0; i<nparticles; i++){
                accel0.x = forces_old[i].x / particles[i].mass;
                accel0.y = forces_old[i].y / particles[i].mass;
                accel1.x = forces[i].x / particles[i].mass;
                accel1.y = forces[i].y / particles[i].mass;

                particles[i].vel.x = particles[i].vel.x + 0.5*(accel0.x + accel1.x) * dt;
                particles[i].vel.y = particles[i].vel.y + 0.5*(accel0.y + accel1.y) * dt;
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