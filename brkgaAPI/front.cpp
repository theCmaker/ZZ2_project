#include "front.h"
#include "pfront.h"
#include "Solutions.h"
double Front::minArea = 0.0;
size_t Front::minTotal = 1;
double Front::maxArea = 0.0;
size_t Front::maxTotal = 1;

double Front::computeHypervolume () {
    Solutions s;
    FPointv pts;

    for (size_t i = 0; i < solutions.size(); ++i)
    {
      pts.push_back(FPoint(i,
                            -float(solutions[i].nbTotalPoints),
                            (float)solutions[i].area,
                            std::string("")));
    }

    s.setPts(pts);
    s.compute_frontiers();
    s.getPFrontiers()[0].compute_stats(-double(maxTotal),-double(minTotal),minArea,maxArea);
    hypervolume = s.getPFrontiers()[0].hypervolumen();
    for (PolyLine::iterator i = s.getPFrontiers()[0].begin(); i != s.getPFrontiers()[0].end(); ++i)
    {
      std::cout << **i << " ";
    }
    std::cout << "in the best front" << std::endl;
    return hypervolume;
}

std::ostream & operator<< (std::ostream & os, const Front & f)
{
  os << f.solutions.size() << " elements, " << f.hypervolume << " hypervolume" << std::endl;
  for (size_t i = 0; i < f.solutions.size(); ++i)
  {
    os << i << ": " << f.solutions[i] << std::endl;
  }
  return os;
}
