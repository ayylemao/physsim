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
    circle.setFillColor(sf::Color::Red); // Set the desired circle color

    for (int i = 0; i < env->nparticles; i++)
    {

        // Set the circle position
        circle.setPosition(env->particles[i].pos.x, env->particles[i].pos.y);

        // Draw the circle
        window.draw(circle);
    }
}

int main(){
    int nparticles = 484;
    double boxsize = 500;
    double dt = 0.001;
    double inelasticity = 0.1;
    double CIRCLE_RADIUS = 3.0;

    auto env = Environment(nparticles, boxsize, dt, inelasticity);
    for (int i = 0; i < nparticles; i++){
        env.particles[i].setParams(1, 3, 100);
    }
    env.initialize_particles();
    sf::RenderWindow window(sf::VideoMode(boxsize, boxsize), "LJ SIM");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        env.calcDistances();
        env.calcForces();
        env.integrateEuler();
        env.enforceBoundary();

        window.clear(); 
        drawParticles(window, &env, CIRCLE_RADIUS);

        window.display();
    }

    return 0;
}