#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>

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

private slots:
    void onOpenNetStreamTriggered();

private:
    QSharedPointer<OpenMediaWindow> m_openMediaWindow{nullptr};

    Ui::MainWindow *m_ui;
};
#endif // MAINWINDOW_H
