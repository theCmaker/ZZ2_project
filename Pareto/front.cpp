#include "front.h"

Front::Front() {}

Front::~Front() {}

PolyLine & Front::pts()
{
    return pts_;
}

const PolyLine & Front::pts() const
{
    return pts_;
}

void Front::setPts(const PolyLine &pts)
{
    pts_ = pts;
}

PolyLine::iterator Front::begin()
{
    return pts_.begin();
}

PolyLine::iterator Front::end()
{
    return pts_.end();
}
QColor Front::lineColor() const
{
    return lineColor_;
}

void Front::setLineColor(const QColor &lineColor)
{
    lineColor_ = lineColor;
}
QColor Front::pointColor() const
{
    return pointColor_;
}

void Front::setPointColor(const QColor &pointColor)
{
    pointColor_ = pointColor;
}
PointShape Front::pointStyle() const
{
    return pointStyle_;
}

void Front::setPointStyle(const PointShape &pointStyle)
{
    pointStyle_ = pointStyle;
}
double Front::hypervolumen() const
{
    return hypervolumen_;
}

void Front::setHypervolumen(double hypervolumen)
{
    hypervolumen_ = hypervolumen;
}
