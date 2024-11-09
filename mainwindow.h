#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>

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
    void play(QString playUrl);
    void play(QQueue<QString> queFile);

private:
    QSharedPointer<OpenMediaWindow> m_openMediaWindow{nullptr};

    Ui::MainWindow *m_ui;

    QSharedPointer<QMediaPlayer> m_player{};
    QSharedPointer<QMediaPlaylist> m_mediaList{};
};
#endif // MAINWINDOW_H
