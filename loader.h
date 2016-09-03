#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <types.h>

class Loader
{
public:
    Loader();
    static void loadModel(const char* fileName, std::vector<float> *vertices, std::vector<float> *textures, std::vector<float> *normals);
};

#endif // LOADER_H
