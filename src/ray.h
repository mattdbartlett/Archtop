#ifndef RAY_H
#define RAY_H

#include <drawingcomponent.h>
#include <point.h>
#include <math.h>

template <typename T>
class Ray {
    public:
        Ray() : m_startX(0), m_startY(0), m_slope(0), m_yIntercept(0) {}
        Ray(Ray<T> const& src) : m_startX(src.m_startX), m_startY(src.m_startY), m_angle(src.m_angle), m_slope(src.m_slope), m_yIntercept(src.m_yIntercept) {}
        Ray(T startX, T startY, T angle) : m_startX(startX), m_startY(startY), m_angle(angle) {
            m_slope = tan(m_angle*M_PI/180.0L);
            m_yIntercept=m_startY-m_slope*m_startX;
        }

        ~Ray() {}

        std::pair<T, T> GetStart() const {return std::pair<T,T>(m_startX, m_startY);};
        T GetAngle() const {return m_angle;};
        T GetIntercept() const {return m_yIntercept;};
        T GetSlope() const {return m_slope;};

        void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;

            T xOff = 1000.0*cos(m_angle*M_PI/180.0L);
            T yOff = 1000.0*sin(m_angle*M_PI/180.0L);
            output.DrawLine(m_startX()*scale, m_startY()*scale, (m_startX()+xOff)*scale, (m_startY()+yOff)*scale);
        }

    private:
        T m_startX;
        T m_startY;
        T m_angle;
        T m_slope;
        T m_yIntercept;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Ray<T> const& rhs) {
    os << "Ray {" << rhs.GetStart() << " -> " << rhs.GetAngle() << "}";
    return os;
}

#endif
