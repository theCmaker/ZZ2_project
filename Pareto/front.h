#ifndef FRONT_H
#define FRONT_H

/*!
 * \file front.h
 * \brief 2D line made of Point s, with graphical information
 * \author Pierre-Loup Pissavy
 */

#include "Point.h"
#include <vector>
#include "qcustomplot.h"

typedef Point<float> FPoint;
typedef QCPScatterStyle::ScatterShape PointShape;
typedef std::vector<FPoint *> PolyLine;

/*!
 * \brief Front with graphical details.
 */

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

    double max_spacing() const;
    void setMax_spacing(double max_spacing);

    double length() const;
    void setLength(double length);

    double mean_spacing() const;
    void setMean_spacing(double mean_spacing);

    void compute_stats(double xmin, double xmax, double ymin, double ymax);

private:
    PolyLine    pts_;           //!< the point(s) that make(s) the front
    QColor      lineColor_;     //!< the color used to print the line
    QColor      pointColor_;    //!< the color used to print the points
    PointShape  pointStyle_;    //!< the shape of the points
    double      hypervolumen_;  //!< hypervolumen value
    double      mean_spacing_;  //!< mean spacing value
    double      max_spacing_;   //!< max spacing value
    double      length_;        //!< length of the front
};

#endif // FRONT_H
