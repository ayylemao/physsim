#ifndef VECTORND_H
#define VECTORND_H

#include <iostream>
#include <cmath>

class Vector2d {
    public:
        double x;
        double y;

        Vector2d(double init_x = 0, double init_y = 0){
            x = init_x;
            y = init_y;
        };

        void print() {
            std::cout << '[' << x << ", " << y << "]\n";
        }

        Vector2d& operator +=(const Vector2d& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2d& operator -=(const Vector2d& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2d& operator *=(const float mult) {
            x = x*mult;
            y = y*mult; 
            return *this;
        }

        Vector2d dist_vec(const Vector2d& other){
            double x_dist = other.x - x;
            double y_dist = other.y - y;
            return Vector2d(x_dist, y_dist);
        }


        double dist(const Vector2d& other){
            double x_dist = x - other.x;
            double y_dist = y - other.y;
            return std::sqrt(x_dist*x_dist + y_dist*y_dist);
        }


        void getUnitAxis(const Vector2d& other, Vector2d& result){
            float x_dist = other.x - x;
            float y_dist = other.y - y;
            float norm_squared = x_dist*x_dist + y_dist*y_dist;
            float inv_sqrt = 1.0f / std::sqrt(norm_squared);
            result.x = x_dist * inv_sqrt;
            result.y = y_dist * inv_sqrt;
        }
};

#endif