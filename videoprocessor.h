#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QImage>
#include <QQueue>
#include <QString>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

struct GstData {
    GstElement *pipeline = nullptr;
    GstElement *sinkVideo = nullptr;
};

class VideoProcessor : public QObject {
    Q_OBJECT
public:
    VideoProcessor();
    ~VideoProcessor();

    int setPlayUrl(const QString& playUrl);
    int gstHandle();
    int setCloseFlag(bool isClosed);
signals:
    void inDisplay(QImage iamge);
public slots:
    void startVideo();

private:
    QString m_playUrl{};
    int m_playIndex{0};

    GstData m_gstData{};
    bool m_isClosed{false};
};

#endif // VIDEOPROCESSOR_H
