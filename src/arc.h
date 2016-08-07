#ifndef ARC_H_
#define ARC_H_

#include <drawingcomponent.h>
#include <point.h>

class DLArcData;

template <typename T>
T ToRadians(T degrees) {
    return degrees*M_PI/180.0L;
}

template <typename T>
class Arc : public DrawingComponent<T> {
    public:
        Arc() : m_radius(0), m_startAngle(0), m_endAngle(0) {
        }

        Arc(T radius, 
            T startAngle,
            T endAngle,
            Point<T> const& center) 
            : m_radius(radius)
            , m_startAngle(startAngle)
            , m_endAngle(endAngle)
            , m_center(center) {
        }

        Arc(DL_ArcData const& arcData) 
         : m_radius(arcData.radius)
         , m_startAngle(arcData.angle1)
         , m_endAngle(arcData.angle2)
         , m_center(arcData.cx, arcData.cy) {
        }

        virtual ~Arc() {};

        T GetRadius() const {return m_radius;}
        T GetStartAngle() const {return m_startAngle;}
        T GetEndAngle() const {return m_endAngle;}
        Point<T> GetCenter() const {return m_center;}

        Point<T> GetStart() const {
            T startX = m_center.GetX()+m_radius*cos(ToRadians(m_startAngle));
            T startY = m_center.GetY()+m_radius*sin(ToRadians(m_startAngle));

            return Point<T>(startX, startY);
        }

        Point<T> GetEnd() const {
            T endX = m_center.GetX()+m_radius*cos(ToRadians(m_endAngle));
            T endY = m_center.GetY()+m_radius*sin(ToRadians(m_endAngle));

            return Point<T>(endX, endY);
        }

        //implement drawingcomponent
        //0 is positive X origin, increasing counter-clockwise
        virtual T MinX() const {
            if (m_startAngle < 180 &&
                m_endAngle > 180) {
                return m_center.GetX()-m_radius;
            } else {
                return fmin(GetStart().GetX(), GetEnd().GetX());
            }
        }

        virtual T MinY() const {
            if (m_startAngle < 270 &&
                m_endAngle > 270) {
                return m_center.GetY()-m_radius;
            } else {
                return fmin(GetStart().GetY(), GetEnd().GetY());
            }
        }
        virtual T MaxX() const {
            if (m_startAngle > m_endAngle) {
                return m_center.GetX()+m_radius;
            } else {
                return fmax(GetStart().GetX(), GetEnd().GetX());
            }
        }
        virtual T MaxY() const {
            if (m_startAngle < 90 &&
                m_endAngle > 90) {
                return m_center.GetY()+m_radius;
            } else {
                return fmax(GetStart().GetY(), GetEnd().GetY());
            }
        }

        virtual void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;
            output.DrawArc(GetStart().GetX()*scale, GetStart().GetY()*scale,
                           GetEnd().GetX()*scale, GetEnd().GetY()*scale,
                           GetCenter().GetX()*scale, GetCenter().GetY()*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const override {
            return std::make_shared<Arc<T>>(*this);
        }

    private:
        T m_radius;
        T m_startAngle;
        T m_endAngle;
        Point<T> m_center;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Arc<T> const& rhs) {
    os << "Arc {" 
       << "r" << rhs.GetRadius() << " @" << rhs.GetCenter() << ", \\" << rhs.GetStartAngle() << " /" << rhs.GetEndAngle()
       << "s " << rhs.GetStart() << ", e " << rhs.GetEnd()
       << "}";
    return os;
}

#endif
