#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <vector>
#include <loader.h>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QWidget>
#include <QTimer>
#include "types.h"

class SpaceObject : public QWidget
{
    Q_OBJECT
public:
    SpaceObject(QOpenGLShaderProgram *program,
                 int vertexAttr, int textureAttr,
                 int textureUniform , QString model, QString texture);
    ~SpaceObject();
    void draw();
    float getTranslateX();
    float getTranslateY();
    float getTranslateZ();
    float getScale();
    virtual float getOrbit()=0;
protected:
    QTimer time;
    float m_x, m_y, m_z;
    float m_scale;
    QString m_name;
    float speedFactor;
private:
    std::vector<float> m_vertices;
    std::vector<float> m_textureCord;
    std::vector<float> m_normals;
    QOpenGLTexture *m_texture;
    int m_vertexAttr;
    int m_textureAttr;
    int m_textureUniform;
    int m_textureEnable;
    QOpenGLShaderProgram *m_program;
};

#endif // SPACEOBJECT_H
