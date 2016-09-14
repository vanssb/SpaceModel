#include "star.h"

Star::Star(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture)
     : SpaceObject(program, vertexAttr, textureAttr,textureUniform, model, texture){
}

void Star::initParams(QString name, float radius){
    m_name = name;
    m_radius = radius;
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

float Star::getOrbit(){
    return 0;
}
