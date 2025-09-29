#ifndef JAKE_MATRIX
#define JAKE_MATRIX

class vec2 {
    public:
        vec2();
        vec2(float x, float y);
        
        float x;
        float y;
};

class vec3 {
    public:
        vec3();
        vec3(float x, float y, float z);

        vec3 operator+(const vec3 u);

        float x;
        float y;
        float z;
};

class vec4 {
    public:
        vec4();
        vec4(float x, float y, float z, float w);
        
        vec4 operator+(const vec4 u);
        vec4 operator-(const vec4 u);
        float operator*(const vec4 u);
        vec4 operator*(const float c);
        vec4 operator/(const float c);
    

        float x;
        float y;
        float z;
        float w;
};

class mat4 {
    public:
        mat4();
        mat4(const mat4 &cpy);
        ~mat4();

        vec4 operator*(const vec4 v);
        mat4 operator*(const mat4 right);

        float** matrix;
};

mat4* basePerspectiveMatrix(float aspect, float theta, float near, float far);
mat4* cameraTransformMatrix(float pitch, float yaw, float roll, float dx, float dy, float dz);
#endif
