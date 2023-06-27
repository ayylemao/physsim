#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "include/vectornd.h"
#include "include/particle.h"
#include "include/utils.h"
#include "include/environment.h"
#include <SFML/Graphics.hpp>


void drawParticles(sf::RenderWindow& window, Environment *env, double CIRCLE_RADIUS)
{
    sf::CircleShape circle(CIRCLE_RADIUS);
    double forcemag;
    for (int i = 0; i < env->nparticles; i++)
    {
        forcemag = env->forces[i].magnitude();
        circle.setFillColor(sf::Color(255, forcemag, forcemag)); // Set the desired circle color
        // Set the circle position
        circle.setPosition(env->particles[i].pos.x, env->particles[i].pos.y);

        // Draw the circle
        window.draw(circle);
    }
}

int main(){
    int nparticles = 100;
    double boxsize = 500;
    double dt = 0.0001;
    double inelasticity = 1.0;
    double CIRCLE_RADIUS = 3.0;
    double VEL_SCALE = 10;

    auto env = Environment(nparticles, boxsize, dt, inelasticity);
    for (int i = 0; i < nparticles; i++){
        env.particles[i].setParams(1, 20, 10);
    }
    env.initialize_particles(VEL_SCALE);
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
        //env.calcEnergy();
        env.enforceBoundary();
        //std::cout << env.energy << '\n';
        window.clear(); 
        drawParticles(window, &env, CIRCLE_RADIUS);

        window.display();
    }

    return 0;
}