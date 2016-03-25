#include <fstream>      // std::ifstream  
#include <cstdlib>      // exit
#include <numeric>      // iota
#include <algorithm>    // find
#include "data.h"
#include "front.h"
#include "solution.h"

Data::Data () {}

Data::Data (Data &other) {
    points = other.points;
}

Data::Data (const std::string & filename)
{
  std::ifstream is(filename.c_str());

  // safety check
  if (!is.is_open())
  {
    std::cerr << "error: unable to open file " << filename << std::endl;
    exit(1);
  }

  // read the data
  size_t nb_points;
  is >> nb_points;
  for (unsigned i = 0; i < nb_points; ++i)
  {
    Point p;
    is >> p.x >> p.y;
    points.push_back(p);
  }

  // update the information for Front
  Front::maxTotal = points.size();
  Front::maxArea = computeArea();
}

void Data::computeConvexHull(std::vector<size_t> &indices, size_t nb_indices, Solution & sol) {

  //std::cout << "Nb indices: " << nb_indices << std::endl;
  Point * polygon = new Point[nb_indices];
  for (size_t i = 0; i < nb_indices; ++i)
  {
    polygon[i] = points[indices[i]];
  }
  
  std::sort(&polygon[0],&polygon[nb_indices]);

  Point * hull = new Point[nb_indices+1];

  sol.nbPoints = computePolygonConvexHull(polygon,nb_indices,hull) - 1;
  //std::cout << "Size of the computed hull: " << sol.nbPoints << std::endl;

  //Resolution Hull with points to Hull with indexes
  //std::cout << "Found hull:" ;
  for (unsigned i = 0; i < sol.nbPoints; ++i) 
  {
    //std::cout << " " << hull[i];
    size_t index = 0;
    while (index < sol.data.points.size() && hull[i] != sol.data.points[index]) {
        ++index;
    }
    sol.hull.push_back(index);
  }
  sol.hull.push_back(sol.hull.front());

  /*
  std::cout << " " << hull[0] << " " ;
  std::cout << "<=" ;
  for (unsigned i = 0; i < sol.hull.size(); ++i)  {
    std::cout << " " << sol.hull[i] ;
  }
  std::cout << std::endl;
  */

  delete [] hull;
  delete [] polygon;

  size_t cpt;
  cpt = sol.computeNbTotalPoints();

  if (Front::maxTotal < cpt) {
    Front::maxTotal = cpt;
  }

  sol.computePerimeter();
  double area;
  area = sol.computeArea();


  if (Front::maxArea < area) {
    Front::maxArea = area;
  }


} 

double Data::computeArea () {
  // create the Id vector
  std::vector<size_t> indices(points.size());
  std::iota(begin(indices), end(indices), static_cast<size_t>(0)); //Fills indices with values starting from 0.

  Solution solution(*this);
  computeConvexHull (indices, indices.size(), solution);

  return solution.computeArea();
}

std::ostream & operator<< (std::ostream & o, Data & d) {
    o << "Data Points:" << std::endl;
    for (unsigned i = 0; i < d.points.size(); ++i) {
        o << "(" << d.points[i].x << "," << d.points[i].y << ") ";
    }
    return o;
}
