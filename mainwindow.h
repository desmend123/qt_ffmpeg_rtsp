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
    static int busProcess(GstData data, const QString prefix);

public slots:
    void onOpenNetStreamTriggered();
    void play(const QString& playUrl);
    void onInDisplay(QImage image);

private:
    void play();

    void setAppIcon();

private:
    QSharedPointer<OpenMediaWindow> m_openMediaWindow{nullptr};
    Ui::MainWindow *m_ui;

    QSharedPointer<VideoProcessor> m_videoProcessor{};

    QPixmap pixmap{};
    QSharedPointer<QGraphicsPixmapItem> m_GraphicItem{};
    QGraphicsScene* m_scene{};
};
#endif // MAINWINDOW_H
