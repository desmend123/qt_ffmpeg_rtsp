#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class OpenMedia;
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
    QSharedPointer<QWidget> OpenMedia{nullptr};

    Ui::MainWindow *ui;
    Ui::OpenMedia *om_ui;
};
#endif // MAINWINDOW_H
