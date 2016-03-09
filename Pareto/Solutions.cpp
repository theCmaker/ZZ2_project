#include "Solutions.h"
#include "qcp_hover.h"
/*!
 * \brief Default solution constructor
 */
Solutions::Solutions() : nb_pts_(0) {
}

/*!
 * \brief Solution constructor from a given file
 * \param name Name of the solutions description file
 */
Solutions::Solutions(const char *name) :
    nb_pts_(0),
    x_min_(std::numeric_limits<float>::max()),
    x_max_(std::numeric_limits<float>::min()),
    y_min_(std::numeric_limits<float>::max()),
    y_max_(std::numeric_limits<float>::min()),
    filename_(name) {
    double x, y;
    std::string buf;
	std::ifstream ifs (name,std::ifstream::in);
    if (!ifs.is_open()) {
        std::cout << "Could not read file." << std::endl;
    } else {
        if (ifs.eof()) {
            std::cout << "File is empty!" << std::endl;
        } else {
            while (std::getline(ifs,buf)) {
                if (buf == "NB SOLUTIONS") {
                    ifs >> nb_pts_;
                } else if (buf == "FIRST OBJECTIVE") {
                    std::getline(ifs,abscissa_);
                } else if (buf == "SECOND OBJECTIVE") {
                    std::getline(ifs,ordinate_);
                } else if (buf == "SOLUTIONS") {
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
            }
        }
    }
}

Solutions::~Solutions() {
}

/*!
 * \brief Highest (and worst) value on first objective
 * \return worst value for the first objective
 */
const float & Solutions::getMaxX() const {
    return x_max_;
}

/*!
 * \brief Highest (and worst) value on second objective
 * \return worst value for the second objective
 */
const float & Solutions::getMaxY() const {
    return y_max_;
}

/*!
 * \brief Lowest (and best) value on first objective
 * \return best value for the first objective
 */
const float & Solutions::getMinX() const {
    return x_min_;
}

/*!
 * \brief Lowest (and best) value on second objective
 * \return best value for the second objective
 */
const float & Solutions::getMinY() const {
    return y_min_;
}

/*!
 * \brief All the solutions read from the file
 * \return vector containing all the points
 */
const FPointv & Solutions::getPts() const {
    return pts_;
}

/*!
 * \brief All the solutions but in a sorted fashion.
 * \return a sorted map indexed on first objective values containing sorted maps of pointers to the solutions, indexed on second objective values.
 * \warning compute_frontiers() must have been called at least one time before calling this method
 */
const FPointPtrMMap & Solutions::getPtsMap() const {
    return pts_map_;
}

/*!
 * \brief Name of the first objective
 * \return abscissa label
 */
const std::string & Solutions::getAbscissa() const {
    return abscissa_;
}

/*!
 * \brief Name of the second objective
 * \return ordinate label
 */
const std::string & Solutions::getOrdinate() const {
    return ordinate_;
}

/*!
 * \brief Name and path of the original file
 * \return name and path of the original file
 */
const std::string & Solutions::getFilename() const {
    return filename_;
}

/*!
 * \brief Number of solutions stored in pts_
 * \return number of solutions
 */
int Solutions::getNbPts() const
{
    return nb_pts_;
}

/*!
 * \brief List of Pareto frontiers, from the best to the worst.
 * \return all the computed frontiers
 * \see compute_frontiers
 */
ParetoFrontv & Solutions::getPFrontiers() {
    return pFrontiers_;
}

/*!
 * \brief Style associated to the graph of the solutions
 * \return style
 */
Style &Solutions::getStyle()
{
    return style_;
}

/*!
 * \brief Builds a list of all the points into the rectangle defined by \a top_left and \a bottom_right.
 * \param top_left     Coordinates of the top left hand corner of the rectangle.
 * \param bottom_right Coordinates of the bottom right hand corner of the rectangle.
 * \return vector of pointers on the points that are effectively in the rectangle.
 */
FPointPtrv Solutions::findPointsInArea(FPoint &top_left, FPoint &bottom_right) const {
    FPointPtrv in_area = FPointPtrv();

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
                        in_area.push_back(border_bottom_itr->second);
                    }
                }
            }
        }
    }
    return in_area;
}

/*!
 * \brief Computes the Pareto Frontiers of the whole file.
 *
 * The pts_map_ is (re)built and the pFrontiers_ are generated.
 */
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
    if (pFrontiers_.size() > 0) {
        //If computing is called again, then clean all the old pareto fronts.
        pFrontiers_.clear();
    }
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
    std::cout << "Computed " << i << " fronts." << std::endl;
}

/*!
 * \brief Saves the set of solutions into \a name file.
 * \param name Name of the output file
 */
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

/*!
 * \brief Export to a TeX file as standalone class with a TikZ picture
 * \param name File name
 *
 * This method uses the computed QCPHover plot to perform a better render.
 *
 * \warning Needs a graphical user interface
 *
 * \todo Make a TikZfyer class that can handle TikZ objects with simple methods.
 */
void Solutions::exportToTikZ(const char *name) {
    std::ofstream output;
    output.open(name);

    //File header
    output << "\\documentclass{standalone}" << std::endl;
    output << "\\usepackage[T1]{fontenc}" << std::endl;
    output << "\\usepackage[utf8]{inputenc}" << std::endl;
    output << "\\usepackage[usenames,dvipsnames]{xcolor}" << std::endl;
    output << "\\usepackage{tikz}" << std::endl;
    output << "\\usetikzlibrary{plotmarks}" << std::endl;
    output << "\\usetikzlibrary{shapes,snakes,arrows}" << std::endl;
    output << "\\begin{document}" << std::endl;
    output << "\\begin{tikzpicture}[xscale=" << 12./style_.width() << ",yscale=" << 12./style_.width()*style_.aspectRatio() << "]" << std::endl;

    //grid
    if (style_.has_grid()) {
        output << "\\draw[xstep=" << style_.x_step() << ",ystep=" << style_.y_step() << ",thin,dotted,color=Black] (" << style_.x_min() << "," << style_.y_min() << ") grid (" << style_.x_max() << "," << style_.y_max() << ");" << std::endl;
    }

    //Scope
    output << "\\begin{scope}" << std::endl;
    output << "  \\clip (" << style_.x_min() << "," << style_.y_min() << ") rectangle (" << style_.x_max() << "," << style_.y_max() << ");" << std::endl;
    //Selected Front, if any
    if (style_.selected_front() >= 0) {
        ParetoFront * front = &(pFrontiers_[style_.selected_front()]);
        QColor hv_color = front->lineColor();
        output << "  \\definecolor{hvColor}{RGB}{" << hv_color.red() << ","
               << hv_color.green() << "," << hv_color.blue() << "}" << std::endl;
        output << "  \\draw[color=hvColor, fill=hvColor, fill opacity=0.4] (" << x_min_ << "," << y_min_ << ")";
        if (front->pts().front()->getX() != x_min_) {
            output << " -- (" << x_min_ << "," << y_max_ << ")";
            output << " -- (" << front->pts().front()->getX() << "," << y_max_ << ")";
        }
        for (PolyLine::const_iterator point = front->pts().begin(); point != front->pts().end(); ++point) {
            output << " -- (" << (*point)->getX() << "," << (*point)->getY() << ")";
        }
        output << " -| (" << x_max_ << "," << y_min_ << ") -- cycle;" << std::endl;
    }
    //Points and fronts
    for (ParetoFrontv::iterator front = pFrontiers_.begin() ; front != pFrontiers_.end(); ++front) {
        QColor pcolor = front->pointColor();
        QColor lcolor = front->lineColor();
        std::string shape  = tikzify(front->pointStyle());
        output << "  \\definecolor{pLineColor}{RGB}{" << lcolor.red() << ","
               << lcolor.green() << "," << lcolor.blue() << "}" << std::endl;
        output << "  \\definecolor{pPointColor}{RGB}{" << pcolor.red() << ","
               << pcolor.green() << "," << pcolor.blue() << "}" << std::endl;
        output << "  \\draw[thick,color=pPointColor] (" << front->pts().front()->getX() << "," << front->pts().front()->getY() << ") " << shape ;
        for (PolyLine::const_iterator point = front->pts().begin()+1; point != front->pts().end(); ++point) {
            output << " -- (" << (*point)->getX() << "," << (*point)->getY() << ") " << shape;
        }
        output << ";" << std::endl;
    }
    output << "\\end{scope}" << std::endl;

    //Axis
    if (style_.has_axes()) {
        //xaxis
        std::vector<double>::iterator itr = style_.x_ticks().begin()+1;
        output << "\\draw[->,>=triangle 45] (" << style_.x_min() << "," << style_.y_min() << ") -- coordinate (x axis mid) (" << style_.x_max() << "," << style_.y_min() << ");" << std::endl;
        if (style_.has_axes_labels()) {
            output << "\\node[below=1cm,anchor=center] at (x axis mid) {" << abscissa_ << "};" << std::endl;
        }
        output << "\\foreach \\x in {";
        while (itr != style_.x_ticks().end()-2) {
            output << *itr << "," ;
            ++itr;
        }
        output << *itr << "}" << std::endl;
        output << "  \\draw (\\x," << style_.y_min() << ") -- (\\x," << style_.y_min() << ") node[anchor=north] {\\x};" << std::endl;

        //yaxis
        output << "\\draw[->,>=triangle 45] (" << style_.x_min() << "," << style_.y_min() << ") -- coordinate (y axis mid) (" << style_.x_min() << "," << style_.y_max() << ");" << std::endl;
        if (style_.has_axes_labels()) {
            output << "\\node[left=1cm,rotate=90,anchor=center] at (y axis mid) {" << ordinate_ << "};" << std::endl;
        }
        itr = style_.y_ticks().begin()+1;
        output << "\\foreach \\y in {";
        while (itr != style_.y_ticks().end()-2) {
            output << *itr << "," ;
            ++itr;
        }
        output << *itr << "}" << std::endl;
        output << "  \\draw (" << style_.x_min() << ",\\y) -- (" << style_.x_min() << ",\\y) node[anchor=east] {\\y};" << std::endl;
    }

    //End of file
    output << "\\end{tikzpicture}" << std::endl;
    output << "\\end{document}" << std::endl;

    output.close();
}

/*!
 * \brief Compute the hypervolumen of the given \a f front.
 * \param f front
 * \note The hv is also stored into the front.
 * \return the computed hypervolumen
 */
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

/*!
 * \brief Compute the style of each front
 * \param graph The graph in the GUI.
 * \warning Needs a GUI.
 */
void Solutions::compute_front_style(QCPHover *graph)
{
    int size = graph->graphCount();
    for (int i = 0; i < size; ++i) {
        pFrontiers_[i].setPointColor(graph->graph(i)->scatterStyle().pen().color());
        pFrontiers_[i].setLineColor(graph->graph(i)->pen().brush().color());
        pFrontiers_[i].setPointStyle(graph->graph(i)->scatterStyle().shape());
    }
}

/*!
 * \brief Turns a QCustomPlot PointShape into a TikZ node string
 * \param s Shape of a point
 * \return string that represents \a s without backslash nor semi-colon.
 */
std::string Solutions::tikzify(PointShape s) const {
    std::string res = "";
    switch(s) {
        case PointShape::ssCross:
            res = "node[color=pPointColor] {{\\LARGE $\\times$}}";
            break;
        case PointShape::ssPlus:
            res = "node[color=pPointColor] {{\\LARGE +}}";
            break;
        case PointShape::ssDisc:
            res = "node[color=pPointColor,fill=pPointColor, circle, inner sep = 0pt, minimum size=2mm] {}";
            break;
        case PointShape::ssSquare:
            res = "node[draw,color=pPointColor,fill=pPointColor, inner sep = 0pt, minimum size=2mm] {}";
            break;
        case PointShape::ssDiamond:
            res = "node[draw,rotate=45,color=pPointColor,fill=pPointColor, inner sep = 0pt, minimum size=2mm] {}";
            break;
        case PointShape::ssTriangle:
            res = "node[draw,regular polygon,regular polygon sides=3,scale=0.5,color=pPointColor,fill=pPointColor] {}";
            break;
        default:
            res = "node[color=pPointColor,fill=pPointColor, circle, inner sep = 0pt, minimum size=2mm] {}";
    }
    return res;
}
