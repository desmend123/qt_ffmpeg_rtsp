#include "openmediawindow.h"

#include <QDir>
#include <QFileDialog>
#include <QListView>
#include <QMessageBox>
#include <QQueue>
#include <QSettings>

OpenMediaWindow::OpenMediaWindow(QWidget *parent):
    QWidget(parent), m_ui(new Ui::OpenMediaWindow) {
    m_ui->setupUi(this);

    loadSettings();
    m_ui->comboBox->setCurrentText(m_playUrl);

    connect(m_ui->playUrlPushButton, SIGNAL(clicked()),
            this, SLOT(onPlayUrlPushButton()));
    connect(m_ui->cancelUrlPushButton, SIGNAL(clicked()),
            this, SLOT(onCancelUrlPushButton()));
}

OpenMediaWindow::~OpenMediaWindow()
{
    delete m_ui;
}

void OpenMediaWindow::loadSettings() {
    QSettings settings("Qt", "MultimeidaPlayer", this);
    m_playUrl = settings.value("playUrl", "").toString();
    m_playFilePath = settings.value("playFilePath", "").toString();
}
void OpenMediaWindow::saveUrlSettings() {
    QSettings settings("Qt", "MultimeidaPlayer", this);
    settings.setValue("playUrl", m_playUrl);
}

void OpenMediaWindow::saveFilePathSettings() {
    QSettings settings("Qt", "MultimeidaPlayer", this);
    settings.setValue("playFilePath", m_playFilePath);
}

void OpenMediaWindow::onPlayUrlPushButton() {
    QString playUrl = m_ui->comboBox->currentText();
    QRegExp regex("://");
    if (regex.indexIn(playUrl) == -1) {
        QMessageBox::warning(this, tr("Warning"),
            QString(tr("%1 is an invalid video streaming!")).arg(playUrl));
        return;
    }
    if (m_ui->comboBox->findText(playUrl) == -1)
    {
        m_ui->comboBox->addItem(playUrl);
    }

    m_playUrl = playUrl;
    saveUrlSettings();
    emit playUrlNeeded(m_playUrl);
}

void OpenMediaWindow::onCancelUrlPushButton() {
    close();
}
