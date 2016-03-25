#pragma once

#include <iostream>
#include <vector>

// one solution, i.e. a convex hull of a subset of points
typedef struct Data Data;

struct Solution
{
  Data & data;
  std::vector<size_t> hull;    // sequence of the n+1 point Ids defining the convex hull, such that hull[0] = hull[n]

  size_t nbPoints;          // number of points in the convex hull (n = nbPoints)
  size_t nbTotalPoints;     // total number of points covered by the convex hull (hull included)
  double perimeter;         // perimeter of the convex hull
  double area;              // area of the convex hull

  Solution (Data & d): data(d) {}

  size_t computeNbTotalPoints ();
  double computePerimeter     ();
  double computeArea          ();
};

std::ostream & operator<< (std::ostream &, const Solution &);
