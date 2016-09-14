#include "satelite.h"

Satelite::Satelite(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture)
          : Planet(program, vertexAttr, textureAttr,textureUniform, model, texture)
{
    connect(&time,SIGNAL(timeout()),this, SLOT(rotate()));
    startD = 367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730530;
    currTimeD = startD;
    oldTimeD = startD;
}

void Satelite::initParams(Planet* base, QString name, float radius,
                          float tilt, float n1, float n2,
                          float i1, float i2, float w1, float w2,
                          float a1, float a2, float e1, float e2,
                          float m1, float m2, float period){
//Инициализация основных параметров спутника
    m_base = base;
    Planet::initParams(name, radius,
                       tilt, n1, n2,
                       i1, i2, w1, w2,
                       a1, a2, e1, e2,
                       m1, m2, period);
}


Planet* Satelite::getBase(){
    return m_base;
}



