#include "qcp_hover.h"
#include <string>
#include <iostream>
#include <sstream>

QCPHover::QCPHover() :
    old_cursor_pos_(QPoint(0,0)),
    sln_(nullptr)
{
    addPlottable(new QCPCurve(this->xAxis,this->yAxis));
}

QCPHover::QCPHover(QSplitter*&split) : QCustomPlot(split),
    old_cursor_pos_(QPoint(0,0)),
    sln_(nullptr)
{
    this->addPlottable(new QCPCurve(this->xAxis,this->yAxis));
}

QCPHover::~QCPHover() {}


void QCPHover::setSolution(Solutions *s) {
    sln_ = s;
}

Solutions * QCPHover::getSolution() const {
    return sln_;
}

/**
 * @brief QCPHover::mouseMoveEvent
 * @param event
 */
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
    if (sln_) {
        paintInfo();
    }
}

void QCPHover::paintInfo() {
    QPainter painter(this);
    int radius = 10;

    // /!\ BE CAREFUL WITH INVERSION ON Y AXIS IN PIXELS !!
    FPoint top_left(0,xAxis->pixelToCoord(cursor_pos_.x()-radius),yAxis->pixelToCoord(cursor_pos_.y()-radius)),
           bottom_right(0,xAxis->pixelToCoord(cursor_pos_.x()+radius),yAxis->pixelToCoord(cursor_pos_.y()+radius));

    if (sln_ != nullptr && sln_->getNbPts() > 0) {
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
                brect.moveRight(cursor_pos_.x() - 5);
            }
            if (brect.bottom() > painter.viewport().bottom()) {
                brect.moveBottom(cursor_pos_.y() - 5);
                if (brect.bottom() > painter.viewport().bottom()) {
                    brect.moveBottom(painter.viewport().bottom());
                }
                if (brect.top() < painter.viewport().top()) {
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

void QCPHover::paintHypervolumen()
{
    if (this->selectedGraphs().size() > 0) {
        QCPDataMap * data = this->selectedGraphs()[0]->data();
        QCPCurve * hypervolumen_area = (QCPCurve *) this->plottable(0);

        QVector<double> data_x, data_y;
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMinY());
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMaxY());
        data_x.push_back(data->firstKey());
        data_y.push_back(sln_->getMaxY());
        for (QCPDataMap::iterator itr = data->begin(); itr != data->end(); ++itr) {
            data_x.push_back(itr->key);
            data_y.push_back(itr->value);
        }
        data_x.push_back(sln_->getMaxX());
        data_y.push_back(data->last().value);
        data_x.push_back(sln_->getMaxX());
        data_y.push_back(sln_->getMinY());
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMinY());

        hypervolumen_area->setData(data_x,data_y);
        QPen pen = this->selectedGraphs()[0]->pen();
        hypervolumen_area->setPen(QPen(QColor(0,0,0,0)));
        hypervolumen_area->setBrush(QBrush(QColor(pen.color().red(),pen.color().green(),pen.color().blue(),100)));//this->selectedGraphs()[0]->brush()
        this->plottable(0)->setVisible(true);
        this->replot();
    } else {
        this->plottable(0)->setVisible(false);
    }
}

void QCPHover::reset() {
    xAxis->setLabel(nullptr);
    yAxis->setLabel(nullptr);
    clearGraphs();
    clearItems();
    clearPlottables();
    clearFocus();
    setSolution(nullptr);
    replot();
    addPlottable(new QCPCurve(this->xAxis,this->yAxis));
}
