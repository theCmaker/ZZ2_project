#ifndef POINT_H
#define POINT_H

#include <string>

template<typename T>
class Point {
private:
    T x_;
    T y_;
    unsigned    id_;
    std::string info_;

public:
    Point (unsigned=0, T=0, T=0, std::string="");
    Point (const Point<T> &);
    ~Point ();

    //getters
    T           getX    () const;
    T           getY    () const;
    unsigned    getId   () const;
    std::string getInfo () const;

    //setters
    void setX       (T);
    void setY       (T);
    void setInfo    (std::string);

    //interrogators
    bool isIn (Point<T> &,Point<T> &) const;

    //comparators
    bool dominates (Point<T> &);
    bool operator< (const Point<T> &) const;

    //transformers
    std::string toString() const;

};

//display
template <typename T>
std::ostream & operator<< (std::ostream & o, const Point<T> &p) {
    o << '(' << p.getX() << ',' << p.getY() << ") " << p.getInfo();
    return o;
}

//Constructors
template <typename T>
Point<T>::Point (unsigned id, T x, T y, std::string s) :
    x_(x),
    y_(y),
    id_(id),
    info_(s)
{
}

template <typename T>
Point<T>::Point (const Point<T> &p) :
    x_(p.x_),
    y_(p.y_),
    id_(p.id_),
    info_(p.info_)
{
}

//Destructor
template <typename T>
Point<T>::~Point () {}

//getters
template <typename T>
T Point<T>::getX () const {
    return x_;
}

template <typename T>
T Point<T>::getY () const {
    return y_;
}

template <typename T>
unsigned Point<T>::getId() const {
    return id_;
}

template <typename T>
std::string Point<T>::getInfo() const {
    return info_;
}

//setters
template <typename T>
void Point<T>::setX (T x) {
    x_ = x;
}

template <typename T>
void Point<T>::setY (T y) {
    y_ = y;
}

template <typename T>
void Point<T>::setInfo(std::string info) {
    info_ = info;
}

//interrogators
template <typename T>
bool Point<T>::isIn (Point<T> & top_left, Point<T> & bottom_right) const {
    return (this->x_ >= top_left.x_) && (this->x_ <= bottom_right.x_) && (this->y_ <= top_left.y_) && (this->y_ >= bottom_right.y_);
}

//comparators
template <typename T>
bool Point<T>::dominates (Point<T> &p) {
    return (this->x_ >= p.x_ && this->y_ <= p.y_);
}

template <typename T>
bool Point<T>::operator< (const Point<T> &p) const {
    return this->x_ < p.x_ && this->y_ >= p.y_;
}

//transformers
template <typename T>
std::string Point<T>::toString () const {
    char buf[128];
    sprintf(&buf[0],"(%f,%f) %s",(float) x_, (float) y_, info_.c_str());
    return std::string(buf);
}


#endif // POINT_H
