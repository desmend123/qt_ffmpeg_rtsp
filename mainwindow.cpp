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
    m_openMediaWindow = QSharedPointer<OpenMediaWindow>::create();
    m_openMediaWindow->resize(640, 480);
    m_openMediaWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_openMediaWindow->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    m_openMediaWindow->close();
}
