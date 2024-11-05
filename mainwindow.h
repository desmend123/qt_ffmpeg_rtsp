#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openmedia.h"

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

private slots:
    void onOpenNetStreamTriggered();

private:
    QSharedPointer<OpenMedia> m_openMedia{nullptr};

    Ui::MainWindow *m_ui;
};
#endif // MAINWINDOW_H
