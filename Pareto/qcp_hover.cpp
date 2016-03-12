#include "qcp_hover.h"
#include <string>
#include <iostream>
#include <sstream>

/*!
 * \brief Default constructor
 */
QCPHover::QCPHover() :
    old_cursor_pos_(QPoint(0,0)),
    sln_(nullptr),
    last_selected_graph(nullptr)
{
    setCursor(Qt::CrossCursor);
    //Add the hypervolumen layer
    addPlottable(new QCPCurve(this->xAxis,this->yAxis));
    this->plottable(0)->setPen(QPen(QColor(0,0,0,0)));
}

/*!
 * \brief Constructor with parent
 * \param split parent of the QCPHover
 */
QCPHover::QCPHover(QSplitter*&split) : QCustomPlot(split),
    old_cursor_pos_(QPoint(0,0)),
    sln_(nullptr),
    last_selected_graph(nullptr)
{
    setCursor(Qt::CrossCursor);
    //Add the hypervolumen layer
    this->addPlottable(new QCPCurve(this->xAxis,this->yAxis));
    this->plottable(0)->setPen(QPen(QColor(0,0,0,0)));
}

/*!
 * \brief Destructor
 */
QCPHover::~QCPHover() {}

/*!
 * \brief set the associated solutions data
 * \param s pointer on the solutions data
 */
void QCPHover::setSolution(Solutions *s) {
    sln_ = s;
}

/*!
 * \brief get the associated solutions data
 * \return pointer on the solutions data
 */
Solutions * QCPHover::getSolution() const {
    return sln_;
}

/**
 * \brief called when the mouse has moved
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

/*!
 * \brief override to add info over the graph when mouse hover
 */
void QCPHover::paintEvent(QPaintEvent *event) {
    QCustomPlot::paintEvent(event);
    if (sln_) {
        paintInfo();
    }
}

/*!
 * \brief Show info (i.e coordinates and comments) about the points near the mouse pointer
 */
void QCPHover::paintInfo() {
    QPainter painter(this);
    int radius = 10;

    //Get the position of the cursor in the real graph
    // /!\ BE CAREFUL WITH INVERSION ON Y AXIS IN PIXELS !!
    FPoint top_left(0,xAxis->pixelToCoord(cursor_pos_.x()-radius),yAxis->pixelToCoord(cursor_pos_.y()-radius)),
           bottom_right(0,xAxis->pixelToCoord(cursor_pos_.x()+radius),yAxis->pixelToCoord(cursor_pos_.y()+radius));

    if (sln_ != nullptr && sln_->getNbPts() > 0) {
        //Gather all the points in the area
        FPointPtrv pts_in_area = sln_->findPointsInArea(top_left,bottom_right);
        if (! pts_in_area.empty()) {
            //Text
            std::string pop_up;
            //Add the first item
            FPointPtrv::iterator i = pts_in_area.begin();
            pop_up.append(" ");
            pop_up.append((*i)->toString());
            i++;
            //Add the other items
            int j = 1; //Number of items in list
            while(i != pts_in_area.end() && j < 10) {
                pop_up += '\n';
                pop_up.append(" ");
                pop_up.append((*i)->toString());
                j++;
                i++;
            }
            //If list is shorter than the pts_in_area, then append ellipsis
            if (j == 10 && i != pts_in_area.end()) {
                pop_up += "\n ...";
            }
            //Shrinked area for the view box
            QRectF rect(cursor_pos_,QSize(350,250));
            //Box that contains the text
            QRectF brect = painter.boundingRect(rect,Qt::TextWordWrap,QString(pop_up.c_str()));
            //Check box visibility
            //Default position: below and right from the pointer position
            //Check horizontal position : if there is enough space between the pointer and the right side
            if (brect.right() > painter.viewport().right()) {
                brect.moveRight(cursor_pos_.x() - 5);
            }
            //Check vertical position
            if (brect.bottom() > painter.viewport().bottom()) {
                brect.moveBottom(cursor_pos_.y() - 5);
                //Check that the bottom of the box is visible
                if (brect.bottom() > painter.viewport().bottom()) {
                    brect.moveBottom(painter.viewport().bottom());
                }
                //Make sure that the top of the box is visible
                if (brect.top() < painter.viewport().top()) {
                    brect.moveTop(painter.viewport().top());
                }
            }
            // Display: gray box with text
            painter.setBrush(QColor(Qt::gray).lighter(150));
            painter.drawRect(brect);
            painter.drawText(brect,Qt::AlignLeft,QString(pop_up.c_str()));
        }
    }
}

/*!
 * \brief Show the hypervolumen area when a graph is selected, hide it otherwise
 */
void QCPHover::paintHypervolumen()
{
    //Revoke color change on last selected graph
    if (last_selected_graph != nullptr) {
        QCPScatterStyle style = last_selected_graph->scatterStyle();
        style.setPen(last_selected_graph_pen);
        style.setBrush(QBrush(last_selected_graph_pen.color()));
        last_selected_graph->setScatterStyle(style);
    }
    //Hypervolumen area
    if (this->selectedGraphs().size() > 0) {
        QCPCurve * hypervolumen_area = (QCPCurve *) this->plottable(0);
        last_selected_graph = this->selectedGraphs()[0];

        //save the pen for reverting it when graph is not selected anymore
        last_selected_graph_pen = last_selected_graph->pen();

        //Update the color of the selected_graph
        QCPScatterStyle style = last_selected_graph->scatterStyle();
        style.setPen(QPen(QColor(0,40,240)));
        style.setBrush(QBrush(QColor(0,40,240)));
        last_selected_graph->setScatterStyle(style);

        //Adding points for the hypervolumen area
        QCPDataMap * data = last_selected_graph->data();
        QVector<double> data_x, data_y;
        //Start at bottom left
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMinY());
        //Top left
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMaxY());
        //Above the first point, on the top line
        data_x.push_back(data->firstKey());
        data_y.push_back(sln_->getMaxY());
        //Follow the graph
        for (QCPDataMap::iterator itr = data->begin(); itr != data->end(); ++itr) {
            data_x.push_back(itr->key);
            data_y.push_back(itr->value);
        }
        //Projection on the right limit
        data_x.push_back(sln_->getMaxX());
        data_y.push_back(data->last().value);
        //Bottom right
        data_x.push_back(sln_->getMaxX());
        data_y.push_back(sln_->getMinY());
        //Bottom left -> cycle
        data_x.push_back(sln_->getMinX());
        data_y.push_back(sln_->getMinY());

        //Apply changes to the graph
        hypervolumen_area->setData(data_x,data_y);
        hypervolumen_area->setBrush(QBrush(QColor(last_selected_graph_pen.color().red(),
                                                  last_selected_graph_pen.color().green(),
                                                  last_selected_graph_pen.color().blue(),
                                                  100)));
        //Show the graph
        hypervolumen_area->setVisible(true);
        this->replot();
    } else {
        //Hide the graph
        this->plottable(0)->setVisible(false);
    }
}

/*!
 * \brief Reinitialize the graph
 */
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
