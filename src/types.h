#ifndef TYPES_H_
#define TYPES_H_

#include <limits>
#include <list>

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
void operator +=(pt_base<T>& lhs, pt_base<T> const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

template <typename T>
void operator -=(pt_base<T>& lhs, pt_base<T> const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
}

typedef pt_base<double> pt;

#endif
