#ifndef RAY_H
#define RAY_H

#include <wx/dc.h>
#include <dl_entities.h>
#include <memory>
#include <math.h>

template <typename T>
class RayBase {
    public:
        RayBase() : m_angle(0) {};
        RayBase(pt_base<T> start, T angle) : m_start(start), m_angle(angle) {}

        pt_base<T> GetStart() const {return m_start;};
        T GetAngle() const {return m_angle;};

        void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;

            T xOff = 1000.0*cos(m_angle*M_PI/180.0L);
            T yOff = 1000.0*sin(m_angle*M_PI/180.0L);
            output.DrawLine(m_start.x*scale, m_start.y*scale, (m_start.x+xOff)*scale, (m_start.y+yOff)*scale);
        }

        /**
         * Get a point on the ray the specified distance from the center
         */
        pt_base<T> GetPoint(T length) const {
            T dx = length*cos(m_angle*M_PI/180.0L);
            T dy = length*sin(m_angle*M_PI/180.0L);
            return pt_base<T>(m_start.x+dx, m_start.y+dy);
        }

    private:
        pt_base<T> m_start;
        T m_angle;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, RayBase<T> const& rhs) {
    os << "Ray {" << rhs.GetStart() << " -> " << rhs.GetAngle() << "}";
    return os;
}

typedef RayBase<double> Ray;

#endif
