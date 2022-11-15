#include "renderer.h"

Renderer::Renderer(QObject *parent) : QObject(parent), m_ebo(QOpenGLBuffer::IndexBuffer),
    rendererFlag(0x0000)
{
    //获取OpenGL函数
    f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    //加载text、sprite着色器程序
    textSP = loadShaderPro(":/shaders/textVS", ":/shaders/textFS");
    checkTextSP();
    textSP->setUniformValue("text", 0);
    setTextColor(QVector3D(1.0f, 1.0f, 1.0f));
    spriteSP = loadShaderPro(":/shaders/spriteVS", ":/shaders/spriteFS");
    checkSpriteSP();
    spriteSP->setUniformValue("sprite", 0);
    setSpriteColor(QVector3D(1.0f, 1.0f, 1.0f));
    //加载vao、vbo、ebo
    loadOpenGLOBJ();
}

Renderer::~Renderer()
{
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
}

void Renderer::begin()
{
    m_vao.bind();
}

void Renderer::end()
{
    m_vao.release();
}

void Renderer::resize(int w, int h)
{
    //更新文字、精灵透视矩阵与窗口一致
    QMatrix4x4 projection;
    projection.ortho(0, w, h, 0, -1, 1);
    this->setTextProjection(projection);
    this->setSpriteProjection(projection);
}

void Renderer::drawSprite(QOpenGLTexture *texture)
{
    //判断纹理是否为空
    if (!texture)
    {
        //为空则报错
        qDebug() << "ERROR::Renderer::drawSprite::TextureIsNull";
        return;
    }
    //绑定spriteSP
    checkSpriteSP();
    //计算顶点数据
    float halfWid = texture->width() / 2.0;
    float halfHei = texture->height() / 2.0;
    float vertices[] = {
         halfWid,  halfHei, 1.0f, 1.0f, // 3 * * 0
         halfWid, -halfHei, 1.0f, 0.0f, // * *   *
        -halfWid, -halfHei, 0.0f, 0.0f, // *   * *
        -halfWid,  halfHei, 0.0f, 1.0f, // 2 * * 1
    };
    //绑定纹理
    texture->bind();
    //填充顶点数据
    m_vbo.bind();
    m_vbo.write(0, vertices, sizeof(vertices));
    m_vbo.release();
    //按索引绘制
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::drawText(const QString& text, float x, float y, float scale, HorAlign align, bool compact)
{
    //绑定textSP
    checkTextSP();
    ResManager *resManager = ResManager::getInstance();
    //禁用字节对齐限制
    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //以H为标准
    static Character charH = resManager->getCharacter((ushort)'H');
    //text字符、顶点数据
    Character ch[text.size()];
    float vertices[text.size()][16];
    float offset = 0;
    //记录每个text
    for (int i = 0; i < text.size(); i++)
    {
        //获取每个字符
        ch[i] = resManager->getCharacter(text[i].unicode());
        //等间距绘制字符
        if (!compact)
        {
            offset += ((charH.advance - ch[i].advance) >> 6);
        }
        //计算顶点数据
        GLfloat xpos = offset + ch[i].bearing.x();
        GLfloat ypos = charH.size.y() - ch[i].bearing.y();
        GLfloat w = ch[i].size.x();
        GLfloat h = ch[i].size.y();
        float ver[] = {
             xpos + w, ypos + h, 1.0f, 1.0f, // 3 * * 0
             xpos + w, ypos,     1.0f, 0.0f, // * *   *
             xpos,     ypos,     0.0f, 0.0f, // *   * *
             xpos,     ypos + h, 0.0f, 1.0f, // 2 * * 1
        };
        memcpy(vertices[i], ver, sizeof(ver));
        //改变x坐标
        if (i == text.size() - 1)
            offset += ch[i].size.x();
        else
            offset += (ch[i].advance >> 6);
    }
    //设置模型矩阵
    QMatrix4x4 textModel;
    textModel.scale(scale);
    if (align == CENTER)
    {
        textModel.translate(-offset / 2, 0);
    }
    else if (align == RIGHT)
    {
        textModel.translate(-offset, 0);
    }
    textModel.translate(x, y);
    textSP->setUniformValue("model", textModel);
    //绘制text
    for (int i = 0; i < text.size(); i++)
    {
        //绑定纹理
        ch[i].texture->bind();
        //填充顶点数据
        m_vbo.bind();
        m_vbo.write(0, vertices[i], sizeof(vertices[i]));
        m_vbo.release();
        //按索引绘制
        f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Renderer::setTextColor(QVector3D textColor)
{
    checkTextSP();
    textSP->setUniformValue("textColor", textColor);
}

void Renderer::setTextProjection(QMatrix4x4 textProjection)
{
    checkTextSP();
    textSP->setUniformValue("projection", textProjection);
}

void Renderer::setSpriteColor(QVector3D spriteColor)
{
    checkSpriteSP();
    spriteSP->setUniformValue("spriteColor", spriteColor);
}

void Renderer::setSpriteModel(QMatrix4x4 spriteModel)
{
    checkSpriteSP();
    spriteSP->setUniformValue("model", spriteModel);
}

void Renderer::setSpriteProjection(QMatrix4x4 spriteProjection)
{
    checkSpriteSP();
    spriteSP->setUniformValue("projection", spriteProjection);
}

void Renderer::setViewport(int x, int y, int width, int height)
{
    f->glViewport(x, y, width, height);
}

QOpenGLShaderProgram *Renderer::loadShaderPro(const QString& vShaderFile, const QString& fShaderFile)
{
    QOpenGLShaderProgram *shaderPro = new QOpenGLShaderProgram(this);
    //添加顶点着色器
    if (!shaderPro->addShaderFromSourceFile(QOpenGLShader::Vertex, vShaderFile))
    {
        qDebug() << "ERROR::Renderer::LoadShaderPro::AddVertex";
        qDebug() << shaderPro->log();
    }
    //添加片段着色器
    if (!shaderPro->addShaderFromSourceFile(QOpenGLShader::Fragment, fShaderFile))
    {
        qDebug() << "ERROR::Renderer::LoadShaderPro::AddFragment";
        qDebug() << shaderPro->log();
    }
    //连接着色器程序
    if(!shaderPro->link()){
        qDebug() << "ERROR::Renderer::LoadShaderPro::LinkFailed" << endl;
        qDebug() << shaderPro->log() << endl;
    }
    return shaderPro;
}

void Renderer::loadOpenGLOBJ()
{
    //填充索引数组
    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };
    //创建vao
    m_vao.create();
    m_vao.bind();
    //创建vbo
    m_vbo.create();
    m_vbo.bind();
    //申请vbo内存
    m_vbo.allocate(sizeof(GLfloat) * 4 * 4);
    //设置使用模式
    m_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    //绑定顶点属性
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //解绑vbo
    m_vbo.release();
    //绑定ebo、加载索引
    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(indices, sizeof(indices));
    //解绑vao、ebo
    m_vao.release();
    m_ebo.release();
}

void Renderer::checkTextSP()
{
    //未绑定着色器、当前是精灵着色器
    if (!rendererFlag.testFlag(UsedShaderPro))
    {
        textSP->bind();
        rendererFlag.setFlag(UsedShaderPro, true);
        rendererFlag.setFlag(IsSpriteShaderPro, false);
    }
    else if (rendererFlag.testFlag(IsSpriteShaderPro))
    {
        textSP->bind();
        rendererFlag.setFlag(IsSpriteShaderPro, false);
    }
}

void Renderer::checkSpriteSP()
{
    //未绑定着色器、当前不是精灵着色器
    if (!rendererFlag.testFlag(UsedShaderPro))
    {
        spriteSP->bind();
        rendererFlag.setFlag(UsedShaderPro, true);
        rendererFlag.setFlag(IsSpriteShaderPro, true);
    }
    else if (!rendererFlag.testFlag(IsSpriteShaderPro))
    {
        spriteSP->bind();
        rendererFlag.setFlag(IsSpriteShaderPro, true);
    }
}
