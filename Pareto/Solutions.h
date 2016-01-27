#ifndef __SOLUTIONS_H__
#define __SOLUTIONS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <set>
#include <map>
#include "Point.h"

//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point.hpp>
//namespace bg = boost::geometry;
//typedef bg::model::point<float, 2, bg::cs::cartesian> Point;

typedef Point<float> FPoint;
typedef std::vector<FPoint> FPointv;
typedef std::vector<FPoint *> FPointPtrv;
typedef std::set<FPoint> FPoints;
typedef std::set<FPoint *> FPointPtrS;
typedef std::map<float,FPoint *> FPointPtrMap;
typedef std::map<float,FPointPtrMap > FPointPtrMMap;
typedef std::vector<FPoint *> ParetoFront;
typedef std::vector<ParetoFront> ParetoFrontv;

class Solutions
{
  protected:
    FPointv                 pts_;
    unsigned                nb_pts_;
    float                   x_min_;
    float                   x_max_;
    float                   y_min_;
    float                   y_max_;
    std::string            *pts_info_;
    std::string             abscissa_;
    std::string             ordinate_;
    std::string             filename_;
    ParetoFrontv            pFrontiers_;
    FPointPtrMMap           pts_map_;
  public:
    Solutions();
    Solutions(const char *);
    ~Solutions();

    float getMaxX() const;
    float getMaxY() const;
    float getMinX() const;
    float getMinY() const;
    std::string getAbscissa() const;
    std::string getOrdinate() const;
    std::string getFilename() const;

    FPointv         getPts              ()                      const;
    FPointPtrMMap   getPtsMap           ()                      const;
    ParetoFrontv    getPFrontiers       ()                      const;
    FPointPtrv      *findPointsInArea   (FPoint &, FPoint &)    const;
    void            compute_frontiers   ();
};

#endif /* end of include guard: __SOLUTIONS_H__ */
