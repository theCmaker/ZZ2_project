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

double Front::max_spacing() const
{
    return max_spacing_;
}

void Front::setMax_spacing(double max_spacing)
{
    max_spacing_ = max_spacing;
}

double Front::length() const
{
    return length_;
}

void Front::setLength(double length)
{
    length_ = length;
}

double Front::mean_spacing() const
{
    return mean_spacing_;
}

void Front::setMean_spacing(double mean_spacing)
{
    mean_spacing_ = mean_spacing;
}

void Front::compute_stats(double xmin, double xmax, double ymin, double ymax)
{
    max_spacing_ = 0.;
    length_ = 0.;
    mean_spacing_ = 0.;
    double delta_x = xmax - xmin;
    double delta_y = ymax - ymin;
    double dist_x;
    double dist_y;
    double sum_dist_x = 0.;
    double sum_dist_y = 0.;
    if (pts_.size() > 1) {
        for (PolyLine::iterator itr = pts_.begin(); itr != pts_.end() - 1; ++itr) {
            dist_x = (*(itr+1))->getX() - (*itr)->getX();
            dist_y = (*itr)->getY() - (*(itr+1))->getY();
            sum_dist_x += dist_x;
            sum_dist_y += dist_y;
            if (dist_x/delta_x + dist_y/delta_y > max_spacing_) {
                max_spacing_ = dist_x/delta_x + dist_y/delta_y;
            }
        }
        length_ = sum_dist_x / delta_x + sum_dist_y / delta_y;
        mean_spacing_ = length_ / (double) (pts_.size() - 1);

        //Hypervolumen
        hypervolumen_ = delta_x * delta_y;
        PolyLine::iterator i = pts_.begin();

        // Compute until penultimate point
        while (i != pts_.end()-1) {
            // Remove rectangle above current point till next point
            hypervolumen_ -= ((*(i+1))->getX() - (*i)->getX()) * (ymax - (*i)->getY());
            ++i;
        }

        // Last point -> rectangle computed with x_max_ as right side abscissa
        // (projection on line x = x_max_)
        hypervolumen_ -= (xmax - (*i)->getX()) * (ymax - (*i)->getY());

        //Normalize
        hypervolumen_ = hypervolumen_ / (delta_x * delta_y);
    }

}



