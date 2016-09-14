#include "planet.h"
#include <math.h>
#include <QDebug>

Planet::Planet(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture)
    : SpaceObject(program, vertexAttr, textureAttr,textureUniform, model, texture)
{
    connect(&time,SIGNAL(timeout()),this, SLOT(rotate()));
    startD = 367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730530;
    currTimeD = startD;
    oldTimeD = startD;
}

void Planet::rotate(){
    oldTimeD = currTimeD;
    currTimeD = currTimeD + 0.007f;//Подставить день/кадр
    float deltaTimeD = currTimeD - oldTimeD;
    float N = ( m_n1 + m_n2 * currTimeD) * M_PI / 180;
    float iPlanet = ( m_i1 + m_i2 * currTimeD) * M_PI / 180;
    float w = ( m_w1 + m_w2 * currTimeD) * M_PI / 180;
    float a = m_a1 + m_a2 * currTimeD;
    float e = m_e1 + m_e2 * currTimeD;
    float M = ( m_m1 + m_m2 * currTimeD) * M_PI / 180;
    float E = M + e * sin(M) * (1.0 + e * cos(M));

    float xv = a * ( cos( E ) - e );
    float yv = a * ( sqrt(1.0 - e * e ) * sin( E ) );
    float v = atan2( yv, xv );

    float r = sqrt( xv * xv + yv * yv );

    float xh = r * ( cos( N ) * cos( v + w )
                  - sin( N ) * sin( v + w ) * cos( iPlanet ) );
    float zh = -r * ( sin( N ) * cos( v + w )
                   + cos( N ) * sin( v + w ) * cos( iPlanet ) );
    float yh = r * ( sin(w + v) * sin( iPlanet ) );

    m_x = xh * auScale;
    m_y = yh * auScale;
    m_z = zh * auScale;



    float radians = m_tilt * M_PI / 180;
    m_rotationX = 0;
    m_rotationY += (deltaTimeD / m_period ) * 2 * M_PI;
    m_rotationZ = radians;
}

void Planet::initParams(QString name, float radius,
                        float tilt, float n1, float n2,
                        float i1, float i2, float w1, float w2,
                        float a1, float a2, float e1, float e2,
                        float m1, float m2, float period){
    m_radius = radius;
    m_tilt = tilt;
    m_n1 = n1; m_n2 = n2;
    m_i1 = i1; m_i2 = i2;
    m_w1 = w1; m_w2 = w2;
    m_a1 = a1; m_a2 = a2;
    m_e1 = e1; m_e2 = e2;
    m_m1 = m1; m_m2 = m2;
    m_period = period;
    m_name = name;
    time.setInterval(1000/60);
    time.start();
}

float Planet::getRotateX(){
    return m_rotationX;
}

float Planet::getRotateY(){
    return m_rotationY;
}

float Planet::getRotateZ(){
    return m_rotationZ;
}


