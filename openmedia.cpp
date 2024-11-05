#include "openmedia.h"

OpenMedia::OpenMedia(QWidget *parent):
    QWidget(parent), m_ui(new Ui::OpenMedia) {
    m_ui->setupUi(this);

    m_ui->playComboBox->addItem("播放(p)");
    m_ui->playComboBox->setCurrentText(QString("播放(p)"));
}

OpenMedia::~OpenMedia()
{
    delete m_ui;
}
