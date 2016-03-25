#pragma once
#include <iostream>
#include <vector>
#include <cmath>


// basic 2D point
class Point
{
public:
  double x;
  double y;

  Point (): x(0.), y(0.) {}
  bool operator== (const Point &) const;
  bool operator!= (Point &);
  bool operator<  (const Point &) const;
};

std::ostream & operator<< (std::ostream &, Point &) ;
std::ostream & operator<< (std::ostream &,const Point &) ;


// isLeft(): tests if a point P2 is Left|On|Right of a line passing through P0 and P1.
// Input: three points P0, P1, and P2
// Return: > 0 if P2 is left of the line
//         = 0 if P2 is on the line
//         < 0 if P2 is right of the line
// borrowed from http://geomalgorithms.com/a03-_inclusion.html

inline double isLeft (const Point & P0, const Point & P1, const Point & P2)
{
  return ( (P1.x - P0.x) * (P2.y - P0.y) - (P2.x -  P0.x) * (P1.y - P0.y) );
}


size_t pointInPolygon (const Point & P, Point * V, size_t n);
double computePolygonArea (Point * V, size_t n);
size_t computePolygonConvexHull ( Point * P, size_t n, Point * H);

// computeDistance(): compute the euclidian distance between two points
// Input: P1, P2 = the two points
// Return: the euclidian distance

inline double computeDistance (const Point & P1, const Point & P2)
{
  return (sqrt((P2.x - P1.x) * (P2.x - P1.x) + (P2.y - P1.y) * (P2.y - P1.y)));
}
