#ifndef QAUDIOPLAYER_H
#define QAUDIOPLAYER_H

#include <memory>
#include <mutex>

#include <QIORingBuffer.h>
#include <QAudioOutput>

class QAudioPlayer : public QIORingBuffer
{
    Q_OBJECT
public:
    QAudioPlayer() = default;

    QAudioPlayer(int buffer_size);
    ~QAudioPlayer();

    void start_consume_audio();
    void stop_consume_audio();
private:
    std::unique_ptr<QAudioOutput> m_audioOutput;
    std::mutex _mutex;
};

#endif // QAUDIOPLAYER_H
