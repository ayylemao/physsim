#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "src/include/vectornd.h"
#include "src/include/particle.h"
#include "src/include/utils.h"
#include <SFML/Graphics.hpp>




class Environment {
    protected:
        float dt;
    public:
        float width;
        int nparticles;
        LJParticle *particles;
        Vector2d *forces;

        Environment(float init_width, int init_nparticles, float init_dt) : 
            width(init_width), 
            nparticles(init_nparticles),
            dt(init_dt)
            {
            particles = new LJParticle[nparticles];
            forces = new Vector2d[nparticles];

            for (int i = 0; i < nparticles; i++) {
                // set all positions and velocities zo zero
                particles[i].init_pos(new Vector2d(0, 0));
                particles[i].init_vel(new Vector2d(0, 0));
            }
        }


        void init_random() {
            for (int i = 0; i < nparticles; i++){
                particles[i].mod_pos(new Vector2d(utils::randUniform(0, width), utils::randUniform(0, width)));
                particles[i].mod_vel(new Vector2d(utils::randUniform(-1, 1), utils::randUniform(-1, 1)));
            }
        }

        void calculateForces(){
            std::memset(forces, 0.0, sizeof(Vector2d) * nparticles);
            for (int i = 0; i < nparticles; i++){
                for (int j = 0; j < nparticles; j++){
                    if (i != j){
                        float distij = particles[i].getPos().dist_to(particles[j].getPos());
                        float f_ij = utils::ljFor(particles[i].sigma, particles[i].eps, distij);
                        Vector2d unitVec = particles[i].getPos().getUnitDistAx(particles[j].getPos());
                        unitVec.scalarMult(f_ij);
                        forces[i].add_vec(unitVec);
                    }
                }
            }
        }

        void IntegrateEuler(){
            // F = m * a -> F/m = a
            // v(t) = F(t)/m * t + v(t-1)
            Vector2d a_i(0,0);
            Vector2d v_i(0,0);
            Vector2d s_i(0,0);
            float m;
            for (int i = 0; i < nparticles; i++){
                a_i = forces[i];
                m = particles[i].getMass();
                a_i.scalarMult(1.0/m);
                a_i.scalarMult(dt);
                v_i = particles[i].getVel();
                v_i.add_vec(a_i);
                particles[i].mod_vel(&v_i);

                s_i = particles[i].getPos();
                v_i.scalarMult(dt);
                s_i.add_vec(v_i);
                particles[i].mod_pos(&s_i);
                
            }

        }
};


int main(){
    int nparticles = 1000;
    float width = 100;
    float dt = 0.01;

    Environment env(width, nparticles, dt);
    env.init_random();

    for (int i = 0; i<1000; i++){
        env.IntegrateEuler(); 
        env.particles[0].printRV();
    } 

};
