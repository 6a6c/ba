#ifndef JAKE_GEOMETRY
#define JAKE_GEOMETRY

#include <matrix.hpp>
#include <vector>

class triangle {
    public:
        triangle();
        triangle(vec4 v0, vec4 v1, vec4 v2);

        int id;

        void rasterize(const int width, const int height, int* buffer);

        vec4 v0;
        vec4 v1;
        vec4 v2;
    
        vec4 projV0;
        vec4 projV1;
        vec4 projV2;

        float nearest;
        
        void project(mat4* transform);

    private:
        vec4* projectedPts;

        void orderProjectedPts();
};

class object {
    public:
        object();
        ~object();

        void readFromFile(char* fn);

        void addTriangle(triangle* toAdd);

        void project(mat4* transform);
        void rasterize(const int width, const int height, int* buffer);

    private:
        void clear();

        std::vector<triangle*> triangles; 
};

#endif
