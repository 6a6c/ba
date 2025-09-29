#include <algorithm>

object::object() {
    this->triangles = std::vector<triangle *>();
}

object::~object() 
{
    this->clear();    
}

/* Reads an object from a .tri file */
void object::readFromFile(char* fn) {
    this->clear();

    FILE* fp;
    int size;
    float x, y, z;
    triangle* tri;

    fp = fopen(fn, "r");

    fscanf(fp, "%d\n", &size);
   
    printf("%s is got %d triangles\n", fn, size);

    this->triangles.resize(size);

    for(int i = 0; i < size; i++) {
        tri = new triangle();

        tri->id = i;

        fscanf(fp, "%f %f %f\n", &(tri->v0.x), &(tri->v0.y), &(tri->v0.z));
        fscanf(fp, "%f %f %f\n", &(tri->v1.x), &(tri->v1.y), &(tri->v1.z));
        fscanf(fp, "%f %f %f\n", &(tri->v2.x), &(tri->v2.y), &(tri->v2.z));
        fscanf(fp, "\n");

        this->triangles[i] = tri;
    }

    printf("Finished reading %s\n", fn);
}

/* Pushes triangle * to vector */
void object::addTriangle(triangle* toAdd) {
    this->triangles.push_back(toAdd);
}

/* Compares triangles based on distance */
bool triangleCompare(triangle* a, triangle* b) {
    return a->nearest > b->nearest;
} 

/* Projects each triangle in object in screen space, and sorts based on distance */
void object::project(mat4* transform) {
    for(triangle *tri : this->triangles) {
        tri->project(transform);
    }

    sort(this->triangles.begin(), this->triangles.end(), triangleCompare);
}

/* Rasterizes each triangle within object after projection */
void object::rasterize(const int width, const int height, int* buffer){
    for(triangle *tri : this->triangles){ 
        tri->rasterize(width, height, buffer);
    }
}

/* Clears all associated memory of object */
void object::clear() {
    for(triangle* tri : this->triangles){
        delete tri;
    }

    this->triangles.clear();
}
