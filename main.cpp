#include <iostream>
#include <cmath>
#include "src/include/vectornd.h"
#include "src/include/particle.h"




int main(){
    Vector2d *vec1 = new Vector2d(69, 69);
    Vector2d *vec2 = new Vector2d(420, 420);
    vec1->printVector();
    vec2->printVector();
    std::cout << vec1->dist_to(*vec2) << '\n';

    LJParticle* lj1 = new LJParticle(0.2, 2.2);
    lj1->init_pos(new Vector2d(0,0));
    lj1->init_vel(new Vector2d(0,0));
    lj1->printRV();

    lj1->mod_pos(vec1);
    lj1->mod_vel(vec2);
    lj1->printRV();

};
