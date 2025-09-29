vec4::vec4(){
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 1.0;
}

vec4::vec4(float x, float y, float z, float w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

vec4 vec4::operator+(const vec4 u) {
    return vec4(this->x + u.x, this->y + u.y, this->z + u.z, this->w + u.w);
}

vec4 vec4::operator-(const vec4 u) {
    return vec4(this->x - u.x, this->y - u.y, this->z - u.z, this->w - u.w);
}

float vec4::operator*(const vec4 u) {
    return this->x * u.x + this->y * u.y + this->z * u.z + this->w * u.w;
}

vec4 vec4::operator*(const float c) {
    return vec4(this->x * c, this->y * c, this->z * c, this->w * c);
}

vec4 vec4::operator/(const float c) {
    return vec4(this->x / c, this->y / c, this->z / c, this->w / c);
}

