#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>

/*!
 * \file Point.h
 * \brief 2D Point with id and info data
 * \author Pierre-Loup Pissavy
 */

template<typename T>
/*!
 * \brief 2D point with id and info data
 */
class Point {
private:
    T x_;               //!< First coordinate
    T y_;               //!< Second coordinate
    unsigned    id_;    //!< Id of the point
    std::string info_;  //!< Comment on the point

public:

    Point (unsigned=0, T=0, T=0, std::string="");
    Point (const Point<T> &);
    ~Point ();

    //getters
    const T           & getX    () const;
    const T           & getY    () const;
    const unsigned    & getId   () const;
    const std::string & getInfo () const;

    //setters
    void setX       (T &);
    void setY       (T &);
    void setInfo    (std::string &);

    //interrogators
    bool isIn (Point<T> &,Point<T> &) const;

    //comparators
    bool dominates (Point<T> &);
    bool operator< (const Point<T> &) const;

    //transformers
    std::string toString() const;

};




template <typename T>
/*!
 * \brief operator <<
 * \param o output stream
 * \param p point to be displayed
 * \return the output stream
 */
std::ostream & operator<< (std::ostream & o, const Point<T> &p) {
    o << '(' << p.getX() << ',' << p.getY() << ") " << p.getInfo();
    return o;
}

//Constructors
template <typename T>
/*!
 * \brief Constructor
 * \param id ID
 * \param x First coordinate
 * \param y Second coordinate
 * \param s Comment
 */
Point<T>::Point (unsigned id, T x, T y, std::string s) :
    x_(x),
    y_(y),
    id_(id),
    info_(s)
{
}

template <typename T>
/*!
 * \brief Copy constructor
 * \param p other point
 */
Point<T>::Point (const Point<T> &p) :
    x_(p.x_),
    y_(p.y_),
    id_(p.id_),
    info_(p.info_)
{
}

//Destructor
template <typename T>
/*!
 * \brief Destructor
 */
Point<T>::~Point () {}

//getters
template <typename T>
/*!
 * \brief Get first coordinate
 * \return first coordinate
 */
const T & Point<T>::getX () const {
    return x_;
}

template <typename T>
/*!
 * \brief Get second coordinate
 * \return second coordinate
 */
const T & Point<T>::getY() const {
    return y_;
}

template <typename T>
/*!
 * \brief Get id
 * \return id
 */
const unsigned & Point<T>::getId() const {
    return id_;
}

template <typename T>
/*!
 * \brief Get comment
 * \return comment
 */
const std::string & Point<T>::getInfo() const {
    return info_;
}

//setters
template <typename T>
/*!
 * \brief Set first coordinate
 * \param x first coordinate
 */
void Point<T>::setX (T & x) {
    x_ = x;
}

template <typename T>
/*!
 * \brief Set second coordinate
 * \param y second coordinate
 */
void Point<T>::setY (T & y) {
    y_ = y;
}

template <typename T>
/*!
 * \brief Set comment
 * \param info comment
 */
void Point<T>::setInfo(std::string & info) {
    info_ = info;
}

//interrogators
template <typename T>
/*!
 * \brief Check if this point is in the rectangle defined by \a top_left and \a bottom_right
 * \param top_left Top left coordinates of the rectangle
 * \param bottom_right Bottom right coordinates of the rectangle
 * \return true if this point is in the rectangle, false otherwise
 */
inline bool Point<T>::isIn (Point<T> & top_left, Point<T> & bottom_right) const {
    return ((this->x_ >= top_left.x_) && (this->x_ <= bottom_right.x_) && (this->y_ <= top_left.y_) && (this->y_ >= bottom_right.y_));
}

//comparators
template <typename T>
/*!
 * \brief Check if this point dominates \a p
 * \param p other point
 * \return true if this point dominates \a p, false otherwise
 */
inline bool Point<T>::dominates (Point<T> &p) {
    return (this->x_ > p.x_ && this->y_ < p.y_);
}

template <typename T>
/*!
 * \brief Comparator
 * \param p other point
 * \return true if this point has lower first coordinate and higher or same second coordinate than \a p
 */
inline bool Point<T>::operator< (const Point<T> &p) const {
    return (this->x_ < p.x_ && this->y_ >= p.y_);
}

//transformers
template <typename T>
/*!
 * \brief String equivalent of this point
 * \return string formatted point
 */
std::string Point<T>::toString () const {
    char buf[128];
    sprintf(&buf[0],"(%f,%f) %s",(float) x_, (float) y_, info_.c_str());
    return std::string(buf);
}


#endif // POINT_H
