#ifndef LINE_H_
#define LINE_H_

#include <drawingcomponent.h>
#include <point.h>
#include <math.h>

template <typename T>
class Line : public DrawingComponent<T> {
    public:
        Line() {}
        Line(Point<T> const& p1, Point<T> const& p2) : m_p1(p1), m_p2(p2) {}
        Line(DL_LineData const& lineData) 
            : m_p1(lineData.x1, lineData.y1)
            , m_p2(lineData.x2, lineData.y2) {}

        virtual ~Line() {}

        Point<T> GetP1() const {return m_p1;};
        Point<T> GetP2() const {return m_p2;};

        virtual T MinX() const {return fmin(m_p1.GetX(), m_p2.GetX());}
        virtual T MinY() const {return fmin(m_p1.GetY(), m_p2.GetY());}
        virtual T MaxX() const {return fmax(m_p1.GetX(), m_p2.GetX());}
        virtual T MaxY() const {return fmax(m_p1.GetY(), m_p2.GetY());}

        virtual void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;
            output.DrawLine(m_p1.GetX()*scale, m_p1.GetY()*scale, m_p2.GetX()*scale, m_p2.GetY()*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const {
            return std::make_shared<Line<T>>(*this);
        }

    private:
        Point<T> m_p1;
        Point<T> m_p2;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Line<T> const& rhs) {
    os << "Line {" << rhs.GetP1() << " <-> " << rhs.GetP2() << "}";
    return os;
}

#endif
