#include "qcp_hover.h"
#include <string>
#include <iostream>
#include <sstream>

QCPHover::QCPHover() {
    old_cursor_pos_ = QPoint(0,0);
}

QCPHover::QCPHover(QSplitter*&split) : QCustomPlot(split) {}

QCPHover::~QCPHover() {}

void QCPHover::setSolution(Solutions *s) {
    sln_ = s;
}

Solutions * QCPHover::getSolution() const {
    return sln_;
}

void QCPHover::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() != Qt::NoButton) {
        QCustomPlot::mouseMoveEvent(event);
    } else {
        cursor_pos_ = event->pos();
        if (abs(old_cursor_pos_.x() - cursor_pos_.x()) > 5 || abs(old_cursor_pos_.y() - cursor_pos_.y()) > 5) {
            if (abs(old_cursor_pos_.x() - cursor_pos_.x()) < 10 || abs(old_cursor_pos_.y() - cursor_pos_.y()) < 10) {
                replot();
            }
            old_cursor_pos_ = cursor_pos_;
        }
    }
}

void QCPHover::paintEvent(QPaintEvent *event) {
    QCustomPlot::paintEvent(event);
    paintInfo();
}

void QCPHover::paintInfo() {

    QPainter painter(this);
    int radius = 10;

    // /!\ BE CAREFUL WITH INVERSION ON Y AXIS IN PIXELS !!
    FPoint top_left(0,xAxis->pixelToCoord(cursor_pos_.x()-radius),yAxis->pixelToCoord(cursor_pos_.y()-radius)),
           bottom_right(0,xAxis->pixelToCoord(cursor_pos_.x()+radius),yAxis->pixelToCoord(cursor_pos_.y()+radius));

    if (sln_) {
        this->setCursor(Qt::CrossCursor);
        FPointPtrv * pts_in_area = sln_->findPointsInArea(top_left,bottom_right);
        if (! pts_in_area->empty()) {
            std::string pop_up;
            bool first = true;
            int j = 0;
            FPointPtrv::iterator i;
            for(i = pts_in_area->begin(); i != pts_in_area->end() && j < 10; ++i) {
                if (!first) {
                    pop_up += '\n';
                } else {
                    first = false;
                }
                pop_up.append((*i)->toString());
                j++;
            }
            if (j == 10 && i != pts_in_area->end()) {
                pop_up += '\n';
                pop_up += "...";
            }
            QRectF rect(cursor_pos_,QSize(350,220));
            QRectF brect = painter.boundingRect(rect,Qt::AlignLeft,QString(pop_up.c_str()));
            //Check box visibility
            if (brect.right() > painter.viewport().right()) {
                brect.moveRight(painter.viewport().right());
            }
            if (brect.bottom() > painter.viewport().bottom()) {
                brect.moveBottom(painter.viewport().bottom());
                if (brect.bottom() > painter.viewport().bottom()) {
                    brect.moveTop(painter.viewport().top());
                }
            }
            //    if (brect.moveTopRight())
            // Display: gray box with text
            painter.setBrush(QColor(Qt::gray).lighter(150));
            painter.drawRoundedRect(brect,8,8);
            painter.drawText(brect,Qt::AlignLeft,QString(pop_up.c_str()));
        }
    }
}
