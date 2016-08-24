#ifndef POINT_H_
#define POINT_H_

#include <dl_entities.h>
#include <drawingcomponent.h>
#include <types.h>

template <typename T >
class PointBase : public DrawingComponent<T> {
    public:
        PointBase() : m_x(0), m_y(0) {};
        PointBase(pt_base<T> pt) : m_x(pt.x), m_y(pt.y) {};
        PointBase(T x, T y) : m_x(x), m_y(y) { }
        PointBase(DL_PointData const& pointData) : m_x(pointData.x), m_y(pointData.y) {}
        virtual ~PointBase() {}

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
            return std::make_shared<PointBase<T>>(*this);
        }

        virtual std::list<pt_base<T>> Intersection(RayBase<T> const& ray) const {
            return std::list<pt_base<T>>();
        }

        virtual std::list<pt_base<T>> Intersection(LineBase<T> const& line) const {
            return std::list<pt_base<T>>();
        }

        virtual void Print(std::ostream& os) const {
            os << *this;
        }

    private:
        T m_x;
        T m_y;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, PointBase<T> const& rhs) {
    os << "Pt {" << rhs.GetX() << ", " << rhs.GetY() << "}";
    return os;
}

typedef PointBase<double> Point;

#endif
