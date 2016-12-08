#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <vector>
#include <loader.h>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QWidget>
#include <QTimer>
#include <QDate>
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
    float getRadius();
    QString getName();
    static void setDate(int year, int month, int day);
    static void setDate(QDate date);
protected:
    float startD;
    float oldTimeD;
    float currTimeD;
    static int year;
    static int month;
    static int day;
    QTimer time;
    float m_x, m_y, m_z;
    float m_radius;
    QString m_name;
    float m_period;
    float m_rotationX, m_rotationY, m_rotationZ;
    float m_tilt;
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
