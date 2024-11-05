#ifndef OPENMEDIA_H
#define OPENMEDIA_H

#include <QWidget>
#include "ui_openmedia.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class OpenMedia;
}
QT_END_NAMESPACE

class OpenMedia : public QWidget
{
    Q_OBJECT

public:
    OpenMedia(QWidget *parent = nullptr);
    ~OpenMedia();

private:
    Ui::OpenMedia* m_ui;
};

#endif // OPENMEDIA_H
