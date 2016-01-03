#include "qcp_hover.h"
#include <string>
#include <iostream>
#include <sstream>

QCPHover::QCPHover() {}

QCPHover::QCPHover(QSplitter*&split) : QCustomPlot(split) {}

QCPHover::~QCPHover() {}

void QCPHover::setSolution(Solutions *s) {
    sln_ = s;
}

Solutions * QCPHover::getSolution() const {
    return sln_;
}

void QCPHover::mouseMoveEvent(QMouseEvent *event) {
    cursor_pos_ = event->pos();
    replot();
    QCustomPlot::mouseMoveEvent(event);
}

void QCPHover::paintEvent(QPaintEvent *event) {
    QCustomPlot::paintEvent(event);
    paintInfo();
}

void QCPHover::paintInfo() {
    QPainter painter(this);
    int radius = 10;

    // /!\ BE CAREFUL WITH INVERSION ON Y AXIS IN PIXELS !!
    FPoint top_left(xAxis->pixelToCoord(cursor_pos_.x()-radius),yAxis->pixelToCoord(cursor_pos_.y()-radius)),
           bottom_right(xAxis->pixelToCoord(cursor_pos_.x()+radius),yAxis->pixelToCoord(cursor_pos_.y()+radius));
    if (sln_) {
        this->setCursor(Qt::CrossCursor);
        FPointv * pts_in_area = sln_->findPointsInArea(top_left,bottom_right);
        if (! pts_in_area->empty()) {
            std::string pop_up;
            for(FPointv::iterator i = pts_in_area->begin(); i != pts_in_area->end(); ++i) {
                pop_up.append(i->toString());
                pop_up += '\n';
            }
            painter.drawText(cursor_pos_, pop_up.c_str());
        }
    }

}
