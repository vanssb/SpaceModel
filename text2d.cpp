#include "text2d.h"
#include "vector"
#include <QCoreApplication>
#include <QFile>

int Text2D::m_screenX = 800;
int Text2D::m_screenY = 600;
int Text2D::m_size = 36;
QString Text2D::m_font;
std::vector<char2D> Text2D::charSet;
QOpenGLTexture* Text2D::m_texture = NULL;
QOpenGLShaderProgram* Text2D::m_program = NULL;
int Text2D::m_vertexAttr = 0;
int Text2D::m_textureAttr = 0;
int Text2D::m_textureUniform = 0;
int Text2D::m_matrixUniform = 0;

Text2D::Text2D()
{

}

int Text2D::getCharNum(int id){
    for (unsigned int i = 0; i < charSet.size();i++){
        if (charSet[i].id == id) return i;
    }
    return 0;

}

void Text2D::setFont(QString font, int size){
    m_font = font;
    m_size = size;
    m_texture = new QOpenGLTexture( QImage(QString(QCoreApplication::applicationDirPath()+"/Fonts/Arial.png").toStdString().c_str()).mirrored() );
    QString fileName = QString(QCoreApplication::applicationDirPath()+"/Fonts/Arial.txt");
    QFile f( fileName );
    QString str;
    if (!f.open(QIODevice::ReadOnly)){
        return;
    }
    QTextStream stream(&f);
    while ( !stream.atEnd() ){
        QString line = stream.readLine();
        QList<QString> data = line.split( QRegExp( "[^0-9]+" ) );
        char2D c;
        c.id = data[1].toInt();
        c.uv_x = data[2].toFloat() / m_texture->width();
        c.uv_y = data[3].toFloat() / m_texture->height();
        c.uv_width = data[4].toFloat() / m_texture->width();
        c.uv_height = data[5].toFloat() / m_texture->height();
        c.width = data[4].toInt();
        c.height = data[5].toInt();
        charSet.push_back(c);
    }
    f.close();
}

Text2D::~Text2D(){
    delete m_texture;
    m_texture = NULL;
}

void Text2D::cleanUp(){
    m_vertices.clear();
    m_textures.clear();
}

void Text2D::setText(QString text, int x, int y){
    cleanUp();
    int length = text.length();    
    for ( int i = 0; i < length; i++ ){
    //Генерация полигонов для буквы
        char character = text[i].toLatin1();
        char2D c = charSet[ getCharNum( character ) ];
        float upLeftX = x;
        float upLeftY = y + c.height;

        float upRightX = x + c.width;
        float upRightY = y + c.height;

        float downRightX = x + c.width;
        float downRightY =  y;

        float downLeftX = x;
        float downLeftY = y;
        x += c.width;
    //Добавление координат в массив
        m_vertices.push_back( upLeftX ); m_vertices.push_back( upLeftY );
        m_vertices.push_back( downLeftX ); m_vertices.push_back( downLeftY );
        m_vertices.push_back( upRightX ); m_vertices.push_back( upRightY );

        m_vertices.push_back( downRightX ); m_vertices.push_back( downRightY );
        m_vertices.push_back( upRightX ); m_vertices.push_back( upRightY );
        m_vertices.push_back( downLeftX ); m_vertices.push_back( downLeftY );
    //Генерация текстурных вершин
        float uvUpLeftX = c.uv_x;
        float uvUpLeftY = 1.0f - c.uv_y;

        float uvUpRightX = c.uv_x + c.uv_width;
        float uvUpRightY = 1.0f - c.uv_y;

        float uvDownRightX = c.uv_x + c.uv_width;
        float uvDownRightY =  1.0f - ( c.uv_y + c.uv_height );

        float uvDownLeftX = c.uv_x;
        float uvDownLeftY = 1.0f - ( c.uv_y + c.uv_height );
    //Добавление текстурных координат в массив
        m_textures.push_back( uvUpLeftX ); m_textures.push_back( uvUpLeftY );
        m_textures.push_back( uvDownLeftX ); m_textures.push_back( uvDownLeftY );
        m_textures.push_back( uvUpRightX ); m_textures.push_back( uvUpRightY );

        m_textures.push_back( uvDownRightX ); m_textures.push_back( uvDownRightY );
        m_textures.push_back( uvUpRightX ); m_textures.push_back( uvUpRightY );
        m_textures.push_back( uvDownLeftX ); m_textures.push_back( uvDownLeftY );
    }
}

void Text2D::printText(){
//Отрисовка текста
    m_texture->bind();
    m_matrix.setToIdentity();
    m_matrix.ortho( - m_screenX, m_screenX, - m_screenY, m_screenY, 1.0f, -1.0f );
    m_program->setUniformValue( m_matrixUniform, m_matrix);
    m_program->setAttributeArray( m_vertexAttr, m_vertices.data(), 2 );
    m_program->setAttributeArray( m_textureAttr, m_textures.data(), 2 );
    m_program->setUniformValue( m_textureUniform, 0 );
    m_program->enableAttributeArray( m_vertexAttr );
    m_program->enableAttributeArray( m_textureAttr );

    glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() / 2 );

    m_program->disableAttributeArray( m_vertexAttr );
    m_program->disableAttributeArray( m_textureAttr );
    m_texture->release();
}

void Text2D::initText(QOpenGLShaderProgram *program, int vertexAttr, int textureAttr, int textureUniform, int matrixUniform){
    m_program = program;
    m_vertexAttr = vertexAttr;
    m_textureAttr = textureAttr;
    m_textureUniform = textureUniform;
    m_matrixUniform = matrixUniform;
}

void Text2D::setResolution(int screenX, int screenY){
    m_screenX = screenX;
    m_screenY = screenY;
}

void Text2D::cleanFont(){
    delete m_texture;
    m_texture = NULL;
}
