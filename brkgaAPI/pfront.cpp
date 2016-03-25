#include "pfront.h"

/*!
 * \brief Constructor
 *
 * Inits the front
 */
PFront::PFront() :
    hypervolumen_(0.),
    mean_spacing_(0.),
    max_spacing_(0.),
    length_(0.)
    {}

/*!
 * \brief Destructor
 */
PFront::~PFront() {}

/*!
 * \brief points vector
 * \return the sorted points that compose the front
 * \note it is a vector of pointers to Points.
 */
PolyLine & PFront::pts()
{
    return pts_;
}

/*!
 * \brief points vector
 * \return the sorted points that compose the front
 * \note it is a vector of pointers to Points.
 */
const PolyLine & PFront::pts() const
{
    return pts_;
}

/*!
 * \brief updates the points
 * \param pts new points of the front
 * \note \a pts must be a vector of pointers to Points.
 */
void PFront::setPts(const PolyLine &pts)
{
    pts_ = pts;
}

/*!
 * \brief Iterator on the points vector
 * \return iterator on the points vector
 */
PolyLine::iterator PFront::begin()
{
    return pts_.begin();
}

/*!
 * \brief End iterator on the points vector
 * \return end iterator of the points vector
 */
PolyLine::iterator PFront::end()
{
    return pts_.end();
}

/*!
 * \brief Color of the line between the points
 * \return the line color in QColor object
 */
/*
QColor PFront::lineColor() const
{
    return lineColor_;
}
*/
/*!
 * \brief Define the line color
 * \param lineColor color of the line between the points, in QColor object
 */
/*
void PFront::setLineColor(const QColor &lineColor)
{
    lineColor_ = lineColor;
}
*/
/*!
 * \brief Color of the points
 * \return the points color in QColor object
 */
/*
QColor PFront::pointColor() const
{
    return pointColor_;
}
*/
/*!
* \brief Define the point color
* \param pointColor color of the points, in QColor object
*/
/*
void PFront::setPointColor(const QColor &pointColor)
{
    pointColor_ = pointColor;
}
*/
/*!
 * \brief Shape of the points of this front
 * \return shape as defined in QCustomPlot
 */
/*
PointShape PFront::pointStyle() const
{
    return pointStyle_;
}
*/

/*!
 * \brief Define the shape of the points
 * \param pointStyle shape of the points as defined in QCustomPlot
 */
/*
void PFront::setPointStyle(const PointShape &pointStyle)
{
    pointStyle_ = pointStyle;
}
*/
/*!
 * \brief Hypervolumen of the front
 * \return hypervolumen value
 * \warning compute_stats() must have been called before
 * \see compute_stats
 * \note The value is normalized. Between 0 and 1.
 */
double PFront::hypervolumen() const
{
    return hypervolumen_;
}

/*!
 * \brief Max spacing of the front
 * \return max spacing value
 * \warning compute_stats() must have been called before
 * \see compute_stats
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 */
double PFront::max_spacing() const
{
    return max_spacing_;
}

/*!
 * \brief Length of the front
 * \return length value, i.e sum of all spacing values.
 * \note The value is normalized. Between 0 and 2.
 * \warning compute_stats() must have been called before
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 * \see compute_stats
 */
double PFront::length() const
{
    return length_;
}

/*!
 * \brief Mean spacing of the front
 * \return mean spacing value
 * \warning compute_stats() must have been called before
 * \note lengths are computed with taxicab geometry (Manhattan distance)
 * \see compute_stats
 */
double PFront::mean_spacing() const
{
    return mean_spacing_;
}

/*!
 * \brief Compute all the stats of the front, i.e. hypervolumen, mean and max spacing, and length.
 * \param xmin lowest first objective value in the global graph
 * \param xmax highest first objective value in the global graph
 * \param ymin lowest second objective value in the global graph
 * \param ymax highest second objective value in the global graph
 * \note Parameters are provided to normalize the stats.
 * \note Lengths are computed with taxicab geometry (Manhattan distance)
 */
void PFront::compute_stats(double xmin, double xmax, double ymin, double ymax)
{
    double delta_x = xmax - xmin;
    double delta_y = ymax - ymin;
    double dist_x;
    double dist_y;
    double sum_dist_x = 0.;
    double sum_dist_y = 0.;
    max_spacing_ = 0.;
    length_ = 0.;
    mean_spacing_ = 0.;
    hypervolumen_ = 0.;

    //Spacings
    if (pts_.size() > 1) {
        for (PolyLine::iterator itr = pts_.begin(); itr != pts_.end() - 1; ++itr) {
            dist_x = (*(itr+1))->getX() - (*itr)->getX();
            dist_y = (*itr)->getY() - (*(itr+1))->getY();
            sum_dist_x += dist_x;
            sum_dist_y += dist_y;
            //update the max_spacing_ if necessary
            if (dist_x/delta_x + dist_y/delta_y > max_spacing_) {
                max_spacing_ = dist_x/delta_x + dist_y/delta_y;
            }
        }
        length_ = sum_dist_x / delta_x + sum_dist_y / delta_y;
        mean_spacing_ = length_ / (double) (pts_.size() - 1);

    }

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



