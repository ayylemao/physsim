#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <random>
#include <unordered_map>
#include <vector>
#include "vectornd.h"
#include "particle.h"
#include "utils.h"

using Cell = std::vector<int>;

class Environment {
    public:
        int nparticles;
        double boxsize;
        double dt;
        double inela = 1.0;
        double **distances;
        double energy = 0;
        bool pbc = true;
        double cell_size;
        int ncells;
        bool **nneighbour;
        std::unordered_map<int, Cell> spatial_grid;
        Particle* particles;
        Vector2d* forces;
        Vector2d* forces_old;


        Environment(
            int numParticles,
            float in_boxsize,
            float timestep,
            float lj_cutoff) 
        : 
            nparticles(numParticles), boxsize(in_boxsize), dt(timestep), cell_size(lj_cutoff)
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

            // cell division scheme
            ncells = boxsize / cell_size;
            nneighbour = new bool*[nparticles];
            for (int i = 0; i < nparticles; i++){
                nneighbour[i] = new bool[nparticles];
            }
            for (int i = 0; i < nparticles; i++) {
                for (int j = 0; j < nparticles; j++) {
                    nneighbour[i][j] = false;
                }
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

        void init_pos_uniform() {
            int index = 0;
            int sidelength = std::ceil(std::sqrt(nparticles));
            double spacing = boxsize / sidelength;
            for (int i = 0; i < sidelength; i++){
                for (int j = 0; j < sidelength; j++){
                    if (index < nparticles){
                        particles[index].pos.x = (i + 0.5) * spacing;
                        particles[index].pos.y = (j + 0.5) * spacing;
                        index++;
                    }
                }
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

        void enforcePBC(){
            for (int i = 0; i < nparticles; i++){
                if (particles[i].pos.x < boxsize) {
                    particles[i].pos.x = particles[i].pos.x + boxsize;
                }
                else if (particles[i].pos.x > boxsize) {
                    particles[i].pos.x = particles[i].pos.x - boxsize;
                }
                if (particles[i].pos.y > boxsize) {
                    particles[i].pos.y = particles[i].pos.y - boxsize;
                }
                else if (particles[i].pos.y < boxsize) {
                    particles[i].pos.y = particles[i].pos.y + boxsize;
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

        Vector2d calc_PBC_dist(const Vector2d& pos1, const Vector2d& pos2){
            double dist_x = pos2.x - pos1.x;
            double dist_y = pos2.y - pos1.y;

            if (dist_x > boxsize * 0.5) {
                dist_x -= boxsize;
            } else if (dist_x < -boxsize * 0.5) {
                dist_x += boxsize;
            }

            if (dist_y < -boxsize * 0.5) {
                dist_y += boxsize;
            } else if (dist_y > boxsize * 0.5) {
                dist_y -= boxsize;
            }
            return Vector2d(dist_x, dist_y);
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
                        if (nneighbour[i][j] == true){
                            dist_vec = calc_PBC_dist(particles[i].pos, particles[j].pos);
                            dist = dist_vec.magnitude();
                            inv_norm = 1.0f / dist;
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

        double calcPotEnergy(){
            double potenergy = 0;
            double dist = 0;
            for (int i = 0; i < nparticles; i++){
                for (int j = i; j < nparticles; j++){
                    if (i != j){
                        dist = calc_PBC_dist(particles[i].pos, particles[j].pos).magnitude();
                        potenergy += 2*utils::ljPot(particles[i].eps, particles[i].sig, dist);
                    }
                }
            }
            return potenergy;
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

        int calculateCellIndex(double x, double y) {
            int cell_x = static_cast<int>(x / cell_size);
            int cell_y = static_cast<int>(y / cell_size);
            return cell_x + cell_y * ncells;
        }

        void assing_cells(){
            for (int i = 0; i<nparticles; i++){
                int cell_index = calculateCellIndex(particles[i].pos.x, particles[i].pos.y);
                spatial_grid[cell_index].push_back(i);
            }
        }

        std::tuple<int, int> index_to_RowCol(int index){
            int row = index / ncells;
            int col = index % ncells;
            return std::make_tuple(row, col);
        }

        int rowCol_to_index(int row, int col){
            if (row < 0) {
                row += ncells;
            }
            else if (row >= ncells){
                row -= ncells;
            }
            if (col < 0){
                col += ncells;
            }
            else if (row >= ncells) {
                col -= ncells;
            } 
            return row * ncells + col;
        }

        void set_nneighbour_zero(){
            for (int i = 0; i<nparticles; i++){
                    std::fill(nneighbour[i], nneighbour[i] + nparticles, false);
                }
        }

        void assignCalcArray(){
            int row;
            int col;
            int jcell;
            set_nneighbour_zero();
            for (int icell = 0; icell < ncells*ncells; icell++){
                int offsets[9][2] = {
                    {-1, -1},
                    {-1, 0},
                    {-1, 1},
                    {0, -1},
                    {0, 0},
                    {0, 1},
                    {1, -1},
                    {1, 0},
                    {1, 1}
                };

                // within cell
                for (int i = 0; i<9; i++){
                    std::tie(row, col) = index_to_RowCol(icell);
                    //std::cout <<row<<' '<<col<<'\n';;
                    row += offsets[i][0];
                    col += offsets[i][1];
                    //std::cout <<row<<' '<<col<<'\n';;
                    jcell = rowCol_to_index(row, col);
                    //std::cout << jcell << '\n';
                    for (int particle_i : spatial_grid[icell]){
                        for (int particle_j : spatial_grid[jcell]){
                            //std::cout << "Particle_i: " << particle_i << "   Particle_j: " << particle_j << '\n';
                            nneighbour[particle_i][particle_j] = true;
                        }
                    }
                }
            }
            spatial_grid.clear();
        }
        

        ~Environment(){
            for (int i = 0; i < nparticles; i++){
                delete[] distances[i];
            }
            for (int i = 0; i< ncells; i++){
                delete[] nneighbour[i];
            }
            delete[] nneighbour;
            delete[] distances;
            delete[] particles;
            delete[] forces;
        }

};
#endif