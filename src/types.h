#ifndef TYPES_H_
#define TYPES_H_

#include <limits>

template <typename T>
struct pt_base {
    public:
        pt_base() : x(0), y(0) {}
        pt_base(T xNew, T yNew) : x(xNew), y(yNew) {}
        T x;
        T y;
};

typedef pt_base<double> pt;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, pt_base<T> const& pt) {
    os << "(" << pt.x << ", " << pt.y << ")"; return os;
};

template <typename T>
bool AlmostEq(T v1, T v2, T epsilon=0.001)
{
    if (v1 == v2) {
        return true;
    } else if (fabs(v1-v2) < epsilon) {
        return true;
    }
    return false;
}

template <typename T>
bool LineIntersection(pt_base<T> pt1, pt_base<T> pt2, pt_base<T> pt3, pt_base<T> pt4, pt_base<T>& intersection) {
    T xNum = (pt1.x*pt2.y-pt1.y*pt2.x)*(pt3.x-pt4.x)-(pt1.x-pt2.x)*(pt3.x*pt4.y-pt3.y*pt4.x);
    T den = (pt1.x-pt2.x)*(pt3.y-pt4.y)-(pt1.y-pt2.y)*(pt3.x-pt4.x);
    T yNum = (pt1.x*pt2.y-pt1.y*pt2.x)*(pt3.y-pt4.y)-(pt1.y-pt2.y)*(pt3.x*pt4.y-pt3.y*pt4.x);

    //if denominator is 0, lines are paralell (no intersection)
    if (AlmostEq(den,0.0)) {
        std::cout << "Lines are parallel" << std::endl;
        return false;
    }

    intersection.x = xNum/den;
    intersection.y = yNum/den;

    return true;
}


template <typename T>
bool InRange(T value, T min, T max) {
    //T epsilon(1e-9);
    //T minExp = std::numeric_limits<T>::min_exponent;
    //std::cout << "MinExponent " << minExp << std::endl;
    T lim(fmax(value,max));
    T epsilon(lim*pow(std::numeric_limits<T>::radix,std::numeric_limits<T>::min_exponent/32));
    if (AlmostEq(value, min, epsilon) || AlmostEq(value, max, epsilon)) {
        return true;
    } else {
        return ((value <= max) && (value >= min));
    }

}

template <typename T>
void operator +=(pt_base<T>& lhs, pt_base<T> const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

template <typename T>
void operator -=(pt_base<T>& lhs, pt_base<T> const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
}
     

#endif
