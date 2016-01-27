#include "Solutions.h"

Solutions::Solutions() : nb_pts_(0) {
}

Solutions::Solutions(const char *name) :
    x_min_(std::numeric_limits<float>::max()),
    x_max_(std::numeric_limits<float>::min()),
    y_min_(std::numeric_limits<float>::max()),
    y_max_(std::numeric_limits<float>::min()),
    filename_(name)
{
	double x, y;
	std::string buf;
    //std::stringstream bufstream;

	std::ifstream ifs (name,std::ifstream::in);

	ifs >> nb_pts_ ;
    ifs >> abscissa_ ;
    ifs >> ordinate_ ;
    //pts_info_ = new std::string[nb_pts_];
	getline(ifs,buf); /* gobble end of line */
    for (unsigned i = 0; i < nb_pts_; ++i) {
		ifs >> x >> y;
        if (x_min_ > x) {
            x_min_ = x;
        } else if (x_max_ < x) {
            x_max_ = x;
        }
        if (y_min_ > y) {
            y_min_ = y;
        } else if (y_max_ < y) {
            y_max_ = y;
        }
        getline(ifs,buf);
        pts_.push_back(FPoint(i,x,y,buf));
	}
    std::cout << "File: " << filename_ << std::endl;
    std::cout << nb_pts_ << " points" << std::endl;
    std::cout << "Axis 1: " << abscissa_ << std::endl << "Axis 2: " << ordinate_ << std::endl;
    int j = 0;
    for (std::vector<FPoint>::iterator i = pts_.begin(); i != pts_.end(); ++i,++j)
    {
        std::cout << *i << std::endl;
    }
}

Solutions::~Solutions() {
	delete [] pts_info_;
}

float Solutions::getMaxX() const {
    return x_max_;
}

float Solutions::getMaxY() const {
    return y_max_;
}

float Solutions::getMinX() const {
    return x_min_;
}

float Solutions::getMinY() const {
    return y_min_;
}

FPointv Solutions::getPts() const {
    return pts_;
}

FPointPtrMMap Solutions::getPtsMap() const {
    return pts_map_;
}

std::string Solutions::getAbscissa() const {
    return abscissa_;
}

std::string Solutions::getOrdinate() const {
    return ordinate_;
}

std::string Solutions::getFilename() const {
    return filename_;
}

ParetoFrontv Solutions::getPFrontiers() const {
    return pFrontiers_;
}

FPointPtrv * Solutions::findPointsInArea(FPoint &top_left, FPoint &bottom_right) const
{
    FPointPtrv * in_area = new FPointPtrv;
    //Nvelle version
    FPointPtrMMap::const_iterator border_left_itr;
    FPointPtrMMap::const_iterator border_right_itr;
    FPointPtrMap::const_iterator  border_top_itr;
    FPointPtrMap::const_iterator  border_bottom_itr;
    if (! pts_map_.empty()) {
        border_left_itr = pts_map_.lower_bound(top_left.getX());
        border_right_itr = pts_map_.upper_bound(bottom_right.getX());
        if (border_left_itr != pts_map_.end()) {
            for (; border_left_itr != border_right_itr; ++border_left_itr) {
                border_bottom_itr = border_left_itr->second.lower_bound(bottom_right.getY());
                border_top_itr = border_left_itr->second.upper_bound(top_left.getY());
                if (border_bottom_itr != border_left_itr->second.end()) {
                    for (; border_bottom_itr != border_top_itr; ++border_bottom_itr) {
                        //TODO: voir si nécessaire d'ajouter le test de positionnement dans rectangle
                        in_area->push_back(border_bottom_itr->second);
                    }
                }
            }
        }
    }
    /*
    //Ancienne version
    for(FPointv::const_iterator i = pts_.begin(); i != pts_.end(); ++i) {
        //std::cout << "test with " << *i << "in rectangle " << top_left << "--" << bottom_right << std::endl;
        if (i->isIn(top_left,bottom_right)) {
            in_area->push_back(new FPoint(*i));
            std::cout << *i << " is in area" << std::endl;
        }
    }
    */
    return in_area;
}

void Solutions::compute_frontiers() {
    /*std::map<float,std::map<float,FPoint> > pts;
    std::map<float,std::map<float,FPoint> >::iterator xitr;
    */
    std::cout << "Compute frontiers" << std::endl;
    //Nvelle version plus efficace
    FPointPtrMMap::iterator x_itr;
    for (FPointv::iterator i = pts_.begin(); i != pts_.end(); ++i) {
        x_itr = pts_map_.find(i->getX()); // recherche abscisse
        if (x_itr != pts_map_.end()) { //abscisse déjà présente
            x_itr->second.insert(std::pair<float,FPoint *>(i->getY(),&(*i)));
        } else { //abscisse absente
            FPointPtrMap ymap;  //création de la sous-map
            ymap.insert(std::pair<float,FPoint *>(i->getY(),&(*i))); //ajout du point
            pts_map_.insert(std::pair<float,std::map<float,FPoint *> >(i->getX(),ymap)); //insertion de la sous-map
        }
    }
    /*
    //Ancienne version
    for (FPointv::iterator i = pts_.begin(); i != pts_.end(); ++i) {
        xitr = pts.find(i->getX()); // recherche abscisse
        if (xitr != pts.end()) { // x existant
            //std::cout << i->getX() << " présent dans la map, ajout de " << *i << std::endl;
            xitr->second.insert(std::pair<float,FPoint>(i->getY(),*i)); // ajout de la valeur dans la sous-map
        } else { // x non existant
            //std::cout << i->getX() << " absent de la map, ajout de " << *i << std::endl;
            std::map<float,FPoint> ymap; // sous map
            ymap.insert(std::pair<float,FPoint>(i->getY(),*i)); // ajout de la valeur
            pts.insert(std::pair<float,std::map<float,FPoint> >(i->getX(),ymap)); // ajout de la sous map
        }
    }
    */

    //Nvelle version
    FPointPtrMMap pts_map_copy(pts_map_);
    std::pair<FPointPtrS::iterator,bool> res;
    while (!pts_map_copy.empty()) {
        pFrontiers_.push_back(FPointPtrS()); //Ajout d'un front vide
        pFrontiers_.back().insert(pts_map_copy.begin()->second.begin()->second);
        FPoint *first = *(--pFrontiers_.back().end());
        pts_map_copy.begin()->second.erase(first->getY());
        if (pts_map_copy.begin()->second.empty()) {
            pts_map_copy.erase(first->getX());
        }
        if (!pts_map_copy.empty()) {
            for (x_itr = pts_map_copy.begin(); x_itr != pts_map_copy.end(); ++x_itr) {
                if (x_itr->second.begin()->second->dominates(*first)) {
                    res = pFrontiers_.back().insert(x_itr->second.begin()->second);
                    first = *(res.first);
                    x_itr->second.erase(first->getY());
                    if (x_itr->second.empty()) {
                        pts_map_copy.erase(x_itr);
                    }
                }
            }
        }
    }
    /*
    //Ancienne version
    std::pair<FPoints::iterator,bool> ret;
    while (!pts.empty()) {
        pFrontiers_.push_back(FPoints());
        pFrontiers_.back().insert(pts.begin()->second.begin()->second);
        //TODO: penser à utiliser le retour de insert.
        FPoint first = *(--(pFrontiers_.back().end()));
        pts.begin()->second.erase(first.getY());
        if (pts.begin()->second.empty()) {
            pts.erase(first.getX());
        }
        if (!pts.empty()) {
            for (xitr = pts.begin(); xitr != pts.end(); ++xitr) {
                if (xitr->second.begin()->second.dominates(first)) {
                    //std::cout << xitr->second.begin()->second << " dominates " << first << "... Add it to the frontier" << std::endl;
                    ret = pFrontiers_.back().insert(xitr->second.begin()->second);
                    first = *(ret.first);
                    xitr->second.erase(ret.first->getY());
                    //TODO: vérif valgrind
                    if (xitr->second.empty()) {
                        pts.erase(xitr);
                    }
                }
            }
        }
    }
    */
}
