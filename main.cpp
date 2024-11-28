#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/app/icons/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    MainWindow w;
    w.showMaximized();
    w.show();
    return a.exec();
}
