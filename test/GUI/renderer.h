#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>

#include "resmanager.h"

class Renderer : public QObject
{
    Q_OBJECT
public:
    enum HorAlign
    {
      LEFT,
      CENTER,
      RIGHT,
    };
    explicit Renderer(QObject *parent = nullptr);
    ~Renderer();
    //开始绘制
    void begin();
    //结束绘制
    void end();
    //改变窗口大小
    void resize(int w, int h);
    //绘制精灵
    void drawSprite(QOpenGLTexture *texture);
    //绘制文字
    void drawText(const QString& text, float x, float y,
                  float scale, HorAlign align = LEFT, bool compact = true);
    //设置文字颜色
    void setTextColor(QVector3D textColor);
    //设置文字透视矩阵
    void setTextProjection(QMatrix4x4 textProjection);
    //设置精灵颜色
    void setSpriteColor(QVector3D spriteColor);
    //设置精灵模型矩阵
    void setSpriteModel(QMatrix4x4 spriteModel);
    //设置精灵透视矩阵
    void setSpriteProjection(QMatrix4x4 spriteProjection);
    //设置视口
    void setViewport(int x, int y, int width, int height);

private:
    //OpenGL对象
    void loadOpenGLOBJ();
    //绑定文字着色器程序
    void checkTextSP();
    //绑定精灵着色器程序
    void checkSpriteSP();
    //加载着色器程序
    QOpenGLShaderProgram *loadShaderPro(const QString& vShaderFile, const QString& fShaderFile);
    //绘制器状态
    enum RendererFlag
    {
        UsedShaderPro         = 0x0001,
        IsSpriteShaderPro     = 0x0002,
    };
    Q_DECLARE_FLAGS(RendererFlags, RendererFlag)

    //OpenGL函数
    QOpenGLFunctions_3_3_Core *f;

    //顶点缓冲对象、索引缓冲对象
    QOpenGLBuffer m_vbo, m_ebo;
    //顶点数组对象
    QOpenGLVertexArrayObject m_vao;

    //文字着色器程序
    QOpenGLShaderProgram *textSP;
    //精灵着色器程序
    QOpenGLShaderProgram *spriteSP;

    //绘制器状态
    RendererFlags rendererFlag;

signals:

};

#endif // RENDERER_H
