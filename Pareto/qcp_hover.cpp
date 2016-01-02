#include "qcp_hover.h"

QCPHover::QCPHover() {}

QCPHover::QCPHover(QSplitter*&split) : QCustomPlot(split) {}

QCPHover::~QCPHover() {}

void QCPHover::setSolution(Solutions *s) {
    sln = s;
}

Solutions * QCPHover::getSolution() const {
    return sln;
}

void QCPHover::mouseMoveEvent(QMouseEvent *event) {
    cursor_pos = event->pos();
    replot();
    QCustomPlot::mouseMoveEvent(event);
}

void QCPHover::paintEvent(QPaintEvent *event) {
    QCustomPlot::paintEvent(event);
    paintInfo();
}

void QCPHover::paintInfo() {
    QPainter painter(this);
    FPoint top_left(xAxis->pixelToCoord(cursor_pos.x()-5),yAxis->pixelToCoord(cursor_pos.y()+5)),
           bottom_right(xAxis->pixelToCoord(cursor_pos.x()+5),yAxis->pixelToCoord(cursor_pos.y()-5));
    //QVector<FPoint> pts_in_area = s->findArea(top_left,bottom_right);
    painter.drawText(cursor_pos, "toto");

}
