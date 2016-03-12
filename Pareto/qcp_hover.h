#ifndef QCP_HOVER_H
#define QCP_HOVER_H

/*!
 * \file qcp_hover.h
 * \brief Custom plot which handles data display
 * \author Pierre-Loup Pissavy
 */

#include "Solutions.h"
#include "qcustomplot.h"

/*!
 * \brief QCustomPlot based plot with data info display when mouse hovers, and area display when a graph is selected
 * \todo Create a QGraphWithData class to make a quicker search of the selected graph, and add a method here to get it.
 */
class QCPHover : public QCustomPlot
{
public:
    QCPHover();
    QCPHover(QSplitter*&);
    ~ QCPHover();

    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void paintInfo();
    void paintHypervolumen();
    void setSolution(Solutions *);
    Solutions * getSolution() const;
    void reset();

private:
    QPoint      cursor_pos_;                //!< Position of the cursor
    QPoint      old_cursor_pos_;            //!< Old position of the cursor
    Solutions * sln_;                       //!< Solutions data
    QCPGraph  * last_selected_graph;        //!< Last selected graph
    QPen        last_selected_graph_pen;    //!< Pen of the last selected graph (to revert when last_selected_graph is not selected anymore)
};

#endif // QCP_HOVER_H
