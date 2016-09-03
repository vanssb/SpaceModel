#include "spaceobject.h"
#include <QCoreApplication>
#include <math.h>


SpaceObject::SpaceObject(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, QString model, QString texture)
{
    m_program = program;
    m_vertexAttr = vertexAttr;
    m_textureAttr = textureAttr ;
    m_textureUniform = textureUniform;
    Loader::loadModel(QString(QCoreApplication::applicationDirPath()+"/Models/"+model).toStdString().c_str(),&m_vertices,&m_textureCord, &m_normals);
    m_texture = new QOpenGLTexture( QImage( QString(QCoreApplication::applicationDirPath()+"/Textures/"+texture) ).mirrored() );
    speedFactor = 0.001;
}

SpaceObject::~SpaceObject(){
    delete m_texture;
    m_texture = NULL;
}

void SpaceObject::draw(){

//Отрисовка объекта:
// 1. Привязка массивов к атрибутам в шейдере
// 2. Включение атрибутов
// 3. Отрисовка массива вершин с наложением текстуры
// 4. Отключение атрибутов

    m_texture->bind();
    m_program->setAttributeArray( m_vertexAttr, m_vertices.data(), 3 );
    m_program->setAttributeArray( m_textureAttr, m_textureCord.data(), 2 );
    m_program->setUniformValue( m_textureUniform, 0 );
    m_program->enableAttributeArray( m_vertexAttr );
    m_program->enableAttributeArray( m_textureAttr );

    glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() / 3 );

    m_program->disableAttributeArray( m_vertexAttr );
    m_program->disableAttributeArray( m_textureAttr );
    m_texture->release();
}

float SpaceObject::getTranslateX(){
    return m_x;
}

float SpaceObject::getTranslateY(){
    return m_y;
}

float SpaceObject::getTranslateZ(){
    return m_z;
}


float SpaceObject::getScale(){
    return scaleUnit * m_scale;
}

