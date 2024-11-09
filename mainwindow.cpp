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
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::onOpenNetStreamTriggered() {
    m_openMediaWindow = QSharedPointer<OpenMediaWindow>::create();
    m_openMediaWindow->resize(640, 480);
    m_openMediaWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_openMediaWindow->show();

    connect(m_openMediaWindow.data(), SIGNAL(playUrlNeeded(QString)),
            this, SLOT(play(QString)));
    connect(m_openMediaWindow.data(), SIGNAL(playFileNeeded(QQueue<QString>)),
            this, SLOT(play(QQueue<QString>)));
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    m_openMediaWindow->close();
}

void MainWindow::play(QString playUrl) {
    m_mediaList->addMedia(QUrl(playUrl));
    m_player->setPlaylist(m_mediaList.data());
    m_player->setVideoOutput(m_ui->videoWidget);
    m_ui->videoWidget->show();
    m_mediaList->setCurrentIndex(0);
    m_player->play();
    m_openMediaWindow->close();
}

void MainWindow::play(QQueue<QString> queFile) {
    //while (1) {
    //    QString playStr = queFile.front();
    //    queFile.pop_front();
    //    play(playStr);
    //}
}
