#include "openmediawindow.h"

OpenMediaWindow::OpenMediaWindow(QWidget *parent):
    QWidget(parent), m_ui(new Ui::OpenMediaWindow) {
    m_ui->setupUi(this);

    m_ui->playComboBox->addItem("播放(P)");
    m_ui->playComboBox->setCurrentText(QString("播放(P)"));
}

OpenMediaWindow::~OpenMediaWindow()
{
    delete m_ui;
}
