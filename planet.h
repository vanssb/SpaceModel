#ifndef PLANET_H
#define PLANET_H

#include <spaceobject.h>
#include <QTimer>
#include <math.h>
#include <Star.h>


class Planet : public SpaceObject
{
    Q_OBJECT
public:
    Planet( QOpenGLShaderProgram *program,
            int vertexAttr,
            int textureAttr,
            int textureUniform,
            QString model,
            QString texture );
    float getEcliptic();
    float getRotateAngle();
    float getObliquity();
    void initParams( QString name,
                     float scaleFactor,
                     float spinFactor,
                     float ratioA,
                     float ratioB,
                     float speedFactor,
                     float ecliptic,
                     float obliquity);
    virtual float getOrbit();
private:
    float m_rateSpin, m_angleSpin;
    float m_speed, m_hAngle, m_vAngle;
    float m_ecliptic;
    float m_translateA, m_translateB;
    float m_obliquity;
protected slots:
    void rotate();
};

#endif // PLANET_H
