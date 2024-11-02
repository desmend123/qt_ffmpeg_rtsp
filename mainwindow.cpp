#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_openmedia.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenNetStreamTriggered() {
    OpenMedia = QSharedPointer<QWidget>::create();
    om_ui->setupUi(OpenMedia.data());
    OpenMedia->show();
}
