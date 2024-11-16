#include "tributton.h"

int TriangleButton::shrinkALittle(int value) {
    // change a value a little
    return value * 999 / 1000;
}

void TriangleButton::drawTriangle() {
    QPainter painter(this);

    // Calculate triangle coordinates
    int xMax = this->width();
    int yMax = this->height();
    //triangle.setPoints(0, 3, edge/2, edge);
    m_triangle.setPoint(0, 0, shrinkALittle(yMax));
    m_triangle.setPoint(1, shrinkALittle(xMax), 0);
    m_triangle.setPoint(2, shrinkALittle(xMax), shrinkALittle(yMax));

    // Draw the triangle
    painter.drawPolygon(m_triangle);
}

void TriangleButton::paintEvent(QPaintEvent *e) {
    drawTriangle();
    setBrushRange(m_pointBrush);
}

void TriangleButton::setBrushRange(const QPoint& mousePoint) {
    if (m_pointBrush.isNull()) {
        return;
    }
    QPainter painter(this);
    QPolygon triangle{POINTS_COUNT};
    triangle.setPoint(0, m_triangle.point(0).x(), m_triangle.point(0).y());

    int xMax = this->width();
    int yMax = this->height();
    triangle.setPoint(1, mousePoint.x(),
        // 100 / 980 in order to correct factor
        m_triangle.point(0).y()  * 1000 / 980 - mousePoint.x() * yMax / xMax);
    triangle.setPoint(2, mousePoint.x(), m_triangle.point(0).y());

    painter.setBrush(m_color);
    painter.drawPolygon(triangle);
}

void TriangleButton::mousePressEvent(QMouseEvent *event) {
    int xMax = this->width();
    int yMax = this->height();
    QPoint point = event->pos();
    if ((point.y() >= yMax - point.x() * yMax / xMax) &&
        point.x() < xMax && m_color == Qt::green) {
        m_pointBrush = point;
        update();
        double percent = (double)point.x() / (double)xMax;
        emit voiceChangeNeeded(percent);
    }
}

void TriangleButton::setColor(Qt::GlobalColor color) {
    m_color = color;
    update();
}
