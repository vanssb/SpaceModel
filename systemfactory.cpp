#include "systemfactory.h"
#include <QCoreApplication>
#include <QSqlQuery>
#include <math.h>

SystemFactory::SystemFactory(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, int matrixUniform)
{
    m_program = program;
    m_vertexAttr = vertexAttr;
    m_textureAttr = textureAttr ;
    m_textureUniform = textureUniform;
    m_matrixUniform = matrixUniform;
    m_orbit = new QOpenGLTexture( QImage( QString(QCoreApplication::applicationDirPath()+"/Textures/Orbit.jpg") ).mirrored() );
}

SystemFactory::~SystemFactory(){
    for ( unsigned int i = 0; i < planets.size(); i++ ){
        delete planets[i];
        planets[i] = NULL;
    }
    planets.clear();
    delete m_orbit;
}

void SystemFactory::initObjects(){
//Инициализация планет с выгрузкой их параметров из базы
    data = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QCoreApplication::applicationDirPath()+"/Data/PlanetStorage.sqlite";
    data.setDatabaseName(path);
    if (data.open()){
        QSqlQuery q("SELECT * FROM Planets");
        while ( q.next() ){
            Planet* object = new Planet( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                        q.value("model").toString(), q.value("texture").toString() );
            object->initParams( q.value("name").toString(),
                                q.value("scale").toFloat(),
                                q.value("spin").toFloat(),
                                q.value("translateA").toFloat(),
                                q.value("translateB").toFloat(),
                                q.value("speed").toFloat(),
                                q.value("ecliptic").toFloat(),
                                q.value("obliquity").toFloat() );
            planets.push_back(object);
        }
        q.clear();
        q.exec("SELECT * FROM Satelites");
        while( q.next() ){
            Satelite* satelite = new Satelite( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                    q.value("model").toString(), q.value("texture").toString());
            satelite->initParams( planets[q.value("planet").toInt()-1],
                                  q.value("name").toString(),
                                  q.value("scale").toFloat(),
                                  q.value("spin").toFloat(),
                                  q.value("translateA").toFloat(),
                                  q.value("translateB").toFloat(),
                                  q.value("speed").toFloat(),
                                  q.value("ecliptic").toFloat() );
            satelites.push_back(satelite);
        }
        q.clear();
        q.exec( "SELECT * FROM Stars" );
        while( q.next() ){
            Star* star = new Star( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                   q.value("model").toString(), q.value("texture").toString() );
            star->initParams( q.value("name").toString(), q.value("scale").toFloat() );
            stars.push_back(star);
        }
    }
    else{
        qDebug() << data.lastError().text();
    }
}

void SystemFactory::drawSystem(QMatrix4x4 pMatrix, QMatrix4x4 vMatrix){
//Перебор списка звезд
    for (unsigned int i = 0; i < stars.size(); i++){
        mMatrix.setToIdentity();
        mMatrix.scale( stars[i]->getScale() );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix);
        stars[i]->draw();
    }
//Перебор списка планет
    for (unsigned int i = 0; i < planets.size(); i++){
    //
        mMatrix.setToIdentity();
        mMatrix.rotate( planets[i]->getEcliptic(), 1.0f, 0.0f, 0.0f );
        mMatrix.translate( planets[i]->getTranslateX(), planets[i]->getTranslateY(), planets[i]->getTranslateZ() );
        //Добавить наклон оси
        //Сюда
        mMatrix.rotate( planets[i]->getObliquity(), 1.0f, 0.0f, 0.0f );
        //
        mMatrix.rotate( planets[i]->getRotateAngle(), 0.0f, 1.0f, 0.0f );
        mMatrix.scale( planets[i]->getScale() );
    //Отправка mvp матрицы в шейдер( составляется в обратном порядке MVP = P * V * M)
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix );
    //Отрисовка объекта
        planets[i]->draw();
    //Отрисовка орбиты
        mMatrix.setToIdentity();
        mMatrix.rotate( planets[i]->getEcliptic(), 1.0f, 0.0f, 0.0f );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix);
        //drawOrbit(planets[i]);
    }
//Перебор списка спутников
    for (unsigned int i = 0; i < satelites.size(); i++){
        mMatrix.setToIdentity();
        mMatrix.translate( satelites[i]->getBase()->getTranslateX(),
                           satelites[i]->getBase()->getTranslateY(),
                           satelites[i]->getBase()->getTranslateZ() );
        mMatrix.rotate( satelites[i]->getEcliptic(), 1.0f, 0.0f, 0.0f );
        mMatrix.translate( satelites[i]->getTranslateX(), satelites[i]->getTranslateY(), satelites[i]->getTranslateZ() );
        mMatrix.scale( satelites[i]->getScale() );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix );
        satelites[i]->draw();
            //Отрисовка орбиты
        mMatrix.setToIdentity();
        mMatrix.translate( satelites[i]->getBase()->getTranslateX(),
                           satelites[i]->getBase()->getTranslateY(),
                           satelites[i]->getBase()->getTranslateZ() );
        mMatrix.rotate( satelites[i]->getEcliptic(), 1.0f, 0.0f, 0.0f );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix);
        //drawOrbit(satelites[i]);
    }
}

void SystemFactory::drawOrbit(SpaceObject* p){
    std::vector<float> orbit;
    std::vector<float> orbitTexture;

    for (float j = 0.0f; j < 2*M_PI; j+=0.01){
        orbit.push_back(p->getOrbit()*sin(j));
        orbit.push_back(p->getTranslateY());
        orbit.push_back(p->getOrbit()*cos(j) );
        orbitTexture.push_back(0.0f);
        orbitTexture.push_back(0.0f);
    }

    m_orbit->bind();
    glLineWidth(0.1f);
    m_program->setAttributeArray( m_vertexAttr, orbit.data(), 3 );
    m_program->setAttributeArray( m_textureAttr, orbitTexture.data(), 2 );
    m_program->setUniformValue( m_textureUniform, 0 );
    m_program->enableAttributeArray( m_vertexAttr );
    m_program->enableAttributeArray( m_textureAttr );

    glDrawArrays( GL_LINE_LOOP, 0, orbit.size() / 3 );

    m_program->disableAttributeArray( m_vertexAttr );
    m_program->disableAttributeArray( m_textureAttr );

    m_orbit->release();
}
