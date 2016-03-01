#ifndef __SOLUTIONS_H__
#define __SOLUTIONS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <set>
#include <map>
#include "style.h"
#include "front.h"

class QCPHover;

typedef std::vector<FPoint> FPointv;
typedef std::vector<FPoint *> FPointPtrv;
typedef std::set<FPoint> FPoints;
typedef std::set<FPoint *> FPointPtrS;
typedef std::map<float,FPoint *> FPointPtrMap;
typedef std::map<float,FPointPtrMap > FPointPtrMMap;
typedef Front ParetoFront;
typedef std::vector<ParetoFront> ParetoFrontv;

class Solutions
{
  protected:
    FPointv         pts_;
    FPointPtrMMap   pts_map_;
    unsigned        nb_pts_;
    float           x_min_;
    float           x_max_;
    float           y_min_;
    float           y_max_;
    std::string     abscissa_;
    std::string     ordinate_;
    std::string     filename_;
    ParetoFrontv    pFrontiers_;
    Style           style_;

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
          int              getNbPts            ()                      const;
    const FPointv       &  getPts              ()                      const;
    const FPointPtrMMap &  getPtsMap           ()                      const;
          ParetoFrontv  &  getPFrontiers       ();
          Style         &  getStyle            ();
          FPointPtrv    *  findPointsInArea    (FPoint &, FPoint &)    const;
          void             compute_frontiers   ();
          float            compute_hypervolumen(ParetoFront &);
          void             compute_front_style (QCPHover *);
          void             saveToFile          (const char *)          const;
          void             exportToTikZ        (const char *);
  private:
    std::string tikzify(PointShape s) const;
};

#endif /* end of include guard: __SOLUTIONS_H__ */
