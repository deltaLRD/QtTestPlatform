#include "resmanager.h"

ResManager ResManager::instance;

ResManager::ResManager(QObject *parent) : QObject(parent), engine(nullptr)
{
    Q_INIT_RESOURCE(res);
    //加载声音引擎
    engine = irrklang::createIrrKlangDevice();
    if (!engine)
    {
        //加载声音引擎失败则报错
        qDebug() << "ERROR::ResManager::Constructor::CreateEngineFailed";
        return;
    }
    //初始化FreeType库
    if (FT_Init_FreeType(&ft))
        qDebug() << "ERROR::FREETYPE: Could not init FreeType Library";
    //使用QFile加载字体文件
    QFile fontFile(":/fonts/KenVector Future");
    //    QFile fontFile("STHUPO.TTF");
    //打开字体文件
    if (!fontFile.open(QIODevice::ReadOnly))
    {
        //打开失败则报错
        qDebug() << "ERROR::ResManager::Constructor::OpenFontFile::Failed";
    }
    //读取字体文件至ByteArray
    fontData = fontFile.readAll();
    //关闭字体文件
    fontFile.close();
    //从内存中读取字体至FreeType面
    if (FT_New_Memory_Face(ft, (const FT_Byte *)fontData.constData(), fontData.size(), 0, &face))
        qDebug() << "ERROR::FREETYPE: Failed to load font";
    //设置字体大小(5/8px)
    FT_Set_Pixel_Sizes(face, 0, 48);
}

ResManager::~ResManager()
{
    //关闭声音引擎
    engine->drop();
    //关闭FreeType面
    FT_Done_Face(face);
    //关闭FreeType库
    FT_Done_FreeType(ft);
}

ResManager *ResManager::getInstance()
{
    //返回唯一实例
    return &instance;
}

void ResManager::freeGLOBJ()
{
    //删除所有纹理
    for (QOpenGLTexture *tex : instance.texMap)
    {
        delete tex;
    }
    //删除所有字形
    for (Character &c : instance.charMap)
    {
        delete c.texture;
    }
}

QOpenGLTexture *ResManager::getTexture(const QString &texPath)
{
    //已加载则立即返回Texture
    QMap<QString, QOpenGLTexture *>::const_iterator i = texMap.find(texPath);
    if (i != texMap.end())
    {
        return i.value();
    }
    //未加载则创建Texture并加入texMap
    QOpenGLTexture *texture = new QOpenGLTexture(QImage(texPath), QOpenGLTexture::GenerateMipMaps);
    //设置过滤器
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    //设置包裹模式
    texture->setWrapMode(QOpenGLTexture::ClampToBorder);
    //添加新纹理至Map
    texMap[texPath] = texture;
    //返回新纹理
    return texMap[texPath];
}

irrklang::ISoundSource *ResManager::getSound(const QString &soundPath)
{
    //已加载则立即返回ISoundSource
    QMap<QString, irrklang::ISoundSource *>::const_iterator i = soundMap.find(soundPath);
    if (i != soundMap.end())
    {
        return i.value();
    }
    //未载则创建ISoundSource并加入soundMap
    //使用QFile加载声音文件
    QFile soundFile(soundPath);
    //打开声音文件
    if (!soundFile.open(QIODevice::ReadOnly))
    {
        //未打开则报错
        qDebug() << "ERROR::ResManager::OpenSoundFile::Failed";
    }
    //加载声音文件至ByteArray
    QByteArray soundData = soundFile.readAll();
    //关闭声音文件
    soundFile.close();
    //从内存中加载声源并放入Map
    soundMap[soundPath] = engine->addSoundSourceFromMemory(soundData.data(), soundData.size(), soundPath.toUtf8().data());
    //判断声源是否为空
    if (!soundMap[soundPath])
    {
        //为空则报错
        qDebug() << "ERROR::ResManager::getSound::SoundSourceIsNull";
    }
    //设置流播放阈值
    soundMap[soundPath]->setForcedStreamingThreshold(irrklang::ESM_AUTO_DETECT);
    //返回声源
    return soundMap[soundPath];
}

Character ResManager::getCharacter(ushort charUnicode)
{
    //已加载则立即返回Character
    QMap<ushort, Character>::const_iterator i = charMap.find(charUnicode);
    if (i != charMap.end())
    {
        return i.value();
    }
    //未载则创建Character并加入charMap
    //加载该字符
    if (FT_Load_Char(face, charUnicode, FT_LOAD_RENDER))
        qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
    //创建纹理
    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    //设置纹理宽高
    texture->setSize(face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
    //设置纹理格式
    texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    //申请空间
    texture->allocateStorage();
    //设置输入格式、类型，添加数据
    texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, face->glyph->bitmap.buffer);
    //设置过滤器
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    //设置包裹模式
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    //创建字形并放入Map
    charMap[charUnicode] = {
        texture,
        QVector2D(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        QVector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (GLuint)face->glyph->advance.x,
    };
    //    qDebug() << face->glyph->bitmap.width << face->glyph->bitmap.rows;
    //返回新加载字形
    return charMap[charUnicode];
}
