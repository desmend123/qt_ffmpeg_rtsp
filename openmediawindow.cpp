#include "openmediawindow.h"

#include <QDir>
#include <QFileDialog>
#include <QListView>
#include <QQueue>
#include <QMessageBox>

OpenMediaWindow::OpenMediaWindow(QWidget *parent):
    QWidget(parent), m_ui(new Ui::OpenMediaWindow) {
    m_ui->setupUi(this);

    connect(m_ui->playUrlPushButton, SIGNAL(clicked()),
            this, SLOT(onPlayUrlPushButton()));
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

void OpenMediaWindow::onPlayUrlPushButton() {
    QString playUrl = m_ui->comboBox->currentText();
    emit playUrlNeeded(playUrl);
}

void OpenMediaWindow::onPlayFilePushButton() {
    QAbstractItemModel *modelPtr = m_ui->fileListView->model();
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
        tr("Open video file"), "", tr("video") + " (*.mp4 *.avi *.*)");
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
