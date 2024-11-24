#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QQueue>
#include <QTimer>

#include "openmediawindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

public slots:
    void onOpenNetStreamTriggered();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void play(QString playUrl);
    void play(QQueue<QString> playQueue);
    void onPlayPauseButton();
    void onStopPlayButton();
    void onSeekForwardButton();
    void onSeekBackButton();
    void onVoiceButton();
    void setPlayProgress();
    void onSliderReleased();
    void onVoiceChangeNeeded(double percent);

private:
    void play();
    void clearMediaList();
    void setPlaybackIcon();
    void setPauseIcon();
    void setVoiceOnIcon();
    void setVoiceOffIcon();

private:
    QSharedPointer<OpenMediaWindow> m_openMediaWindow{nullptr};

    Ui::MainWindow *m_ui;

    QSharedPointer<QMediaPlayer> m_player{};
    QSharedPointer<QMediaPlaylist> m_mediaList{};
    QSharedPointer<QTimer> playProgressTimer{};

    int lastVolume{0};
    bool isVoiceOff{false};
};
#endif // MAINWINDOW_H
