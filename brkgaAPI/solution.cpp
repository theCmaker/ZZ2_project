#include "solution.h"
#include "data.h"
#include "geometry.h"
#include <algorithm>
size_t Solution::computeNbTotalPoints ()
{
  // build the polygon out of the hull
  Point * polygon = new Point[hull.size()];

  for (size_t i = 0; i < hull.size(); ++i)
  {
    polygon[i] = data.points[hull[i]];
  }


  size_t cpt = 0;
  for (size_t i = 0; i < data.points.size(); ++i)
  {
    if (pointInPolygon(data.points[i], polygon, nbPoints) != 0 
        || std::find(&polygon[0],&polygon[hull.size()],data.points[i]) != &polygon[hull.size()]) { // NOTE: the winding number is exact only on simple polygons
      //std::cout << "Point " << data.points[i] << " is in the hull" << std::endl;
      ++cpt;
    }
  }
  nbTotalPoints = cpt;

  delete [] polygon;
  return cpt;
}


double Solution::computePerimeter ()
{
  double peri = 0.0;
  for (size_t i = 0; i < nbPoints; ++i)
  {
    peri += computeDistance(data.points[hull[i]], data.points[hull[i + 1]]);
  }
  perimeter = peri;
  return peri;
}


double Solution::computeArea ()
{
  // build the polygon out of the hull
  Point * polygon = new Point[hull.size()];
  for (size_t i = 0; i < hull.size(); ++i)
  {
    polygon[i] = data.points[hull[i]];
  }

  area = computePolygonArea(polygon, nbPoints);

  delete [] polygon;

  return area;
}


std::ostream & operator<< (std::ostream & os, const Solution & s)
{
  os << s.nbTotalPoints << " points covered, \t" << s.area << " area, \t" << s.nbPoints << " vertices, \t" << s.perimeter << " perimeter" << std::endl;
  os << "Hull: ";
  for (unsigned i = 0; i < s.hull.size() - 1; ++i) {
    os << s.data.points[s.hull[i]] << " -- ";
  }
  os << "cycle;";

  return os;
}

