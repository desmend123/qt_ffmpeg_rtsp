#include <QAudioPlayer.h>

QAudioPlayer::QAudioPlayer(int buffer_size)
    :QIORingBuffer(buffer_size)
{
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    m_audioOutput.reset(new QAudioOutput(format));
    m_audioOutput->setVolume(100);
    if(!this->open(QIODevice::ReadWrite | QIODevice::Unbuffered))
    {
        qCritical() << "Failed open QAudioPlayer";
    };
}

QAudioPlayer::~QAudioPlayer()
{
    m_audioOutput->stop();
}
void QAudioPlayer::start_consume_audio()
{
    std::lock_guard<std::mutex> _l(_mutex);
    if(m_audioOutput->state() == QAudio::StoppedState)
    {
        m_audioOutput->start(this);
    }
}
void QAudioPlayer::stop_consume_audio()
{
    std::lock_guard<std::mutex> _l(_mutex);
    if(m_audioOutput->state() == QAudio::ActiveState)
    {
        m_audioOutput->stop();
    }
}
