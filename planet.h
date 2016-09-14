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
    float getRotateX();
    float getRotateY();
    float getRotateZ();
    float getScale();
    void initParams(QString name, float radius,
                     float tilt, float n1, float n2,
                     float i1, float i2, float w1, float w2,
                     float a1, float a2, float e1, float e2,
                     float m1, float m2, float period);
protected:
    float startD;
    float oldTimeD;
    float currTimeD;
    float m_n1, m_n2;
    float m_i1, m_i2;
    float m_w1, m_w2;
    float m_a1, m_a2;
    float m_e1, m_e2;
    float m_m1, m_m2;
protected slots:
    void rotate();
};

#endif // PLANET_H
