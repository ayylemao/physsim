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

        void add_vec(Vector2d vec2){
            for (int i = 0; i < 2; i++) {
                *(comp[i]) += *(vec2.comp[i]);
            }
        }

        float dist_to(Vector2d vec2){
            float dist = 0;
            for (int i = 0; i < 2; i++) {
                dist += std::pow(*(comp[i]) - *(vec2.comp[i]), 2);
            }
            dist = std::sqrt(dist);
            return dist;
        }

        void printVector(){
            std::cout << '[';
            std::cout << x << ", " << y;
            std::cout << ']';
            std::cout << '\n'; 
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
