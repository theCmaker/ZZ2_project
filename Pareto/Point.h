#ifndef POINT_H
#define POINT_H

#include <string>

template<typename T>
class Point {
private:
    T x_;
    T y_;
    unsigned id_;

    static unsigned cpt_;

public:
    Point (T=0, T=0);
    Point (const Point<T> &);
    ~Point ();

    //getters
    T getX () const;
    T getY () const;

    //setters
    void setX (T);
    void setY (T);

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
    o << '(' << p.getX() << ',' << p.getY() << ')';
    return o;
}

template <typename T>
Point<T>::Point (T x, T y) :
    x_(x),
    y_(y)
{
    id_ = cpt_++;
}

template <typename T>
Point<T>::Point (const Point<T> &p) :
    x_(p.x_),
    y_(p.y_)
{
    id_ = cpt_++;
}

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

//setters
template <typename T>
void Point<T>::setX (T x) {
    x_ = x;
}

template <typename T>
void Point<T>::setY (T y) {
    y_ = y;
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
    sprintf(&buf[0],"(%f,%f)",(float) x_, (float) y_);
    return std::string(buf);
}


#endif // POINT_H
