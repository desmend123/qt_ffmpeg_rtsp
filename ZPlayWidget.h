#ifndef ZPLAYWIDGET_H
#define ZPLAYWIDGET_H

#include <QPainter>
#include <QWidget>

class ZPlayWidget : public QWidget {
    Q_OBJECT
public:
    ZPlayWidget(QWidget *parent = nullptr);
    int setImage(QImage image);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QImage m_image{};
};

#endif // ZPLAYWIDGET_H
