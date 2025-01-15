#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QScreen>
#include <QThread>
#include <QTime>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_videoProcessor = QSharedPointer<VideoProcessor>::create();

    m_videoProcessor->moveToThread(new QThread(this));

    connect(m_videoProcessor->thread(), SIGNAL(started()),
            m_videoProcessor.data(), SLOT(start_preview()));
    connect(m_videoProcessor.data(), SIGNAL(inDisplay(QImage)),
            this, SLOT(onInDisplay(QImage)));


    connect(m_ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
    onOpenNetStreamTriggered();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::onOpenNetStreamTriggered() {
    if (m_openMediaWindow == nullptr) {
        m_openMediaWindow = QSharedPointer<OpenMediaWindow>::create(this);

        QList<QScreen*> screens = QApplication::screens();
        int i = 0;
        for (; i <= screens.size() - 1; i++) {
            QRect geometry = screens[i]->geometry();

            int x = geometry.x() + geometry.width() / 4;
            int y = geometry.y() + geometry.height() / 5;
            m_openMediaWindow->setGeometry(x, y, geometry.width() / 2, geometry.height() * 3 / 5);
            break;
        }

        m_openMediaWindow->setWindowFlags(Qt::Tool);
        m_openMediaWindow->show();

        connect(m_openMediaWindow.data(), SIGNAL(playUrlNeeded(QString)),
            this, SLOT(play(QString)));
    } else {
        m_openMediaWindow->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    m_openMediaWindow->close();

    m_videoProcessor->thread()->quit();
    m_videoProcessor->thread()->wait();
}

void MainWindow::play() {
    if (m_videoProcessor->thread()->isRunning()) {
        m_videoProcessor->thread()->quit();
        m_videoProcessor->thread()->wait();
    }
    m_videoProcessor->thread()->start();
    if (m_openMediaWindow != nullptr) {
        m_openMediaWindow->hide();
    }
}

void MainWindow::play(const QString& playUrl) {
    if (playUrl.isEmpty()) {
        return;
    }
    m_videoProcessor->setPlayUrl(playUrl);
    play();
}

void MainWindow::onInDisplay(QImage image) {
    m_ui->playWidget->setImage(image);
    m_ui->playWidget->update();
}
