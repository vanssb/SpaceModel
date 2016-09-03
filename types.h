#ifndef TYPES_H
#define TYPES_H

struct cordinate{
    float x;
    float y;
    float z;
};

struct char2D{
    int id;
    float uv_x;
    float uv_y;
    float uv_width;
    float uv_height;
    int width;
    int height;
};

const float astroUnit = 12000.0f;
const float scaleUnit = 20.0f;




#endif // TYPES_H
