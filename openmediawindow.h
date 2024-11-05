#ifndef OPENMEDIAWINDOW_H
#define OPENMEDIAWINDOW_H

#include <QWidget>
#include "ui_openmediawindow.h"

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
    Ui::OpenMediaWindow* m_ui;
};

#endif // OPENMEDIAWINDOW_H
