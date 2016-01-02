#ifndef POINT_H
#define POINT_H

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

    //comparators
    bool dominates (Point<T>);
    bool operator< (const Point<T> &) const;

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

//comparators
template <typename T>
bool Point<T>::dominates (Point<T> p) {
    return (this->x_ >= p.x_ && this->y_ <= p.y_);
}

template <typename T>
bool Point<T>::operator< (const Point<T> &p) const {
    return this->x_ < p.x_ && this->y_ >= p.y_;
}


#endif // POINT_H
