#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QDebug>
#include <QtMath>
#include <QObject>

#include <irrKlang/irrKlang.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(irrklang::ISoundEngine *engine, irrklang::ISoundSource *soundSource, QObject *parent = nullptr);
    ~AudioPlayer();
    //播放声音，参数为是否循环、音量大小
    void play(bool playLooped = false, float volume = 1.0);
    //设置暂停
    void setPaused(bool paused);
    //停止播放
    void stop();

private:
    //当前声音
    irrklang::ISound *sound;
    //声音引擎
    irrklang::ISoundEngine *engine;
    //声源
    irrklang::ISoundSource *soundSource;

signals:
    void finished();

};

#endif // AUDIOPLAYER_H
