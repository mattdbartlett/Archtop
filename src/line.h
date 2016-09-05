#ifndef LINE_H_
#define LINE_H_

#include <wx/dc.h>
#include <dl_entities.h>
#include <memory>
#include <math.h>
#include <types.h>

template <typename T>
class LineBase {
    public:
        LineBase(pt_base<T> const& p1, pt_base<T> const& p2) : m_p1(p1), m_p2(p2) {}
        LineBase(T x1, T y1, T x2, T y2): m_p1(x1,y1), m_p2(x2,y2) {}

        pt_base<T> GetP1() const {return m_p1;};
        pt_base<T> GetP2() const {return m_p2;};

        void Draw(wxDC& output, T scale) const {
            output.DrawLine(m_p1.x*scale, m_p1.y*scale, m_p2.x*scale, m_p2.y*scale);
        }

        bool IsVertical() const {return fabs(m_p1.x-m_p2.x) <= 1e-6;};

    private:
        pt_base<T> m_p1;
        pt_base<T> m_p2;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, LineBase<T> const& rhs) {
    os << "Line {" << rhs.GetP1() << ", " << rhs.GetP2() <<  "}";
    return os;
}

typedef LineBase<double> Line;

#endif
