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

signals:
    void playUrlNeeded(QString playUrl);
    void playFileNeeded(QQueue<QString> queFile);

public slots:
    void onPlayUrlPushButton();
    void onPlayFilePushButton();
    void onAddPushButton();
    void onDelPushButton();


private:
    Ui::OpenMediaWindow* m_ui{nullptr};

    QStringList m_fileStrList{};
    QSharedPointer<QStringListModel> model{nullptr};
};

#endif // OPENMEDIAWINDOW_H
