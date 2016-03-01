#include "Solutions.h"
#include "qcp_hover.h"
Solutions::Solutions() : nb_pts_(0) {
}

Solutions::Solutions(const char *name) :
    x_min_(std::numeric_limits<float>::max()),
    x_max_(std::numeric_limits<float>::min()),
    y_min_(std::numeric_limits<float>::max()),
    y_max_(std::numeric_limits<float>::min()),
    filename_(name) {
    double x, y;
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

int Solutions::getNbPts() const
{
    return nb_pts_;
}

ParetoFrontv & Solutions::getPFrontiers() {
    return pFrontiers_;
}

Style &Solutions::getStyle()
{
    return style_;
}

FPointPtrv * Solutions::findPointsInArea(FPoint &top_left, FPoint &bottom_right) const {
    FPointPtrv * in_area = new FPointPtrv;

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
                        in_area->push_back(border_bottom_itr->second);
                    }
                }
            }
        }
    }
    return in_area;
}

void Solutions::compute_frontiers() {
    //Build Map
    FPointPtrMMap::iterator x_itr;
    for (FPointv::iterator i = pts_.begin(); i != pts_.end(); ++i) { //For each point
        x_itr = pts_map_.find(i->getX()); //Look for the abscissa (if submap exists)
        if (x_itr != pts_map_.end()) { //Submap exists
            x_itr->second.insert(std::pair<float,FPoint *>(i->getY(),&(*i)));
        } else { //Submap does not exist
            FPointPtrMap ymap;  //Create the submap
            ymap.insert(std::pair<float,FPoint *>(i->getY(),&(*i))); //Add the point
            pts_map_.insert(std::pair<float,std::map<float,FPoint *> >(i->getX(),ymap)); //Insert the submap
        }
    }
    //Compute the frontiers using the previously built map
    FPointPtrMMap pts_map_copy(pts_map_);
    int i = 0; //Number of fronts
    while (!pts_map_copy.empty()) { //While there are remaining points
        ++i;
        pFrontiers_.push_back(ParetoFront()); //New empty front
        pFrontiers_.back().pts().push_back(pts_map_copy.begin()->second.begin()->second); //Add first point to the front
        FPoint *current = pFrontiers_.back().pts().front();  //Set current point
        pts_map_copy.begin()->second.erase(current->getY()); //Remove the point in the map
        if (pts_map_copy.begin()->second.empty()) {          //Remove the submap if empty
            pts_map_copy.erase(current->getX());
        }
        if (!pts_map_copy.empty()) {        //If there are some remaining points in the map
            x_itr = pts_map_copy.begin();   //Init the iterator over the map
            while (x_itr != pts_map_copy.end()) { //Some points have not been tested for insertion
                if (x_itr->second.begin()->second->dominates(*current)) { //If the last inserted point is dominated
                    pFrontiers_.back().pts().push_back(x_itr->second.begin()->second);  //Insertion
                    current = x_itr->second.begin()->second;    //Update the last inserted point
                    x_itr->second.erase(current->getY());       //Remove the point in the map
                    if (x_itr->second.empty()) {                //If submap is empty
                        x_itr = pts_map_copy.erase(x_itr);          //Remove the submap
                    } else {                                    //Else
                        ++x_itr;                                    //Jump to the next point
                    }
                } else {        //Last inserted point is not dominated
                    ++x_itr;    //Jump to the next point
                }
            }
        }
    }
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

void Solutions::exportToTikZ(const char *name) {
    std::ofstream output;
    output.open(name);

    //File header
    output << "\\documentclass{standalone}" << std::endl;
    output << "\\usepackage[usenames,dvipsnames]{xcolor}" << std::endl;
    output << "\\usepackage{tikz}" << std::endl;
    output << "\\usetikzlibrary{plotmarks}" << std::endl;
    output << "\\usetikzlibrary{shapes,snakes}" << std::endl;
    output << "\\begin{document}" << std::endl;
    output << "\\begin{tikzpicture}[xscale=" << 12./style_.width() << ",yscale=" << 12./style_.width()*style_.aspectRatio() << "]" << std::endl;

    //grid
    output << "\\draw[xstep=" << style_.x_step() << ",ystep=" << style_.y_step() << ",thin,dotted,color=Black] (" << style_.x_min() << "," << style_.y_min() << ") grid (" << style_.x_max() << "," << style_.y_max() << ");" << std::endl;

    //Points and fronts
    output << "\\begin{scope}" << std::endl;
    output << "\\clip (" << style_.x_min() << "," << style_.y_min() << ") rectangle (" << style_.x_max() << "," << style_.y_max() << ");" << std::endl;
    for (ParetoFrontv::iterator front = pFrontiers_.begin() ; front != pFrontiers_.end(); ++front) {
        QColor pcolor = front->pointColor();
        QColor lcolor = front->lineColor();
        std::string shape  = tikzify(front->pointStyle());
        output << "\\definecolor{pLineColor}{RGB}{" << lcolor.red() << ","
               << lcolor.green() << "," << lcolor.blue() << "}" << std::endl;
        output << "\\definecolor{pPointColor}{RGB}{" << pcolor.red() << ","
               << pcolor.green() << "," << pcolor.blue() << "}" << std::endl;
        output << "\\draw[color=pLineColor] (" << front->pts().front()->getX() << "," << front->pts().front()->getY() << ") " << shape ;
        for (PolyLine::const_iterator point = front->pts().begin()+1; point != front->pts().end(); ++point) {
            output << " -- (" << (*point)->getX() << "," << (*point)->getY() << ") " << shape;
        }
        output << ";" << std::endl;
    }
    output << "\\end{scope}" << std::endl;

    //xaxis
    std::vector<double>::iterator itr = style_.x_ticks().begin()+1;
    std::vector<std::string>::iterator itrlabel = style_.x_ticks_labels().begin()+1;
    output << "\\draw (" << style_.x_min() << "," << style_.y_min() << ") -- coordinate (x axis mid) (" << style_.x_max() << "," << style_.y_min() << ");" << std::endl;
    output << "\\foreach \\x/\\xl in {";
    while (itr != style_.x_ticks().end()-2) {
        output << *itr << "/"
               << *itr << "," ;
        ++itr;
        ++itrlabel;
    }
    output << *itr << "/" << *itr << "}" << std::endl;
    output << "  \\draw (\\x," << style_.y_min() << ") -- (\\x," << style_.y_min() << ") node[anchor=north] {\\xl};" << std::endl;
    //yaxis
    output << "\\draw (" << style_.x_min() << "," << style_.y_min() << ") -- coordinate (y axis mid) (" << style_.x_min() << "," << style_.y_max() << ");" << std::endl;
    itr = style_.y_ticks().begin()+1;
    itrlabel = style_.y_ticks_labels().begin()+1;
    output << "\\foreach \\y/\\yl in {";
    while (itr != style_.y_ticks().end()-2) {
        output << *itr << "/"
               << *itr << "," ;
        ++itr;
        ++itrlabel;
    }
    output << *itr << "/" << *itr << "}" << std::endl;
    output << "  \\draw (" << style_.x_min() << ",\\y) -- (" << style_.x_min() << ",\\y) node[anchor=east] {\\yl};" << std::endl;

    //End of file
    output << "\\end{tikzpicture}" << std::endl;
    output << "\\end{document}" << std::endl;

    output.close();
}

float Solutions::compute_hypervolumen(ParetoFront &f) {
    float delta_x = x_max_ - x_min_;
    float delta_y = y_max_ - y_min_;
    float hv = delta_x * delta_y;
    PolyLine::iterator i = f.begin();

    // Compute until penultimate point
    while (i != f.end()-1) {
        // Remove rectangle above current point till next point
        hv -= ((*(i+1))->getX() - (*i)->getX()) * (y_max_ - (*i)->getY());
        ++i;
    }

    // Last point -> rectangle computed with x_max_ as right side abscissa
    // (projection on line x = x_max_)
    hv -= (x_max_ - (*i)->getX()) * (y_max_ - (*i)->getY());

    //Normalize
    hv = hv / (delta_x * delta_y);
    f.setHypervolumen(hv);
    return hv;
}

void Solutions::compute_front_style(QCPHover *graph)
{
    int size = graph->graphCount();
    for (int i = 0; i < size; ++i) {
        pFrontiers_[i].setPointColor(graph->graph(i)->scatterStyle().pen().color());
        pFrontiers_[i].setLineColor(graph->graph(i)->pen().brush().color());
        pFrontiers_[i].setPointStyle(graph->graph(i)->scatterStyle().shape());
    }
}

std::string Solutions::tikzify(PointShape s) const {
    std::string res = "";
    switch(s) {
        case PointShape::ssCircle:
            res = "node[draw,color=pPointColor,circle] {}";
            break;
        case PointShape::ssCross:
            res = "node[color=pPointColor] {{\\huge $\\times$}}";
            break;
        case PointShape::ssPlus:
            res = "node[color=pPointColor] {{\\huge +}}";
            break;
        case PointShape::ssDisc:
            res = "node[color=pPointColor,fill=pPointColor, circle] {}";
            break;
        case PointShape::ssSquare:
            res = "node[draw,color=pPointColor] {}"; //fill=pPointColor
            break;
        case PointShape::ssDiamond:
            res = "node[draw,rotate=45,color=pPointColor] {}"; //fill=pPointColor
            break;
        case PointShape::ssTriangle:
            res = "node[draw,regular polygon,regular polygon sides=3,scale=0.5,color=pPointColor] {}";
            break;
        default:
            std::cout << "cas par defaut" << std::endl;
            res = "node[color=pPointColor,fill=pPointColor, circle] {}";
    }
    return res;
}
