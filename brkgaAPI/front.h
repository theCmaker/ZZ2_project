#pragma once

#include <vector>
#include "solution.h"
#include <limits>

typedef struct Front
{
  static double minArea;
  static double maxArea;
  static size_t minTotal;
  static size_t maxTotal;

  std::vector<Solution> solutions;

  double hypervolume;

  double computeHypervolume ();
  void add (const Solution & s) {solutions.push_back(s);}
  void clear () {solutions.clear(); hypervolume = std::numeric_limits<double>::max();}
} Front;

std::ostream & operator<< (std::ostream & os, const Front & f);
