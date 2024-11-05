#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::onOpenNetStreamTriggered() {
    m_openMedia = QSharedPointer<OpenMedia>::create();
    m_openMedia->show();
}
