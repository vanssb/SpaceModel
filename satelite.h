#ifndef SATELITE_H
#define SATELITE_H

#include <spaceobject.h>
#include <planet.h>
#include <qtimer.h>


class Satelite : public Planet
{
    Q_OBJECT
public:
    Satelite(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture);
    void initParams(Planet* base, QString name, float radius, float tilt, float n1, float n2, float i1, float i2, float w1, float w2, float a1, float a2, float e1, float e2, float m1, float m2, float period);
    Planet* getBase();
private:
    Planet* m_base;
};

#endif // SATELITE_H
