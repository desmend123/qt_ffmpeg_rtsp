#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMutex>
#include <QQueue>
#include <QTimer>

#include "openmediawindow.h"
#include "videoprocessor.h"

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

public:

public slots:
    void onOpenNetStreamTriggered();
    void play(const QString& playUrl);
    void onInDisplay(QImage image);

private:
    void play();

private:
    QSharedPointer<OpenMediaWindow> m_openMediaWindow{nullptr};
    Ui::MainWindow *m_ui;

    QSharedPointer<VideoProcessor> m_videoProcessor{};
};
#endif // MAINWINDOW_H
