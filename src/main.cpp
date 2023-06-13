#include <iostream>
#include <cmath>

//typedef std::vector<float, > vector3d;

/*
class Environment {
    private:
        float height;
        float width;
        int nparticles;
        std::vector<std::vector<
};
*/

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

int main(){
    Vector3d *vec1 = new Vector3d(0, 0, 0);
    Vector3d *vec2 = new Vector3d(1, 0, 1);
    vec1->printVector();
//    vec1->add_vec(*vec2);
    vec2->printVector();
    std::cout << vec1->dist_to(*vec2) << '\n';

};