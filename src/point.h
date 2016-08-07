#ifndef POINT_H_
#define POINT_H_

#include <dl_entities.h>
#include <drawingcomponent.h>

template <typename T >
class Point : public DrawingComponent<T> {
    public:
        Point() : m_x(0), m_y(0) {};
        Point(T x, T y) : m_x(x), m_y(y) { }
        Point(DL_PointData const& pointData) : m_x(pointData.x), m_y(pointData.y) {}
        virtual ~Point() {}

        T GetX() const {return m_x;};
        T GetY() const {return m_y;};

        virtual T MinX() const {return m_x;}
        virtual T MinY() const {return m_y;}
        virtual T MaxX() const {return m_x;}
        virtual T MaxY() const {return m_y;}

        virtual void Draw(wxDC& output, T scale) const {
            output.DrawPoint(GetX()*scale, GetY()*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const override {
            return std::make_shared<Point<T>>(*this);
        }

    private:
        T m_x;
        T m_y;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, Point<T> const& rhs) {
    os << "Pt {" << rhs.GetX() << ", " << rhs.GetY() << "}";
    return os;
}

#endif
