//
//  main.cpp
//  gravitationLensing
//
//  Created by Александр Георгиев on 29.06.2025.
//

#include <iostream>
#include <fstream>
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;



struct vec3 {
    public:
        float x;
        float y;
        float z;
    vec3(){};
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
    void operator +=(const vec3& v){
        x+=v.x; y+=v.y; z+= v.z;
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
    float scalarMultiply(vec3 v){
        return x*v.x + y*v.y + z*v.z;
    }
    void print(){
        std::cout<< "x  "<< x << " y "<< y <<" z "<< z << endl;
    }
};
struct vec2 {
    public:
        float x;
        float y;
    vec2(){};
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
    void print(){
        std::cout<< "x = " << x <<" y = "<< y << endl;
    }
};


struct Ray{
    vec3 startPosition;
    vec3 startDirection;
    Ray(vec3 sPos, vec3 sDir){
        startPosition = sPos;
        startDirection = sDir;
    }
    Ray();
};

float massInside(float r){
    return 2e5;
}

float zPlaneDist(const float z, vec3& pos){
    return z-pos.z;
}

float minf(const float& a, const float& b){
    if (a<b){
        return a;
    }
    else{
        return b;
    }
}


vec2 rayMarch(const Ray& ray, vec3& massCoord){
    vec3 pos = ray.startPosition;
    vec3 vel = ray.startDirection;
    const float step = 0.1f;
    float dT = step;
    for (int i = 0; i < 200; i++){
        float zpd = zPlaneDist(2, pos);
        if (zpd < 1.5*step){
            break;
        }
        dT = minf(zpd, step);
        float r = (pos-massCoord).length();
        vel += (massCoord-pos)*(dT * massInside(r)/(r*r*r));
        //((massCoord-pos)*(dT * massInside(r)/(r*r*r))).print();
        pos += vel*dT;
    }
    return vec2(pos.x, pos.y);
}

struct RGB {
    uint8_t r, g, b;
    void operator+=(RGB rgb){
        r+=r; g+=g; b+=b;
    }
    void print(){
        std::cout<< "r " << int(r) << " g " << int(g) << " b " << int(b) << endl;
    }
};

void save_png(const std::vector<std::vector<RGB>>& pixels, const char* filename) {
    if (pixels.empty()) {
        std::cerr << "Error: Image has no rows\n";
        return;
    }

    const int width = pixels[0].size();
    const int height = pixels.size();
    const int channels = 3;

    for (const auto& row : pixels) {
        if (row.size() != static_cast<size_t>(width)) {
            std::cerr << "Error: Inconsistent row sizes\n";
            return;
        }
    }

    std::vector<uint8_t> buffer(width * height * channels);

    for (int y = 0; y < height; ++y) {
        const RGB* row_ptr = pixels[y].data();
        uint8_t* dest = buffer.data() + y * width * channels;
        memcpy(dest, row_ptr, width * sizeof(RGB));
    }

    if (!stbi_write_png(filename, width, height, channels, buffer.data(), width * channels)) {
        std::cerr << "Error: Failed to write PNG file\n";
    }
}

std::vector<std::vector<RGB>> load_png(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb);

    if (!data) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return {};
    }

    std::vector<std::vector<RGB>> pixels(height, std::vector<RGB>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int src_idx = (y * width + x) * 3;
            pixels[y][x] = {
                data[src_idx],
                data[src_idx + 1],
                data[src_idx + 2]
            };
        }
    }

    stbi_image_free(data);
    return pixels;
}

void fixImage(const int xGravCentre, const int yGravCentre, const char* inImagePatch, const char* outImagePatch){
    std::vector<std::vector<RGB>> inImage = load_png(inImagePatch);
    const int width = inImage[0].size();
    const int height = inImage.size();
    std::vector<std::vector<RGB>> outImage = load_png(inImagePatch);
    const float scale = 1;
    vec3 gravCentre(float(xGravCentre)*scale, float(yGravCentre)*scale, 1);
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            outImage[y][x] = {0, 0, 0};
            vec3 dir(0,0,1);
            Ray pixRay(vec3(float(x)*scale, float(y)*scale , 0), dir);
            vec2 outPixCoord = rayMarch(pixRay, gravCentre);
            int outX = round(outPixCoord.x / scale);
            int outY = round(outPixCoord.y / scale);
            if(outX < 0){
                outX = 0;
            }
            if(outX >= width){
                outX = width-1;
            }
            if(outY < 0){
                outY = 0;
            }
            if(outY >= height){
                outY = height-1;
            }
            outImage[outY][outX] = inImage[y][x];
        }
    }
    save_png(outImage, outImagePatch);
    
}

int main(int argc, const char * argv[]) {
    fixImage(464, 363, "/Users/aleksandrgeorgiev/Desktop/dev/gravitationLensing/gravitationLensing/input.png", "/Users/aleksandrgeorgiev/Desktop/dev/gravitationLensing/gravitationLensing/output.png");
    return 0;
}
 
