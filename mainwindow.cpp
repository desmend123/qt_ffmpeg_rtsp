#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_player = QSharedPointer<QMediaPlayer>::create();
    m_mediaList = QSharedPointer<QMediaPlaylist>::create();

    connect(m_ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
    connect(m_player.data(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));

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
    m_player->setPlaylist(m_mediaList.data());
    m_player->setVideoOutput(m_ui->videoWidget);
    m_ui->videoWidget->show();
    m_player->play();
    m_openMediaWindow->hide();
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
        m_mediaList->addMedia(QUrl(playStr));
        playQueue.pop_front();
    }
    play();
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
    }
}
