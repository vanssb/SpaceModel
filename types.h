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

const float auScale = 149597.870700;




#endif // TYPES_H
