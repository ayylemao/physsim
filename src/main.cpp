#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "include/vectornd.h"
#include "include/particle.h"
#include "include/utils.h"
#include "include/environment.h"
#include <SFML/Graphics.hpp>


void drawParticles(sf::RenderWindow& window, const Environment *env, double CIRCLE_RADIUS)
{
    sf::CircleShape circle(CIRCLE_RADIUS);
    circle.setFillColor(sf::Color(255, 0, 0));
    //double forcemag;
    for (int i = 0; i < env->nparticles; i++)
    {
    //    forcemag = env->forces[i].magnitude();
    //    if (i == 0){
    //        circle.setFillColor(sf::Color(forcemag, forcemag, 255)); // Set the desired circle color
    //    }
    //    else {
    //        circle.setFillColor(sf::Color(255, forcemag, forcemag)); // Set the desired circle color
    //    }
    //    // Set the circle position
        circle.setPosition(env->particles[i].pos.x, env->particles[i].pos.y);

        // Draw the circle
        window.draw(circle);
    }
}

int main(){
    int nparticles = 1000;
    double boxsize = 500;
    double dt = 0.0005;
    double inelasticity = 1.0;
    double CIRCLE_RADIUS = 3.0;
    double VEL_SCALE = 40;

    auto env = Environment(nparticles, boxsize, dt, inelasticity);
    for (int i = 0; i < nparticles; i++){
        env.particles[i].setParams(20, 17, 10);
    }
    env.initialize_particles(VEL_SCALE);

    // set position to equally spaced
    int index = 0;
    int sidelength = std::ceil(std::sqrt(nparticles));
    double spacing = boxsize / sidelength;
    for (int i = 0; i < sidelength; i++){
        for (int j = 0; j < sidelength; j++){
            if (index < nparticles){
                env.particles[index].pos.x = (i + 0.5) * spacing;
                env.particles[index].pos.y = (j + 0.5) * spacing;
                index++;
            }
        }
    }


    sf::RenderWindow window(sf::VideoMode(boxsize, boxsize), "LJ SIM");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        env.integrateVerlet();
        env.enforcePBC();

        window.clear(); 
        drawParticles(window, &env, CIRCLE_RADIUS);
        window.display();
    }

    return 0;
}