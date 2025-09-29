#include <stdio.h>
#include <math.h>

/* Blank constructor */
triangle::triangle() {
    this->v0 = vec4();
    this->v1 = vec4();
    this->v2 = vec4();

    this->projectedPts = new vec4[3];

    this->nearest = 1;
}

/* Constructor taking three vectors as the vertices */
triangle::triangle(vec4 v0, vec4 v1, vec4 v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    
    this->projectedPts = new vec4[3];

    this->nearest = 1;
}

/* Projects the triangle into screen space. */
/*  mat4* transform is the result of all needed transform (world -> camera -> screen)
 * ordersProjected pts after multiplcation and perspective divide */
void triangle::project(mat4* transform){
    /* Av with A as our transform matrix takes vectors in screen space */
    /* with their w component representing their original z component */
    this->projectedPts[0] = (*transform) * this->v0;
    this->projectedPts[1] = (*transform) * this->v1;
    this->projectedPts[2] = (*transform) * this->v2;
    
    /* Perspective divide of vector with stored z component */
    float w = 0.0;
    for(int i = 0; i < 3; i++) {
        w = this->projectedPts[i].w;

        if(w != 0) {
            this->projectedPts[i].x /= w; 
            this->projectedPts[i].y /= w;
            this->projectedPts[i].z /= w;    
            this->projectedPts[i].w /= w;

            /* Calculates nearest vertex to camera, used to rasterize triangles in order */
            if(this->projectedPts[i].z < this->nearest) this->nearest = this->projectedPts[i].z;
        }
    }

    this->orderProjectedPts();
}

/* Orders the three projected pts of the triangle based on y value 
 * the top most projected pt will be projV0, then projV1, then projV2.
 * This aids in rasterization of the triangle so we can split it into a top half and a bottom half */
void triangle::orderProjectedPts(){

    if(this->projectedPts[0].y >= this->projectedPts[1].y && this->projectedPts[0].y >= this->projectedPts[2].y) {
        this->projV0 = this->projectedPts[0];
        
        if(this->projectedPts[1].y >= this->projectedPts[2].y){
            this->projV1 = this->projectedPts[1];
            this->projV2 = this->projectedPts[2];
        } else {
            this->projV1 = this->projectedPts[2];
            this->projV2 = this->projectedPts[1];
        }

    } else if(this->projectedPts[1].y >= this->projectedPts[0].y && this->projectedPts[1].y >= this->projectedPts[2].y) {
        this->projV0 = this->projectedPts[1];
        
        if(this->projectedPts[0].y >= this->projectedPts[2].y){
            this->projV1 = this->projectedPts[0];
            this->projV2 = this->projectedPts[2];
        } else {
            this->projV1 = this->projectedPts[2];
            this->projV2 = this->projectedPts[0];
        }
        
    } else {
        this->projV0 = this->projectedPts[2];
        
        if(this->projectedPts[0].y >= this->projectedPts[1].y){
            this->projV1 = this->projectedPts[0];
            this->projV2 = this->projectedPts[1];
        } else {
            this->projV1 = this->projectedPts[1];
            this->projV2 = this->projectedPts[0];
        }
    }
}

/* Helper function to take left-right within frame buffer to float x within [-1, 1] */
float screenToX(const int i, const int width) {
    return 2 * (float) i / width - 1;
}

/* Helper function to take float x within [-1, 1] to left-right in frame buffer */
int xToScreen(const float x, const int width) {
    return (int) ((x + 1) * width / 2);
}

/* Helper function to take up-down within frame buffer to float y within [-1, 1] */
float screenToY(const int j, const int height) {
    return -1 * ((2 * ((float) j / height)) - 1);
}

/* Helper function to take float y within [-1, 1] to up-down in frame buffer */
int yToScreen(const float y, const int height) {
    return (int) (height - ((y + 1) * height / 2));
}

/* Calulates the intercept of a side of the triangle with current horizontal scanline */
int intercept(const vec4 u, const vec4 v, int j, int width, int height) {
    /* Scanline we're intercepting */
    float y = screenToY(j, height);    

    /* m = (u.y - y) / (u.x - x) */
    /* x = -1 ((v0.y - y) / m ) + x0.x) */    

    /* For small differences in y, dont even try or we get wacky lines */
    if(u.y - v.y <= 0.005) {
        return (u.x < v.x) ? xToScreen(u.x, width) : xToScreen(v.x, width);
    }

    /* calculate x value of intercept and return as left-right screen coord */
    float x = -1 * ( ( (u.x - v.x) * (u.y - y) / (u.y - v.y) ) - u.x );
    return xToScreen(x, width);
}

/* Calculates z component of triangle at current scanline */
float depth(const vec4 u, const vec4 v, int j, int width, int height) {
    float y = screenToY(j, height);    

    if(u.y - v.y <= 0.005) {
        return (u.z < v.z) ? u.z : v.z;
    }

    return -1 * ( ( (u.z - v.z) * (u.y - y) / (u.y - v.y) ) - u.z );
}

void triangle::rasterize(const int width, const int height, int* buffer){ 
    int left, right, swap;
    float leftDepth, rightDepth, swapDepth; 

    /* Top half */
    /*    /\ v0
     *   /##\
     *  /####\
     *v1\     \
     *    \    \
     *      \   \
     *        \  \
     *          \ \
     *            \\ v2
     */
    for(int j = yToScreen(this->projV0.y, height); j < yToScreen(this->projV1.y, height) && j < height; j++) {
        if(j < 0) {
            j = 0;
            continue;
        }

        /* Get left/right side */
        left = intercept(this->projV0, this->projV1, j, width, height);
        right = intercept(this->projV0, this->projV2, j, width, height);

        leftDepth = depth(this->projV0, this->projV1, j, width, height);
        rightDepth = depth(this->projV0, this->projV2, j, width, height);

        /* Swap if needed */
        if(left > right) {
            swap = left;
            left = right;
            right = swap;

            swapDepth = leftDepth;
            leftDepth = rightDepth;
            rightDepth = swapDepth;
        }

        /* Clip */
        if(left < 0) continue;
        if(right >= width) continue;

        leftDepth = (-1 * leftDepth + 1.0) / 2.0;
        rightDepth = (-1 * rightDepth + 1.0) / 2.0;

        for(int i = left; i <= right; i++) {
            float p;

            if(right != left) {
                p = (i - left) / (right - left);
            } else {
                p = 1;
            }

            int shade = (p * (rightDepth - leftDepth) + leftDepth) * 100 + 155;

            buffer[i + (j * width)] = (255 << 24) | (shade  << 16) | (shade << 8) | shade;
        }
    }

    /* Bottom half */
    /*    /\ v0
     *   /  \
     *  /    \
     *v1\#####\
     *    \####\
     *      \###\
     *        \##\
     *          \#\
     *            \\ v2
     */
    for(int j = yToScreen(this->projV1.y, height); j <= yToScreen(this->projV2.y, height) && j < height; j++) {
        if(j < 0) {
            j = 0;
            continue;
        }

        int left, right, swap;    
        float leftDepth, rightDepth, swapDepth;

        left = intercept(this->projV1, this->projV2, j, width, height);
        right = intercept(this->projV0, this->projV2, j, width, height);

        leftDepth = depth(this->projV1, this->projV2, j, width, height);
        rightDepth = depth(this->projV0, this->projV2, j, width, height);
    
        if(left < 0) left = 0;
        if(right >= width) right = width - 1;

        if(left > right) {
            swap = left;
            left = right;
            right = swap;

            swapDepth = leftDepth;
            leftDepth = rightDepth;
            rightDepth = swapDepth;
        }

        if(left < 0) continue;
        if(right >= width) continue;

        leftDepth = (-1 * leftDepth + 1.0) / 2.0;
        rightDepth = (-1 * rightDepth + 1.0) / 2.0;

        for(int i = left; i <= right; i++) {
            float p;

            if(right != left) {
                p = (i - left) / (right - left);
            } else {
                p = 1;
            }

            int shade = (p * (rightDepth - leftDepth) + leftDepth) * 100 + 155;

            buffer[i + (j * width)] = (255 << 24) | (shade  << 16) | (shade << 8) | shade;
        }
    }
}
