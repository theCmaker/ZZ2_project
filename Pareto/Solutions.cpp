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
	pts_info_ = new std::string[nb_pts_];
	getline(ifs,buf); /* gobble end of line */
	for (int i = 0; i < nb_pts_; ++i) {
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
		getline(ifs,pts_info_[i]);
        pts_.push_back(FPoint(x,y));
	}
    std::cout << "File: " << filename_ << std::endl;
    std::cout << nb_pts_ << " points" << std::endl;
    std::cout << "Axis 1: " << abscissa_ << std::endl << "Axis 2: " << ordinate_ << std::endl;
    int j = 0;
    for (std::vector<FPoint>::iterator i = pts_.begin(); i != pts_.end(); ++i,++j)
    {
        std::cout << '(' << i->getX() << ',' << i->getY() << ')' << pts_info_[j] << std::endl;
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

std::string Solutions::getAbscissa() const {
    return abscissa_;
}

std::string Solutions::getOrdinate() const {
    return ordinate_;
}

std::string Solutions::getFilename() const {
    return filename_;
}

std::vector<FPoints> Solutions::getPFrontiers() const {
    return pFrontiers_;
}

FPointv * Solutions::findPointsInArea(FPoint &top_left, FPoint &bottom_right) const
{
    FPointv * in_area = new FPointv;
    for(FPointv::const_iterator i = pts_.begin(); i != pts_.end(); ++i) {
        //std::cout << "test with " << *i << "in rectangle " << top_left << "--" << bottom_right << std::endl;
        if (i->isIn(top_left,bottom_right)) {
            in_area->push_back(*i);
            //std::cout << *i << " is in area" << std::endl;
        }
    }
    return in_area;
}

void Solutions::compute_frontiers() {
    std::map<float,std::map<float,FPoint> > pts;
    std::map<float,std::map<float,FPoint> >::iterator xitr;
    //std::map<float,FPoint>::iterator yitr;
    /*std::cout << "debut" << std::endl;*/
    for (FPointv::iterator i = pts_.begin(); i != pts_.end(); ++i) {
        xitr = pts.find(i->getX()); // recherche abscisse
        if (xitr != pts.end()) { // x existant
            /*std::cout << i->getX() << " présent dans la map, ajout de " << *i << std::endl;*/
            xitr->second.insert(std::pair<float,FPoint>(i->getY(),*i)); // ajout de la valeur dans la sous-map
        } else { // x non existant
            /*std::cout << i->getX() << " absent de la map, ajout de " << *i << std::endl;*/
            std::map<float,FPoint> ymap; // sous map
            ymap.insert(std::pair<float,FPoint>(i->getY(),*i)); // ajout de la valeur
            pts.insert(std::pair<float,std::map<float,FPoint> >(i->getX(),ymap)); // ajout de la sous map
        }
    }

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
                    /*std::cout << xitr->second.begin()->second << " dominates " << first << "... Add it to the frontier" << std::endl;*/
                    ret = pFrontiers_.back().insert(xitr->second.begin()->second);
                    first = *(ret.first);
                    xitr->second.erase(ret.first->getY());
                    //TODO: vérif valgrind
                    if (xitr->second.empty()) {
                        pts.erase(xitr);
                    }
                    /*if (ret.second) {
                        std::cout << "insertion succeeded with " << *(ret.first) << std::endl;
                    } else {
                        std::cout << "insertion failed " << *(ret.first) << " was here" << std::endl;
                    }*/
                }/* else {
                    std::cout << xitr->second.begin()->second << " does not dominate " << first << "... Discarding it" << std::endl;
                }*/
            }
        }
    }
}
