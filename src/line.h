#ifndef LINE_H_
#define LINE_H_

#include <point.h>
#include <math.h>

template <typename T>
class Line {
    public:
        Line() {}
        Line(T slope, T intercept) : m_slope(slope), m_intercept(intercept) {}
        Line(Line<T> const& ref) : m_slope(ref.m_slope), m_intercept(ref.m_intercept) {}

        ~Line() {}

        T GetSlope() const {return m_slope;}
        T GetIntercept() const {return m_intercept;}

        void Draw(wxDC& output, T scale) const {
            //y=mx+b
            T x1 = 1000.0;
            T x2 = -1000.0;
            T y1 = m_slope*x1+m_intercept;
            T y2 = m_slope*x2+m_intercept;
            output.DrawLine(x1*scale, y1*scale, x2*scale, y2*scale);
        }

    private:
        T m_slope;
        T m_intercept; //y intercept
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Line<T> const& rhs) {
    os << "Line {m=" << rhs.GetSlope() << ", b= " << rhs.GetIntercept() << "}";
    return os;
}

#endif
