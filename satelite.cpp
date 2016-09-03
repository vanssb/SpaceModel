#include "satelite.h"

Satelite::Satelite(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture)
          : SpaceObject(program, vertexAttr, textureAttr,textureUniform, model, texture)
{
    connect(&time,SIGNAL(timeout()),this, SLOT(rotate()));
    m_hAngle = 0;
    m_angleSpin = 0;
}

void Satelite::initParams(Planet* base, QString name, float scaleFactor, float spinFactor, float ratioA, float ratioB, float speed, float ecliptic){
//Инициализация основных параметров спутника
    m_base = base;
    m_scale = scaleFactor;
    m_rateSpin = spinFactor;
    m_translateA = ratioA;
    m_translateB = ratioB;
    m_speed  = speed;
    m_ecliptic = ecliptic;
    m_name = name;
    time.setInterval( 1000 / 60 );
    time.start();
}

void Satelite::rotate(){
    m_x = astroUnit * m_translateA * sin( m_hAngle * M_PI / 180 );
    m_z = astroUnit * m_translateB * cos( m_hAngle * M_PI / 180 );
    m_y = 0.0f;

    m_hAngle += speedFactor * m_speed;
    m_angleSpin += speedFactor * m_rateSpin * m_speed;
    if (m_angleSpin >= 360.0f || m_angleSpin <= -360.0f){
        m_angleSpin = 0;
    }
    if ( m_hAngle >= 360.0f || m_hAngle <= -360.0f ) m_hAngle = 0;
}

float Satelite::getOrbit(){
    return astroUnit * m_translateA;
}

float Satelite::getEcliptic(){
    return m_ecliptic;
}

Planet* Satelite::getBase(){
    return m_base;
}

float Satelite::getRotateAngle(){
    return m_angleSpin;
}


