#pragma once
#include <vector>
#include "geometry.h"

#include "solution.h"
// global data storage

typedef struct Data
{
  std::vector<Point> points;

  Data (const std::string & filename);
  Data ();
  Data (Data &);

  void computeConvexHull (std::vector<size_t> &, size_t, Solution &); // TODO -> connect with computeConvexHull(P,n,H)
  double computeArea ();
} Data;

std::ostream & operator<< (std::ostream & o, Data & d);

