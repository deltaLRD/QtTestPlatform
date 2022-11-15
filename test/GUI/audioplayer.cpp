#include "audioplayer.h"

AudioPlayer::AudioPlayer(irrklang::ISoundEngine *engine, irrklang::ISoundSource *soundSource, QObject *parent) : QObject(parent), sound(nullptr), engine(engine), soundSource(soundSource)
{
    //判断声音引擎是否为空
    if (!engine)
    {
        //为空则报错
        qDebug() << "ERROR::AudioPlayer::Constructor::SoundEngineIsNull";
    }
    //判断声源是否为空
    if (!soundSource)
    {
        //为空则报错
        qDebug() << "ERROR::AudioPlayer::Constructor::SoundSourceIsNull";
    }
}

AudioPlayer::~AudioPlayer()
{
    //停止播放
    stop();
}

void AudioPlayer::play(bool playLooped, float volume)
{
    //停止播放
    stop();
    //获取声音按照是否循环
    sound = engine->play2D(soundSource, playLooped, true);
    //设置音量
    sound->setVolume(volume);
    //开始播放
    sound->setIsPaused(false);
}

void AudioPlayer::setPaused(bool paused)
{
    if (sound)
    {
        sound->setIsPaused(paused);
    }
}

void AudioPlayer::stop()
{
    //如果有正在播放的声音
    if (sound)
    {
        //如果正在播放的声音未播完
        if (!sound->isFinished())
        {
            //停止当前播放的声音
            sound->stop();
        }
        //丢弃当前播放的声音
        sound->drop();
        //设置当前无声音正在播放
        sound = nullptr;
    }
}
