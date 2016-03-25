#ifndef __SOLUTIONS_H__
#define __SOLUTIONS_H__

/*!
 * \file Solutions.h
 * \brief Set of solutions read from a file, has Pareto front processing abilities
 * \author Pierre-Loup Pissavy
 */

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <map>
#include "style.h"
#include "front.h"

class QCPHover;

typedef std::vector<FPoint> FPointv;
typedef std::vector<FPoint *> FPointPtrv;
typedef std::map<float,FPoint *> FPointPtrMap;
typedef std::map<float,FPointPtrMap > FPointPtrMMap;
typedef Front ParetoFront;
typedef std::vector<ParetoFront> ParetoFrontv;

/*!
 * \brief Set of solutions and pareto frontiers on these solutions
 */
class Solutions
{
  protected:
    FPointv         pts_;           //!< Points read in data file
    FPointPtrMMap   pts_map_;       //!< Points in a sorted fashion
    unsigned        nb_pts_;        //!< Number of points
    float           x_min_;         //!< Lowest x coordinate
    float           x_max_;         //!< Highest x coordinate
    float           y_min_;         //!< Lowest y coordinate
    float           y_max_;         //!< Highest y coordinate
    std::string     abscissa_;      //!< Name of the x axis
    std::string     ordinate_;      //!< Name of the y axis
    std::string     filename_;      //!< File used to get the data
    ParetoFrontv    pFrontiers_;    //!< Pareto frontiers
    Style           style_;         //!< Style of the graph

  public:
    Solutions();
    Solutions(const char *);
    ~Solutions();

    const float & getMaxX() const;
    const float & getMaxY() const;
    const float & getMinX() const;
    const float & getMinY() const;
    const std::string & getAbscissa() const;
    const std::string & getOrdinate() const;
    const std::string & getFilename() const;
    const unsigned      &  getNbPts            ()                      const;
    const FPointv       &  getPts              ()                      const;
    const FPointPtrMMap &  getPtsMap           ()                      const;
          ParetoFrontv  &  getPFrontiers       ();
          Style         &  getStyle            ();
          void             setPts              (FPointv &);
          FPointPtrv       findPointsInArea    (FPoint &, FPoint &)    const;
          void             compute_frontiers   ();
          void             compute_front_style (QCPHover *);
          void             saveToFile          (const char *)          const;
          void             exportToTikZ        (const char *);
  private:
    std::string tikzify(PointShape s) const;
};

#endif /* end of include guard: __SOLUTIONS_H__ */
