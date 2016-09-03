#ifndef SATELITE_H
#define SATELITE_H

#include <spaceobject.h>
#include <planet.h>
#include <qtimer.h>


class Satelite : public SpaceObject
{
    Q_OBJECT
public:
    Satelite(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture);
    void initParams(Planet* base, QString name,float scaleFactor, float spinFactor, float ratioA, float ratioB, float speedFactor, float ecliptic);
    virtual float getOrbit();
    float getEcliptic();
    float getRotateAngle();
    Planet* getBase();
private:
    Planet* m_base;
    float m_rateSpin, m_angleSpin;
    float m_speed, m_hAngle, m_vAngle;
    float m_ecliptic;
    float m_translateA, m_translateB;
protected slots:
    void rotate();
};

#endif // SATELITE_H
