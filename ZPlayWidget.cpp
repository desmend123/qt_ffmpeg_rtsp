#include "ZPlayWidget.h"

ZPlayWidget::ZPlayWidget(QWidget *parent) : QWidget(parent) {

}

int ZPlayWidget::setImage(QImage image) {
    m_image = image;
    return 0;
}

void ZPlayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(rect(), m_image);
}
