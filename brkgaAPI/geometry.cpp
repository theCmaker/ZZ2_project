#include "geometry.h"

bool Point::operator< (const Point &p) const {
  bool res = true;
  if (x > p.x) {
    res = false;
  }
  if (x == p.x && y > p.y) {
    res = false;
  }
  return res;
}

bool Point::operator== (const Point &p) const {
  return p.x == this->x && p.y == this->y;
}

bool Point::operator!= (Point &p) {
  return p.x != this->x || p.y != this->y;
}

std::ostream & operator<< (std::ostream &o, Point &p) {
  o << '(' << p.x << ',' << p.y << ')' ;
  return o;
}

std::ostream & operator<< (std::ostream &o,const Point &p) {
  o << '(' << (double) p.x << ',' << (double) p.y << ')' ;
  return o;
}

// pointInPolygon(): test for a point in a polygon, using the winding number
// Input: P = a point,
//        V[] = a polygon of n+1 points such that V[n] = V[0]
// Return: the winding number (=0 only when P is strictly outside)
// borrowed from http://geomalgorithms.com/a03-_inclusion.html

size_t pointInPolygon (const Point & P, Point * V, size_t n) {
  
  size_t wn = 0;    // the  winding number counter

  // loop through all edges of the polygon
  for (size_t i = 0; i < n; ++i) {// edge from V[i] to  V[i+1]
    if (V[i].y <= P.y) {// start y <= P.y
      if (V[i + 1].y > P.y) {     // an upward crossing
        if (isLeft(V[i], V[i + 1], P) > 0) { // P left of  edge
          ++wn;            // have  a valid up intersect
        }
	    }
    } else {                       // start y > P.y (no test needed)
      if (V[i + 1].y <= P.y) {     // a downward crossing
        if (isLeft(V[i], V[i + 1], P) < 0) { // P right of edge
          --wn;            // have  a valid down intersect
        }
	    }
    }
  }
  return wn;
}



// computePolygonArea(): compute the area of a 2D polygon
// Input: V = a polygon of n+1 points such that V[n] = V[0]
//        n = the number of vertices in the polygon
// Return: the area of the polygon
// borrowed from http://geomalgorithms.com/a01-_area.html

double computePolygonArea (Point * V, size_t n)
{
  // simple check
  if (n < 3) return 0.0;

  double area = 0.0;
  for (size_t i = 1, j = 2, k = 0; i < n; ++i, ++j, ++k)
  {
    area += V[i].x * (V[j].y - V[k].y);
  }
  area += V[n].x * (V[1].y - V[n-1].y);  // wrap-around term

  return area * 0.5;
}



// computePolygonConvexHull(): Andrew's monotone chain 2D convex hull algorithm
// Input:  P[] = an array of points presorted by increasing x and y-coordinates
//         n =  the number of points in P
// Output: H[] = an array of the convex hull vertices (max is n)
// Return: the number of points in H
// borrowed from http://geomalgorithms.com/a10-_hull-1.html

size_t computePolygonConvexHull ( Point * P, size_t n, Point * H)
{
    // the output array H[] will be used as the stack
    int   bot=0, top=(-1);   // indices for bottom and top of the stack
    int   i;                 // array scan index

    // Get the indices of points with min x-coord and min|max y-coord
    int minmin = 0, minmax;
    double xmin = P[0].x;
    for (i=1; i<int(n); i++)
        if (P[i].x != xmin) break;
    minmax = i-1;
    if (minmax == int(n)-1) {       // degenerate case: all x-coords == xmin
        H[++top] = P[minmin];
        if (P[minmax].y != P[minmin].y) // a  nontrivial segment
            H[++top] =  P[minmax];
        H[++top] = P[minmin];            // add polygon endpoint
        return top+1;
    }

    // Get the indices of points with max x-coord and min|max y-coord
    int maxmin, maxmax = n-1;
    double xmax = P[n-1].x;
    for (i=n-2; i>=0; i--)
        if (P[i].x != xmax) break;
    maxmin = i+1;

    // Compute the lower hull on the stack H
    H[++top] = P[minmin];      // push  minmin point onto stack
    i = minmax;
    while (++i <= maxmin)
    {
        // the lower line joins P[minmin]  with P[maxmin]
        if (isLeft( P[minmin], P[maxmin], P[i])  >= 0 && i < maxmin)
            continue;           // ignore P[i] above or on the lower line

        while (top > 0)         // there are at least 2 points on the stack
        {
            // test if  P[i] is left of the line at the stack top
            if (isLeft(  H[top-1], H[top], P[i]) > 0)
                 break;         // P[i] is a new hull  vertex
            else
                 top--;         // pop top point off  stack
        }
        H[++top] = P[i];        // push P[i] onto stack
    }

    // Next, compute the upper hull on the stack H above  the bottom hull
    if (maxmax != maxmin)      // if  distinct xmax points
         H[++top] = P[maxmax];  // push maxmax point onto stack
    bot = top;                  // the bottom point of the upper hull stack
    i = maxmin;
    while (--i >= minmax)
    {
        // the upper line joins P[maxmax]  with P[minmax]
        if (isLeft( P[maxmax], P[minmax], P[i])  >= 0 && i > minmax)
            continue;           // ignore P[i] below or on the upper line

        while (top > bot)     // at least 2 points on the upper stack
        {
            // test if  P[i] is left of the line at the stack top
            if (isLeft(  H[top-1], H[top], P[i]) > 0)
                 break;         // P[i] is a new hull  vertex
            else
                 top--;         // pop top point off  stack
        }
        H[++top] = P[i];        // push P[i] onto stack
    }
    if (minmax != minmin)
        H[++top] = P[minmin];  // push  joining endpoint onto stack

    return top+1;
}

