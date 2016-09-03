#ifndef TEXT2D_H
#define TEXT2D_H

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <vector>
#include <QMatrix4x4>
#include <types.h>

class Text2D
{
public:
    Text2D();
    ~Text2D();
    void printText();
    void setText(QString text, int x, int y);
    static void setResolution(int screenX, int screenY);
    static void setFont(QString font, int size);
    static void cleanFont();
    static void initText(QOpenGLShaderProgram* program,
                         int vertexAttr,
                         int textureAttr,
                         int textureUniform,
                         int matrixUniform);
private:
    QString m_text;
    std::vector<float> m_vertices;
    std::vector<float> m_textures;
    QMatrix4x4 m_matrix;
    void cleanUp();
    int getCharNum( int id );
    static QOpenGLShaderProgram* m_program;
    static int m_vertexAttr, m_textureAttr, m_textureUniform, m_matrixUniform;
    static int m_size;
    static int m_screenX, m_screenY;
    static QString m_font;
    static std::vector<char2D> charSet;
    static QOpenGLTexture* m_texture;
};

#endif // TEXT2D_H
