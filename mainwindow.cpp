#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QTime>

const int SKIP_TIME = 15000; // unit is ms
const int CHECK_PLAY_TIME = 1000; // unit is ms
const double SLIDER_MAX_VALUE = 1000000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->playSlider->setMinimum(0);
    m_ui->playSlider->setMaximum(SLIDER_MAX_VALUE);
    m_player = QSharedPointer<QMediaPlayer>::create();
    m_mediaList = QSharedPointer<QMediaPlaylist>::create();

    connect(m_ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
    connect(m_player.data(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_ui->playPauseButton, SIGNAL(clicked()), this,
            SLOT(onPlayPauseButton()));
    connect(m_ui->stopPlayButton, SIGNAL(clicked()), this,
                SLOT(onStopPlayButton()));
    connect(m_ui->seekForwardButton, SIGNAL(clicked()), this,
            SLOT(onSeekForwardButton()));
    connect(m_ui->seekBackButton, SIGNAL(clicked()), this,
            SLOT(onSeekBackButton()));
    connect(m_ui->voiceButton, SIGNAL(clicked()), this,
            SLOT(onVoiceButton()));
    connect(m_ui->voiceRangeButton, SIGNAL(voiceChangeNeeded(double)), this,
            SLOT(onVoiceChangeNeeded(double)));
    connect(m_ui->playSlider, SIGNAL(sliderReleased()), this,
            SLOT(onSliderReleased()));

    playProgressTimer = QSharedPointer<QTimer>::create();
    connect(playProgressTimer.data(), SIGNAL(timeout()), this,
            SLOT(setPlayProgress()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::onOpenNetStreamTriggered() {
    if (m_openMediaWindow == nullptr) {
        m_openMediaWindow = QSharedPointer<OpenMediaWindow>::create();
        m_openMediaWindow->resize(640, 480);
        m_openMediaWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_openMediaWindow->show();

        connect(m_openMediaWindow.data(), SIGNAL(playUrlNeeded(QString)),
            this, SLOT(play(QString)));
        connect(m_openMediaWindow.data(), SIGNAL(playFileNeeded(QQueue<QString>)),
            this, SLOT(play(QQueue<QString>)));
    } else {
        m_openMediaWindow->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    m_openMediaWindow->close();
}

void MainWindow::clearMediaList() {
    int count = m_mediaList->mediaCount();
    if (count != 0) {
        m_mediaList->removeMedia(0, count);
    }
}


void MainWindow::play() {
    if(m_mediaList->isEmpty()) {
        return;
    }
    m_player->setPlaylist(m_mediaList.data());
    m_player->setVideoOutput(m_ui->videoWidget);
    m_ui->videoWidget->show();
    m_player->play();
    m_openMediaWindow->hide();

    setPauseIcon();
}

void MainWindow::play(QString playUrl) {
    clearMediaList();
    m_mediaList->addMedia(QUrl(playUrl));
    play();
}

void MainWindow::play(QQueue<QString> playQueue) {
    clearMediaList();
    while (!playQueue.empty()) {
        QString playStr = playQueue.front();
        m_mediaList->addMedia(QUrl::fromUserInput(playStr));
        playQueue.pop_front();
    }
    playProgressTimer->start(CHECK_PLAY_TIME);
    play();
}

void MainWindow::setPlaybackIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/play/icons/playback.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_ui->playPauseButton->setIcon(icon);
}

void MainWindow::setPauseIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/play/icons/pauseplay.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_ui->playPauseButton->setIcon(icon);
}

void MainWindow::setVoiceOnIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/play/icons/voiceon.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_ui->voiceButton->setIcon(icon);
}

void MainWindow::setVoiceOffIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/play/icons/voiceoff.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_ui->voiceButton->setIcon(icon);
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        setPlaybackIcon();
        playProgressTimer->stop();
        m_ui->playTimeLabel->setText("00:00");
        m_ui->remainTImeLabel->setText("00:00");
        m_ui->playSlider->setValue(0);
    } else {

    }
}

void MainWindow::onPlayPauseButton() {
    if (m_player->state() == QMediaPlayer::PlayingState) {
        m_player->pause();
        playProgressTimer->stop();
        setPlaybackIcon();
    } else if(m_player->state() == QMediaPlayer::PausedState) {
        m_player->play();
        playProgressTimer->start();
        setPauseIcon();
    } else if(m_player->state() == QMediaPlayer::StoppedState) {
        play();
        playProgressTimer->start();
    }else {
        return;
    }
}

void MainWindow::onStopPlayButton() {
    m_player->stop();
    playProgressTimer->stop();
    setPlaybackIcon();
    m_ui->playTimeLabel->setText("00:00");
    m_ui->remainTImeLabel->setText("00:00");
    m_ui->playSlider->setValue(0);
}

void MainWindow::onSeekForwardButton() {
    int position = m_player->position();
    int duration = m_player->duration();
    if (duration != 0) {
        m_player->setPosition(position + SKIP_TIME);
    }
}

void MainWindow::onSeekBackButton() {
    int position = m_player->position();
    int duration = m_player->duration();
    if (duration != 0) {
        m_player->setPosition(position - SKIP_TIME);
    }
}

void MainWindow::setPlayProgress() {
    int position = m_player->position();

    QTime time = QTime::fromMSecsSinceStartOfDay(position);
    QString str = time.toString("mm:ss");
    m_ui->playTimeLabel->setText(str);

    int duration = m_player->duration();
    if (duration != 0) {
        time = QTime::fromMSecsSinceStartOfDay(duration - position);
        str = time.toString("mm:ss");
        m_ui->remainTImeLabel->setText(str);

        double sliderVal = (position * SLIDER_MAX_VALUE) / duration;
        m_ui->playSlider->setValue(sliderVal);
    } else {
        m_ui->playSlider->setValue(0);
        m_ui->remainTImeLabel->setText("00:00");
    }
}

void MainWindow::onSliderReleased() {
    int value = m_ui->playSlider->value();
    int duration = m_player->duration();
    if(duration == 0) {
        return;
    }
    double position = value/ SLIDER_MAX_VALUE * duration;
    m_player->setPosition(position);
}

void MainWindow::onVoiceButton() {
    if (isVoiceOff) {
        m_player->setVolume(lastVolume);
        setVoiceOnIcon();
        isVoiceOff = false;
        m_ui->voiceRangeButton->setColor(Qt::green);
    } else {
        lastVolume = m_player->volume();
        m_player->setVolume(0);
        setVoiceOffIcon();
        isVoiceOff = true;
        m_ui->voiceRangeButton->setColor(Qt::gray);
    }
}

void MainWindow::onVoiceChangeNeeded(double percent) {
    double volume = percent * 100;
    m_player->setVolume(volume);
}
