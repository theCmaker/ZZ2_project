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

double Front::max_distance() const
{
    return max_distance_;
}

void Front::setMax_distance(double max_distance)
{
    max_distance_ = max_distance;
}

double Front::sum_distance() const
{
    return sum_distance_;
}

void Front::setSum_distance(double sum_distance)
{
    sum_distance_ = sum_distance;
}

double Front::mean_distance() const
{
    return mean_distance_;
}

void Front::setMean_distance(double mean_distance)
{
    mean_distance_ = mean_distance;
}

void Front::compute_distances()
{
    double dist;
    max_distance_ = 0.;
    sum_distance_ = 0.;
    mean_distance_ = 0.;
    if (pts_.size() > 1) {
        for (PolyLine::iterator itr = pts_.begin(); itr != pts_.end() - 1; ++itr) {
            dist = (*(itr+1))->getX() - (*itr)->getX() + (*itr)->getY() - (*(itr+1))->getY();
            sum_distance_ += dist;
            if (dist > max_distance_) {
                max_distance_ = dist;
            }
        }
        mean_distance_ = sum_distance_ / (double) (pts_.size() - 1);
    }
}



