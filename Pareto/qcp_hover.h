#ifndef QCP_HOVER_H
#define QCP_HOVER_H

#include "qcustomplot.h"
#include "Solutions.h"

class QCPHover : public QCustomPlot
{
public:
    QCPHover();
    QCPHover(QSplitter*&);
    ~ QCPHover();

    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void paintInfo();
    void setSolution(Solutions *);
    Solutions * getSolution() const;

private:
    QPoint cursor_pos;
    Solutions *sln;
};

#endif // QCP_HOVER_H
