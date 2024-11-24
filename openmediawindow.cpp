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
    connect(m_ui->cancelFilePushButton, SIGNAL(clicked()),
            this, SLOT(onCancelFilePushButton()));
    connect(m_ui->playFilePushButton, SIGNAL(clicked()),
            this, SLOT(onPlayFilePushButton()));
    connect(m_ui->addPushButton, SIGNAL(clicked()),
            this, SLOT(onAddPushButton()));
    connect(m_ui->delPushButton, SIGNAL(clicked()),
            this, SLOT(onDelPushButton()));
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

void OpenMediaWindow::onCancelFilePushButton() {
    close();
}

void OpenMediaWindow::onPlayFilePushButton() {
    QAbstractItemModel *modelPtr = m_ui->fileListView->model();
    if (!modelPtr) {
        return;
    }
    QQueue<QString> playQueue{};
    for (int i = 0; i < modelPtr->rowCount(); ++i) {
        QModelIndex index = modelPtr->index(i, 0);
        QString itemText = modelPtr->data(index, Qt::DisplayRole).toString();
        playQueue.push_back(itemText);
    }

    emit playFileNeeded(playQueue);
}

void OpenMediaWindow::onAddPushButton() {
    QString fileName = QFileDialog::getOpenFileName(nullptr,
        tr("Open video file"), m_playFilePath, tr("video") +
        " (*.mp4 *.avi *.*)");

    m_playFilePath  = fileName;
    saveFilePathSettings();

    if(m_fileStrList.contains(fileName)) {
        QMessageBox::warning(this, tr("Warning"),
            QString(tr("%1 is already in the list!")).arg(fileName));
        return;
    }
    m_fileStrList.append(fileName);
    model = QSharedPointer<QStringListModel>::create(m_fileStrList);
    m_ui->fileListView->setModel(model.data());
    m_ui->fileListView->show();
}

void OpenMediaWindow::onDelPushButton() {
    QModelIndexList selected = m_ui->fileListView->selectionModel()->selectedIndexes();
    for (int i = 0; i < selected.size(); ++i) {
        QModelIndex index = selected.at(i);
        QString str = index.data().toString();
        m_fileStrList.removeOne(str);
    }
    model->setStringList(m_fileStrList);
    m_ui->fileListView->setModel(model.data());
    m_ui->fileListView->show();
}
