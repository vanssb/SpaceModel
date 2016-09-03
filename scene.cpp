#include "scene.h"
#include <math.h>

Scene::Scene(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this, SLOT(update()));
    //time.setInterval(10);
    m_timer.start();
    m_wireMode = false;
    scale = 1.0f;
    distance = 30.0f;
    xPos = astroUnit;
    yPos = 3000.0f;
    zPos = 0.0f;
    hRot = M_PI / 2;
    vRot = M_PI / 2;
    m_frameCount = 0;
}

void Scene::initDebugInfo(){
    Text2D::initText(&m_program, m_vertexAttr, m_textureAttr, m_textureUniform, m_matrixUniform);
    Text2D::setFont( "Arial", 36 );
    Text2D::setResolution( -width() + 20, -height() + 28 );
    strFPS = new Text2D();
    strFPS->setText( "FPS: 60.0", 0, 0 );
    strResolution = new Text2D();
    strResolution->setText( "Resolution: 800x600" , - width() + 20, - height() + 56 );
    strPos = new Text2D();
    strPos->setText("Position: ( 20.0, 20.0, -50.0)", -width() + 20, -height() + 92);
}

void Scene::showDebugInfo(){
    Text2D::setResolution( width(), height() );
    if ( m_time.elapsed() > 100 ){
        strFPS->setText("FPS: "+
                        QString::number( m_frameCount * 1000.0f / m_time.elapsed(), 'f', 2 ),
                         -width() + 20, -height() + 28 );
        m_time.restart();
        m_frameCount = 0;
    }
    m_frameCount++;
    strFPS->printText();
    strResolution->setText( "Resolution: "+
                            QString::number( width() ) + "x" +
                            QString::number( height() ),
                            -width() + 20,
                            -height() + 56 );
    strResolution->printText();

    strPos->setText( "Position: (" +
                           QString::number( xPos, 'f', 2 ) + ", " +
                           QString::number( yPos, 'f', 2 ) + ", " +
                           QString::number( zPos, 'f',2 ) + ")",
                           -width() + 20,
                           -height() + 92);
    strPos->printText();
}

void Scene::cleanDebugInfo(){
    delete strFPS;
    delete strResolution;
    delete strPos;
}


Scene::~Scene(){
    makeCurrent();
    delete factory;
    Text2D::cleanFont();
    cleanDebugInfo();
    doneCurrent();
}

void Scene::initializeGL(){
//Создание вершинного шейдера
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    vShader.compileSourceFile(":/Shaders/vShader.glsl");
//Создание фрагментного шейдера
    QOpenGLShader fShader(QOpenGLShader::Fragment);
    fShader.compileSourceFile(":/Shaders/fShader.glsl");
//Добавление шейдеров в конвеер и сборка ш. программы
    m_program.addShader(&vShader);
    m_program.addShader(&fShader);
    if ( !m_program.link() )
    {
        qWarning( "Error: unable to link a shader program." );
        return;
    }
//Привязка атрибутов
    m_vertexAttr = m_program.attributeLocation( "vertexAttr" );
    m_textureAttr = m_program.attributeLocation( "textureAttr" );
    m_matrixUniform = m_program.uniformLocation( "MVP" );
    m_textureUniform = m_program.uniformLocation( "textureUniform" );
//Начальные настройки OpenGL
    glClearColor(0.0f,0.0f,0.0f,1.0f);
//Включение теста глубины
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
//Включение отсечения невидимых граней
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);
//Создание и инициализация фабрики
    factory = new SystemFactory( &m_program, m_vertexAttr, m_textureAttr, m_textureUniform, m_matrixUniform );
    factory->initObjects();
    initDebugInfo();
    m_time.start();
}

void Scene::paintGL(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if ( !m_program.bind() )
        return;
//Установка способа отрисовки
    if (m_wireMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//Позиция камеры viewMatrix, использует: координаты камеры, точку направления взгляда и ось направленную вверх( обычно Oy т.e. вектор 0,1,0)
    vMatrix.setToIdentity();
    calcPoint();
    vMatrix.lookAt( QVector3D(xPos, yPos, zPos), QVector3D(xPoint, yPoint, zPoint), QVector3D(0, 1, 0) );
//Глобальные координаты modelMatrix
    factory->drawSystem( pMatrix, vMatrix );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    showDebugInfo();
    m_program.release();
}

void Scene::resizeGL(int w, int h){
//Матрица проекции, использует: угол обзора, соотношение сторон и ближнюю и дальнюю стенки проекции
    float aspect = float(w) / float(h ? h : 1);
    const float zNear = 0.1f, zFar = 650.0f * astroUnit, fov = 60.0;
    pMatrix.setToIdentity();
    pMatrix.perspective(fov, aspect, zNear, zFar);
}

void Scene::moveForward(){
    xPos = xPoint;
    yPos = yPoint;
    zPos = zPoint;
}

void Scene::moveBack(){
    xPos = xPos - distance * sin(vRot) * cos(hRot);
    yPos = yPos - distance * cos(vRot);
    zPos = zPos - distance * sin(vRot) * sin(hRot);
}

void Scene::rotateUp() // повернуть сцену вверх
{
    if (vRot > M_PI / 10) vRot -= 0.01f;
}

void Scene::rotateDown() // повернуть сцену вниз
{
    if (vRot < M_PI * 9 / 10) vRot += 0.01f;
}

void Scene::rotateLeft() // повернуть сцену влево
{
   if (hRot > 0)
       hRot -= 0.02f;
   else
       hRot = 2*M_PI - hRot;
}

void Scene::rotateRight() // повернуть сцену вправо
{
    if (hRot < 2 * M_PI)
        hRot += 0.02f;
    else
        hRot = hRot - 2 * M_PI;
}

void Scene::calcPoint(){
    xPoint = xPos + distance * sin(vRot) * cos(hRot);
    yPoint = yPos + distance * cos(vRot);
    zPoint = zPos + distance * sin(vRot) * sin(hRot);
}

void Scene::switchMode(){
    m_wireMode = !m_wireMode;
}

