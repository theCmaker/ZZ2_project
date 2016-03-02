#ifndef QCP_HOVER_H
#define QCP_HOVER_H

#include "Solutions.h"
#include "qcustomplot.h"

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
    QPoint      cursor_pos_;
    QPoint      old_cursor_pos_;
    Solutions  *sln_;
};

#endif // QCP_HOVER_H
