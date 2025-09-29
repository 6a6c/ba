#include <stdio.h>
#include <math.h>

/* Generates a perspective transform matrix to get things from world space into screen space */
/* Acts like this:
 *  [ w / (h * tan(t/2) )      0              0             0     ] [ x ]    [ x_c ]
 *  [        0             1 / tan(f/2)       0             0     ] [ y ]  = [ y_c ]
 *  [        0                 0         f / (f - n)  -fn/(f - n) ] [ z ]    [ z_c ]
 *  [        0                 0              1             0     ] [ 1 ]    [  z  ]
 *
 *  with w = width, h = height, t = feild of view (rad), f = far z clip, n = near z clip 
 *  to get non-z-normalized vectors in camera space.
 *  Following LH multiplication with this matrix, vectors need to perform a persepctive divide 
 *  before orthographic projection to get the x and y values in screen space.
 */
mat4* basePerspectiveMatrix(float aspect, float theta, float near, float far) {
    mat4* ret = new mat4();

    ret->matrix[0][0] = aspect * (1 / tan(theta / 2) );
    ret->matrix[1][1] = 1 / tan(theta / 2) ;
    ret->matrix[2][2] = far / (far - near);
    ret->matrix[2][3] = (-1 * far * near) / (far - near);
    ret->matrix[3][2] = 1.0;

    return ret;
}

/* Generates a camera transform matrix to move objects relative to the camera. */
mat4* cameraTransformMatrix(float pitch, float yaw, float roll, float dx, float dy, float dz){
    mat4* ret = new mat4();

    ret->matrix[0][0] = 1.0;
    ret->matrix[1][1] = 1.0;
    ret->matrix[2][2] = 1.0;
    ret->matrix[3][3] = 1.0;

    ret->matrix[0][3] = dx;
    ret->matrix[1][3] = dy;
    ret->matrix[2][3] = dz;

    return ret;
}

mat4::mat4(){
    this->matrix = new float*[4];

    for(int i = 0; i < 4; i++){
        this->matrix[i] = new float[4];
        for(int j = 0; j < 4; j++){
            this->matrix[i][j] = 0.0;
        }
    }
}

mat4::mat4(const mat4 &cpy) {
    this->matrix = new float*[4];

    for(int i = 0; i < 4; i++){
        this->matrix[i] = new float[4];
        for(int j = 0; j < 4; j++){
            this->matrix[i][j] = cpy.matrix[i][j];
        }
    }
}

mat4::~mat4(){
   for(int i = 0; i < 4; i++){
        delete[] this->matrix[i];
   }

   delete[] this->matrix;
}

/* Av */
vec4 mat4::operator*(vec4 v){
    vec4 ret = vec4();

    ret.x = (v.x * this->matrix[0][0]) + (v.y * this->matrix[0][1]) + (v.z * this->matrix[0][2]) + (v.w * this->matrix[0][3]);
    ret.y = (v.x * this->matrix[1][0]) + (v.y * this->matrix[1][1]) + (v.z * this->matrix[1][2]) + (v.w * this->matrix[1][3]);
    ret.z = (v.x * this->matrix[2][0]) + (v.y * this->matrix[2][1]) + (v.z * this->matrix[2][2]) + (v.w * this->matrix[2][3]);
    ret.w = (v.x * this->matrix[3][0]) + (v.y * this->matrix[3][1]) + (v.z * this->matrix[3][2]) + (v.w * this->matrix[3][3]);

    return ret;
}

/* AB */
/* This is actually the fastest, most efficent and best way to do graphics actually. */
mat4 mat4::operator*(mat4 right){
    mat4 ret = mat4();

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret.matrix[i][j] = (this->matrix[i][0] * right.matrix[0][j]) + (this->matrix[i][1] * right.matrix[1][j]) + (this->matrix[i][2] * right.matrix[2][j]) + (this->matrix[i][3] * right.matrix[3][j]);
        }
    }

    return ret;
}
