#include <iostream>
#include <cmath>
#include "src/include/vectornd.h"
#include "src/include/particle.h"
#include "src/include/utils.h"




class Environment {
    public:
        float width;
        int nparticles;
        LJParticle *particles;
        float **forces;

        Environment(float init_width, int init_nparticles) : 
            width(init_width), 
            nparticles(init_nparticles) 
            {
            particles = new LJParticle[nparticles];

            for (int i = 0; i < nparticles; i++) {
                // set all positions and velocities zo zero
                particles[i].init_pos(new Vector2d(0, 0));
                particles[i].init_vel(new Vector2d(0, 0));
            }

            forces = new float*[nparticles];
            for (int i = 0; i < nparticles; i++){
                forces[i] = new float[nparticles];
                for (int j = 0; j < nparticles; j++){
                    // set all force vals to zero
                    forces[i][j] = 0;
                }
            }
        }

        void init_random() {
            for (int i = 0; i < nparticles; i++){
                particles[i].mod_pos(new Vector2d(utils::randUniform(0, width), utils::randUniform(0, width)));
                particles[i].mod_vel(new Vector2d(utils::randUniform(-1, 1), utils::randUniform(-1, 1)));
            }
        }
};


int main(){
    int nparticles = 100;
    float width = 100;

    Environment env(nparticles, width);

    //env.particles[0].printRV();
    env.init_random();
    //for (int i = 0; i<100;i++){
    //    env.particles[i].printRV();
    //}
    //std::cout << env.forces[0][1];

    for (int i = 1; i<nparticles;i++){
        float dist = env.particles[0].getPos().dist_to(env.particles[i].getPos());
        float f_i = utils::ljPot(env.particles[0].sigma, env.particles[0].eps, dist);
        std::cout << f_i << '\n';

    };

    //env.particles[0].getPos().printVector(); 

};
