#ifndef RAY_H
#define RAY_H

#include <wx/dc.h>
#include <dl_entities.h>
#include <memory>
#include <math.h>

template <typename T>
class Ray {
    public:
        Ray() : m_angle(0) {};
        Ray(pt_base<T> start, T angle) : m_start(start), m_angle(angle) {}

        pt_base<T> GetStart() const {return m_start;};
        T GetAngle() const {return m_angle;};

        void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;

            T xOff = 1000.0*cos(m_angle*M_PI/180.0L);
            T yOff = 1000.0*sin(m_angle*M_PI/180.0L);
            output.DrawLine(m_start.x*scale, m_start.y*scale, (m_start.x+xOff)*scale, (m_start.y+yOff)*scale);
        }

    private:
        pt_base<T> m_start;
        T m_angle;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Ray<T> const& rhs) {
    os << "Ray {" << rhs.GetStart() << " -> " << rhs.GetAngle() << "}";
    return os;
}

#endif
