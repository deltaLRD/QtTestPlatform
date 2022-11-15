#ifndef IMPORTER_H
#define IMPORTER_H

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <irrKlang/irrKlang.h>

struct Character
{
    QOpenGLTexture *texture;  // 字形纹理
    QVector2D       size;     // 字形大小
    QVector2D       bearing;  // 从基准线到字形左部/顶部的偏移值
    GLuint          advance;  // 原点距下一个字形原点的距离
};

class ResManager : public QObject
{
    Q_OBJECT
public:
    //获取ResManager
    static ResManager *getInstance();
    //删除ResManager
    static void freeGLOBJ();
    //获取texture
    QOpenGLTexture *getTexture(const QString &texPath);
    //获取Sound
    irrklang::ISoundSource *getSound(const QString &soundPath);
    //获取Character
    Character getCharacter(ushort charUnicode);

    //获取ISoundEngine
    inline irrklang::ISoundEngine *getISoundEngine()
    {
        return engine;
    }

private:
    explicit ResManager(QObject *parent = nullptr);
    ~ResManager();

    //单例唯一实例
    static ResManager instance;

    //texPath->Texture
    QMap<QString, QOpenGLTexture *> texMap;
    //irrklang引擎
    irrklang::ISoundEngine *engine;
    //soundPath->ISoundSource
    QMap<QString, irrklang::ISoundSource *> soundMap;
    //FreeType库
    FT_Library ft;
    //FreeType面
    FT_Face face;
    //字体ByteArray
    QByteArray fontData;
    //unicode->Character
    QMap<ushort, Character> charMap;

signals:

};

#endif // IMPORTER_H
