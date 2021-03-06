#include "systemfactory.h"
#include <QCoreApplication>
#include <QSqlQuery>
#include <math.h>
#include <logger.h>
#include <QDate>

SystemFactory::SystemFactory(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, int matrixUniform)
{
    Logger::write(LOG_INFO, "Factory created.");
    m_program = program;
    m_vertexAttr = vertexAttr;
    m_textureAttr = textureAttr ;
    m_textureUniform = textureUniform;
    m_matrixUniform = matrixUniform;
    m_orbit = new QOpenGLTexture( QImage( QString(QCoreApplication::applicationDirPath()+"/Textures/Orbit.jpg") ).mirrored() );
    Planet::setDate(QDate::currentDate());
}

SystemFactory::~SystemFactory(){
    Logger::write(LOG_INFO, "Factory deleted.");
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
            Planet* planet = new Planet( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                        q.value("model").toString(), q.value("texture").toString() );
            planet->initParams( q.value("name").toString(),
                                q.value("radius").toFloat(),
                                q.value("tilt").toFloat(),
                                q.value("n1").toFloat(), q.value("n2").toFloat(),
                                q.value("i1").toFloat(), q.value("i2").toFloat(),
                                q.value("w1").toFloat(), q.value("w2").toFloat(),
                                q.value("a1").toFloat(), q.value("a2").toFloat(),
                                q.value("e1").toFloat(), q.value("e2").toFloat(),
                                q.value("m1").toFloat(), q.value("m2").toFloat(),
                                q.value("period").toFloat() );
            planets.push_back(planet);
        }
        Logger::write(LOG_INFO, "Planets loaded.");
        q.clear();
        q.exec("SELECT * FROM Satelites");
        while( q.next() ){
            Satelite* satelite = new Satelite( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                    q.value("model").toString(), q.value("texture").toString());
            satelite->initParams( planets[q.value("planet").toInt()-1],
                                  q.value("name").toString(),
                                                    q.value("radius").toFloat(),
                                                    q.value("tilt").toFloat(),
                                                    q.value("n1").toFloat(), q.value("n2").toFloat(),
                                                    q.value("i1").toFloat(), q.value("i2").toFloat(),
                                                    q.value("w1").toFloat(), q.value("w2").toFloat(),
                                                    q.value("a1").toFloat(), q.value("a2").toFloat(),
                                                    q.value("e1").toFloat(), q.value("e2").toFloat(),
                                                    q.value("m1").toFloat(), q.value("m2").toFloat(),
                                                    q.value("period").toFloat() );
            satelites.push_back(satelite);
        }
        Logger::write(LOG_INFO, "Satelites loaded.");
        q.clear();
        q.exec( "SELECT * FROM Stars" );
        while( q.next() ){
            Star* star = new Star( m_program, m_vertexAttr, m_textureAttr, m_textureUniform,
                                   q.value("model").toString(), q.value("texture").toString() );
            star->initParams( q.value("name").toString(), q.value("radius").toFloat() );
            stars.push_back(star);
        }
         Logger::write( LOG_INFO, "Stars loaded" );
    }
    else{
        Logger::write( LOG_ERROR, data.lastError().text() );
    }
}

void SystemFactory::drawSystem(QMatrix4x4 pMatrix, QMatrix4x4 vMatrix){
//Перебор списка звезд
    for (unsigned int i = 0; i < stars.size(); i++){
        mMatrix.setToIdentity();
        mMatrix.scale( stars[i]->getRadius(), stars[i]->getRadius(), stars[i]->getRadius() );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix);
        stars[i]->draw();
    }
//Перебор списка планет
    for (unsigned int i = 0; i < planets.size(); i++){
    //
        mMatrix.setToIdentity();
        mMatrix.translate( planets[i]->getTranslateX(), planets[i]->getTranslateY(), planets[i]->getTranslateZ() );
        mMatrix.rotate( planets[i]->getRotateX(), 1.0f, 0.0f, 0.0f );
        mMatrix.rotate( planets[i]->getRotateY(), 0.0f, 1.0f, 0.0f );
        mMatrix.rotate( planets[i]->getRotateZ(), 0.0f, 0.0f, 1.0f );
        mMatrix.scale( planets[i]->getRadius(), planets[i]->getRadius(), planets[i]->getRadius() );
    //Отправка mvp матрицы в шейдер( составляется в обратном порядке MVP = P * V * M)
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix );
    //Отрисовка объекта
        planets[i]->draw();
    }
//Перебор списка спутников
    for (unsigned int i = 0; i < satelites.size(); i++){
        mMatrix.setToIdentity();
        mMatrix.translate( satelites[i]->getBase()->getTranslateX(),
                           satelites[i]->getBase()->getTranslateY(),
                           satelites[i]->getBase()->getTranslateZ() );
        mMatrix.translate( satelites[i]->getTranslateX(), satelites[i]->getTranslateY(), satelites[i]->getTranslateZ() );
        mMatrix.rotate( satelites[i]->getRotateX(), 1.0f, 0.0f, 0.0f );
        mMatrix.rotate( satelites[i]->getRotateY(), 0.0f, 1.0f, 0.0f );
        mMatrix.rotate( satelites[i]->getRotateZ(), 0.0f, 0.0f, 1.0f );
        mMatrix.scale( satelites[i]->getRadius() );
        m_program->setUniformValue( m_matrixUniform, pMatrix * vMatrix *  mMatrix );
        satelites[i]->draw();
    }
}

void SystemFactory::drawOrbit(SpaceObject* p){
    /*
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
    */
}
