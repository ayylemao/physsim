#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <chrono>
#include "include/vectornd.h"
#include "include/particle.h"
#include "include/utils.h"
#include "include/energy.h"
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
    double duration_sum = 0;
    double frame_count = 0;
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point stop;
    int nparticles = 400;
    double boxsize = 500;
    double dt = 0.0005;
    double CIRCLE_RADIUS = 3.0;
    double VEL_SCALE = 1;
    double LJ_CUTOFF = 15;

    auto env = Environment(nparticles, boxsize, dt, LJ_CUTOFF);
    for (int i = 0; i < nparticles; i++){
        env.particles[i].setParams(2, 5, 1);
    }
    //env.initialize_particles(VEL_SCALE);
    //GradientDescent opt = GradientDescent(&env, 0.01);
    //opt.steepest_descent(0.001, 1000, 0.05);
    env.init_pos_uniform();
    



    sf::RenderWindow window(sf::VideoMode(boxsize, boxsize), "LJ SIM");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        start = std::chrono::high_resolution_clock::now();
        env.assing_cells();
        env.assignCalcArray();

        env.integrateVerlet();
        env.enforcePBC();

        window.clear(); 
        drawParticles(window, &env, CIRCLE_RADIUS);
        window.display();
        stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        duration_sum += duration.count();
        frame_count += 1.0;
        if (frame_count > 99){
            std::cout << duration_sum/frame_count << '\n';
            frame_count = 0;
            duration_sum = 0;
        }
    }

    return 0;
}