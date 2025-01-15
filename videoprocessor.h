#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QPainter>
#include <QWidget>
#include <QTimer>

#include <FFmpegPlayer.h>
#include <QAudioPlayer.h>

class VideoProcessor : public QObject, public FFmpegPlayer
{
    Q_OBJECT
public:
    VideoProcessor();

    int setPlayUrl(const QString& playUrl);
signals:
    void inDisplay(QImage iamge);

public slots:
    void start_preview();
    void stop_preview();

protected:

    void on_new_frame_avaliable(std::shared_ptr<FrameCache> m_frame_cache) override;
    void on_new_audio_frame_avaliable(std::shared_ptr<FrameCache> m_frame_cache) override;
private:
    QImage m_image;
    std::unique_ptr<QAudioPlayer> m_audioPlayer;

    QString m_playUrl{};
    int m_playIndex{0};
};

#endif // VIDEOPROCESSOR_H
