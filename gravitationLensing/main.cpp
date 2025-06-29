//
//  main.cpp
//  gravitationLensing
//
//  Created by Александр Георгиев on 29.06.2025.
//

#include <iostream>


struct vec3 {
    public:
        float x;
        float y;
        float z;
    vec3();
    vec3(const float ix, const float iy, const float iz){
        x = ix;
        y = iy;
        z = iz;
    }
    float length(){
        return sqrt(x*x + y*y + z*z);
    }
    vec3 operator +(const vec3& v){
        return vec3(x+v.x, y+v.y, z+ v.z);
    }
    vec3 operator -(const vec3& v){
        return vec3(x-v.x, y-v.y, z-v.z);
    }
    vec3 operator *(const float w){
        return vec3(x*w, y*w, z*w);
    }
    vec3 operator /(const float w){
        return vec3(x/w, y/w, z/w);
    }
    vec3 normalize(){
        float l = this->length();
        return (*this)/l;
    }
};
struct vec2 {
    public:
        float x;
        float y;
    vec2();
    vec2(const float ix, const float iy){
        x = ix;
        y = iy;
    }
    float length(){
        return sqrt(x*x + y*y);
    }
    vec2 operator +(const vec2& v){
        return vec2(x+v.x, y+v.y);
    }
    vec2 operator -(const vec2& v){
        return vec2(x-v.x, y-v.y);
    }
    vec2 operator *(const float w){
        return vec2(x*w, y*w);
    }
    vec2 operator /(const float w){
        return vec2(x/w, y/w);
    }
    vec2 normalize(){
        float l = this->length();
        return (*this)/l;
    }
};


class ray{
    vec3 startPosition;
    vec3 startDirection;
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
 
