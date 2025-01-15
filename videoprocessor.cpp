#include <videoprocessor.h>
#include <QDateTime> // for log

VideoProcessor::VideoProcessor() :
    m_audioPlayer(new QAudioPlayer(8192 * 16))
{
}

void VideoProcessor::start_preview()
{
    FFmpegPlayer::start_preview(m_playUrl.toStdString());
    m_audioPlayer->start_consume_audio();
}
void VideoProcessor::stop_preview()
{
    FFmpegPlayer::stop_preview();
    m_audioPlayer->stop_consume_audio();
}

void VideoProcessor::on_new_frame_avaliable(std::shared_ptr<FrameCache> m_frame_cache)
{
    QImage frame_image(m_frame_cache->m_cache->data[0],
                 m_frame_cache->m_cache->width,
                 m_frame_cache->m_cache->height,
                 QImage::Format_RGBA8888);
    QImage image = std::move(frame_image);
    emit inDisplay(image);
    frame_consumed.store(true, std::memory_order_release);
}
void VideoProcessor::on_new_audio_frame_avaliable(std::shared_ptr<FrameCache> m_frame_cache)
{
    if(m_audioPlayer)
    {
        m_audioPlayer->write(reinterpret_cast<const char*>(m_frame_cache->m_cache->data[0]), m_frame_cache->m_cache->linesize[0]);
    }
}

int VideoProcessor::setPlayUrl(const QString& playUrl) {
    m_playUrl = playUrl;
    return 0;
}
