#ifndef VECTORND_H
#define VECTORND_H

#include <iostream>
#include <cmath>

class Vector2d {
    public:
        float x;
        float y;
        float* comp[2];

        Vector2d(float init_x, float init_y) {
            x = init_x;
            y = init_y;
            comp[0] = &x;
            comp[1] = &y;
        }

        Vector2d() : x(0), y(0){
            //pass
        }

        void add_vec(Vector2d vec2){
            x += vec2.x;
            y += vec2.y;
        }

        float dist_to(Vector2d vec2){
            float dist = 0;
            for (int i = 0; i < 2; i++) {
                dist += std::pow(*(comp[i]) - *(vec2.comp[i]), 2);
            }
            dist = std::sqrt(dist);
            return dist;
        }

        void scalarMult(float multi){
            x = x*multi;
            y = y*multi;
        }

        void printVector(){
            std::cout << '[';
            std::cout << x << ", " << y;
            std::cout << ']';
            std::cout << '\n'; 
        }

        static float getNorm(Vector2d vec) {
            float norm = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
            return norm;
        }

        Vector2d getUnitDistAx(Vector2d vec2) {
            Vector2d tempvec(0, 0);
            tempvec.x = vec2.x - x;
            tempvec.y = vec2.y - y;
            float norm = getNorm(tempvec);
            tempvec.x = tempvec.x / norm;
            tempvec.y = tempvec.y / norm;
            return tempvec;
        }
};

class Vector3d {
    public:
        float x;
        float y;
        float z;
        float* comp[3];

        Vector3d(float init_x, float init_y, float init_z) {
            x = init_x;
            y = init_y;
            z = init_z;
            comp[0] = &x;
            comp[1] = &y;
            comp[2] = &z;
        }

        void add_vec(Vector3d vec2){
            for (int i = 0; i < 3; i++) {
                *(comp[i]) += *(vec2.comp[i]);
            }
        }

        float dist_to(Vector3d vec2){
            float dist = 0;
            for (int i = 0; i < 3; i++) {
                dist += std::pow(*(comp[i]) - *(vec2.comp[i]), 2);
            }
            dist = std::sqrt(dist);
            return dist;
        }

        void printVector(){
            std::cout << '[';
            std::cout << x << ", " << y << ", " << z;
            std::cout << ']';
            std::cout << '\n'; 
        }
};

#endif