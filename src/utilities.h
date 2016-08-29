#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <cmath>
#include <types.h>

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
T Len(pt_base<T> p1, pt_base<T> p2) {
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

template <typename T>
class DistanceSort {
    public:
        DistanceSort(pt_base<T> startPt) : m_start(startPt) {};
        bool operator()(pt_base<T> const& p1, pt_base<T> const& p2) {
            T l1 = Len(p1, m_start);
            T l2 = Len(p2, m_start);
            return l1 < l2;
        }
    private:
        pt_base<T> m_start;
};

template <typename T>
T ToRadians(T degrees) {
    return degrees*M_PI/180.0L;
}

template <typename T>
T FromRadians(T radians) {
    return radians*180.0L/M_PI;
}

template <typename T>
std::list<T> SolveQuadratic(T a, T b, T c) {
    std::list<T> results;
    T descriminant = pow(b,2)-4*a*c;
    //descriminiant < 0 is no intersection
    // descriminant == 0 is a tangent
    // we don't care about either
    if (descriminant > 0) {
        T descriminantSqrt = sqrt(descriminant);
        results.push_back((-b+descriminantSqrt)/(2*a));
        results.push_back((-b-descriminantSqrt)/(2*a));
    }
    return results;
}


/**
 * Find the center (possible centers) of a circle based on 2 points (and a radius)
 */
template <typename T>
std::list<pt_base<T>> CenterFromTwoPoints(pt_base<T> const& p1, pt_base<T> const& p2, T radius) {
    std::list<pt_base<T>> result;

    //2 cases, if p1 and p2 are horizontal, the line through the center will be vertical
    if (AlmostEq(p1.y, p2.y)) {
        T x = (p1.x-p2.x)/2+p2.x;
        //(p1.x-c.x)^2+(p1.y-c.y)^2=r^2
        // c.x = x
        // p1.y^2-2p1.y*c.y+c.y^2+(p1.x-c.x)^2-r^2;
        // c.y^2-2p1.y*c.y+p1.y^2+(p1.x-c.x)^2-r^2;
        // a=1
        // b=-2*p1.y
        // c=p1.y^2-r^2+(p1.x-c.x)^2
        T a = 1;
        T b = -2*p1.y;
        T c = pow(p1.y,2)-pow(radius,2)+pow((p1.x-x),2);
        std::list<T> yVals = SolveQuadratic(a,b,c);

        for (auto idx = yVals.begin(); idx != yVals.end(); ++idx) {
            result.push_back(pt_base<T>(x, *idx));
        }
    } else {
        //calculate line passing through center
        /*T slope = (p1.y-p2.y)/(p2.x-p1.x);
        T cX = p2.x+(p1.x-p2.x)/2;
        T cY = p2.y+(p1.y-p2.y)/2;
        T intercept = cY-slope*cX;*/
    }

    return result;
}

#endif
