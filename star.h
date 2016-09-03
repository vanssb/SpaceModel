#ifndef STAR_H
#define STAR_H

#include <spaceobject.h>


class Star : public SpaceObject
{
public:
    Star(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture);
    void initParams(QString name, float scale);
    virtual float getOrbit();
};

#endif // STAR_H
