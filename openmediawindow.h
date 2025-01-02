#ifndef OPENMEDIAWINDOW_H
#define OPENMEDIAWINDOW_H

#include "ui_openmediawindow.h"

#include <QWidget>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class OpenMediaWindow;
}
QT_END_NAMESPACE

class OpenMediaWindow : public QWidget
{
    Q_OBJECT

public:
    OpenMediaWindow(QWidget *parent = nullptr);
    ~OpenMediaWindow();

private:
    void loadSettings();
    void saveUrlSettings();
    void saveFilePathSettings();

signals:
    void playUrlNeeded(QString playUrl);

public slots:
    void onPlayUrlPushButton();
    void onCancelUrlPushButton();

private:
    Ui::OpenMediaWindow* m_ui{nullptr};

    QStringList m_fileStrList{};
    QSharedPointer<QStringListModel> model{nullptr};

    QString m_playUrl{};
    QString m_playFilePath{};
};

#endif // OPENMEDIAWINDOW_H
