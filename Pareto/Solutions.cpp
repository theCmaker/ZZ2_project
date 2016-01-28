#include "Solutions.h"

Solutions::Solutions() : nb_pts_(0) {
}

Solutions::Solutions(const char *name) :
    x_min_(std::numeric_limits<float>::max()),
    x_max_(std::numeric_limits<float>::min()),
    y_min_(std::numeric_limits<float>::max()),
    y_max_(std::numeric_limits<float>::min()),
    filename_(name) {
    int i = 0;
    double x, y;
    bool next = false;
    std::string buf;
	std::ifstream ifs (name,std::ifstream::in);
    //HEADER
    do {
        std::getline(ifs,buf);
    } while (buf != "NB SOLUTIONS");
    ifs >> nb_pts_;

    do {
        std::getline(ifs,buf);
    } while (buf != "FIRST OBJECTIVE");
    std::getline(ifs,abscissa_);

    do {
        std::getline(ifs,buf);
    } while (buf != "SECOND OBJECTIVE");
    std::getline(ifs,ordinate_);

    do {
        std::getline(ifs,buf);
    } while (buf != "SOLUTIONS");

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
        std::getline(ifs,buf);
        pts_.push_back(FPoint(i,x,y,buf.substr(0,buf.find('#'))));
    }
}

Solutions::~Solutions() {
}

const float & Solutions::getMaxX() const {
    return x_max_;
}

const float & Solutions::getMaxY() const {
    return y_max_;
}

const float & Solutions::getMinX() const {
    return x_min_;
}

const float & Solutions::getMinY() const {
    return y_min_;
}

const FPointv & Solutions::getPts() const {
    return pts_;
}

const FPointPtrMMap & Solutions::getPtsMap() const {
    return pts_map_;
}

const std::string & Solutions::getAbscissa() const {
    return abscissa_;
}

const std::string & Solutions::getOrdinate() const {
    return ordinate_;
}

const std::string & Solutions::getFilename() const {
    return filename_;
}

const ParetoFrontv & Solutions::getPFrontiers() const {
    return pFrontiers_;
}

FPointPtrv * Solutions::findPointsInArea(FPoint &top_left, FPoint &bottom_right) const {
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

    //Ancienne version
    /*for(FPointv::const_iterator i = pts_.begin(); i != pts_.end(); ++i) {
        //std::cout << "test with " << *i << "in rectangle " << top_left << "--" << bottom_right << std::endl;
        if (i->isIn(top_left,bottom_right)) {
            in_area->push_back(new FPoint(*i));
            //std::cout << *i << " is in area" << std::endl;
        }
    }*/

    return in_area;
}

void Solutions::compute_frontiers() {
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
    //Nvelle version
    FPointPtrMMap pts_map_copy(pts_map_);
    std::pair<ParetoFront::iterator,bool> res;
    int i = 0;
    while (!pts_map_copy.empty()) {
        ++i;
        pFrontiers_.push_back(ParetoFront()); //Ajout d'un front vide
        pFrontiers_.back().push_back(pts_map_copy.begin()->second.begin()->second);
        FPoint *first = pFrontiers_.back().front();
        pts_map_copy.begin()->second.erase(first->getY());
        if (pts_map_copy.begin()->second.empty()) {
            pts_map_copy.erase(first->getX());
        }
        if (!pts_map_copy.empty()) {
            x_itr = pts_map_copy.begin();
            while (x_itr != pts_map_copy.end()) {
                if (x_itr->second.begin()->second->dominates(*first)) {
                    pFrontiers_.back().push_back(x_itr->second.begin()->second);
                    first = x_itr->second.begin()->second;
                    x_itr->second.erase(first->getY());
                    if (x_itr->second.empty()) {
                        x_itr = pts_map_copy.erase(x_itr);
                    } else {
                        ++x_itr;
                    }
                } else {
                    ++x_itr;
                }
            }
        }
    }
    std::cout << "Computed " << pFrontiers_.size() << " frontiers." << std::endl;
}

void Solutions::saveToFile(const char * name) const {
    std::ofstream output;
    output.open (name);

    output << "Solution file generated by ParetoViewer." << std::endl << std::endl;
    output << "NB SOLUTIONS" << std::endl;
    output << nb_pts_ << std::endl;
    output << "FIRST OBJECTIVE" << std::endl;
    output << abscissa_ << std::endl;
    output << "SECOND OBJECTIVE" << std::endl;
    output << ordinate_ << std::endl;
    output << "SOLUTIONS" << std::endl;
    for (FPointv::const_iterator itr = pts_.begin(); itr != pts_.end(); ++itr) {
        output << itr->getX() << '\t' << itr->getY() << '\t' << itr->getInfo() << std::endl;
    }
    output.close();
}
