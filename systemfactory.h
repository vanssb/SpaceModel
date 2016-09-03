#ifndef SYSTEMFACTORY_H
#define SYSTEMFACTORY_H

#include <planet.h>
#include <satelite.h>
#include <star.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class SystemFactory
{
public:
    SystemFactory(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, int matrixUniform);
    ~SystemFactory();
    void initObjects();
    void drawSystem(QMatrix4x4 pMatrix, QMatrix4x4 vMatrix);
private:
    void drawOrbit(SpaceObject *p);
    std::vector<Planet*> planets;
    std::vector<Satelite*> satelites;
    std::vector<Star*> stars;
    int m_vertexAttr;
    int m_textureAttr;
    int m_textureUniform;
    int m_matrixUniform;
    QMatrix4x4 mMatrix;
    QOpenGLShaderProgram *m_program;
    QSqlDatabase data;
    QOpenGLTexture *m_orbit;
};

#endif // SYSTEMFACTORY_H
