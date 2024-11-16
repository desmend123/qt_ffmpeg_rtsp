#ifndef TRIEPROGRESSBAR_H
#define TRIEPROGRESSBAR_H

#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QPoint>
#include <QWidget>

int const POINTS_COUNT = 3;

class TriangleButton : public QPushButton {
    Q_OBJECT
public:
    TriangleButton(QWidget *parent = 0) : QPushButton(parent) {

    }

    void drawTriangle();
    void setBrushRange(const QPoint& point);
    void setColor(Qt::GlobalColor color);

private:
    int shrinkALittle(int value);
protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void voiceChangeNeeded(double percent);

private:
    QPolygon m_triangle{POINTS_COUNT};
    QPoint m_pointBrush{};

    Qt::GlobalColor m_color{Qt::green};
};


#endif // TRIEPROGRESSBAR_H
