#ifndef FRONT_H
#define FRONT_H

#include "Point.h"
#include <vector>
#include "qcustomplot.h"

typedef Point<float> FPoint;
typedef QCPScatterStyle::ScatterShape PointShape;
typedef std::vector<FPoint *> PolyLine;

class Front
{
public:
    Front();
    ~Front();

    PolyLine & pts();
    const PolyLine & pts() const;
    void setPts(const PolyLine &pts);

    PolyLine::iterator begin();
    PolyLine::iterator end();

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

    QColor pointColor() const;
    void setPointColor(const QColor &pointColor);

    PointShape pointStyle() const;
    void setPointStyle(const PointShape &pointStyle);

    double hypervolumen() const;
    void setHypervolumen(double hypervolumen);

    double max_distance() const;
    void setMax_distance(double max_distance);

    double sum_distance() const;
    void setSum_distance(double sum_distance);

    double mean_distance() const;
    void setMean_distance(double mean_distance);

    void compute_distances();

private:
    PolyLine    pts_;
    QColor      lineColor_;
    QColor      pointColor_;
    PointShape  pointStyle_;
    double      hypervolumen_;
    double      mean_distance_;
    double      max_distance_;
    double      sum_distance_;
};

#endif // FRONT_H
