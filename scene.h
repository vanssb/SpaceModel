#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QKeyEvent>
#include <systemfactory.h>
#include <text2d.h>
#include <QTime>

class Scene : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();
    void moveForward();
    void moveBack();
    void rotateUp();
    void rotateDown();
    void rotateRight();
    void rotateLeft();
protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
private:
    std::vector<Text2D*> text;
    int m_frameCount;
    void calcPoint();
    QOpenGLShaderProgram m_program;
    int m_vertexAttr;
    int m_textureAttr;
    int m_matrixUniform;
    int m_textureUniform;
    QTimer m_timer;
    QTime m_time;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    QMatrix4x4 mMatrix;
    SystemFactory* factory;
//Общие настройки
    bool m_wireMode;
//Параметры камеры
    float scale;
    float xPos, yPos, zPos;
    float distance;
    float xPoint, yPoint, zPoint;
    float hRot,vRot;
//Служебная информация
    //bool enabledDebugInfo;
    void initDebugInfo();
    void showDebugInfo();
    void cleanDebugInfo();
    Text2D* strFPS;
    Text2D* strResolution;
    Text2D* strPos;
public slots:
    void switchMode();
    //void switchDebugInformation();
};

#endif // SCENE_H
