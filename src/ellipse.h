#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include <drawingcomponent.h>
#include <memory>
#include <types.h>
#include <utilities.h>


template <typename T>
class EllipseBase : public DrawingComponent<T> {
    public:
        EllipseBase() : m_xRadius(0), m_yRadius(0) {
        }

        EllipseBase(T xRadius, 
            T yRadius,
            T endAngle,
            pt_base<T> const& center) 
            : m_xRadius(xRadius)
            , m_yRadius(yRadius)
            , m_center(center) {
        }

        virtual ~EllipseBase() {};

        T GetXRadius() const {return m_xRadius;}
        T GetYRadius() const {return m_yRadius;}
        pt_base<T> GetCenter() const {return m_center;}

        //implement drawingcomponent
        //0 is positive X origin, increasing counter-clockwise
        virtual T MinX() const {
            return m_center.x-m_xRadius;
        }

        virtual T MinY() const {
            return m_center.y-m_yRadius;
        }
        virtual T MaxX() const {
            return m_center.x+m_xRadius;
        }
        virtual T MaxY() const {
            return m_center.y+m_yRadius;
        }

        virtual void Draw(wxDC& output, T scale) const {
            output.DrawEllipse(m_center.x*scale, m_center.y*scale, m_xRadius*scale, m_yRadius*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const override {
            return std::make_shared<EllipseBase<T>>(*this);
        }

        virtual std::list<pt_base<T>> Intersection(RayBase<T> const& ray) const {

            T angle = ToRadians(ray.GetAngle()); //get the angle, and convert to radians
            pt_base<T> pt2(1000*cos(angle), 1000*sin(angle));
            pt2 += ray.GetStart();

            LineBase<T> tempLine(ray.GetStart(), pt2);
            std::list<pt_base<T>> result;
            result = Intersection(tempLine);

            std::list<pt_base<T>> filtResult;
            for (auto idx=result.begin(); idx != result.end(); ++idx) {
                T angle = FromRadians(atan2(idx->y-ray.GetStart().y, idx->x-ray.GetStart().x));
                if (angle < 0) {
                    angle += 360.0L;
                }

                //std::cout << "ray angle " << ray.GetAngle() << " intercept angle " << angle << std::endl;
                if (fabs(angle-ray.GetAngle()) < 1.0L) {
                    filtResult.push_back(*idx);
                }
            }

            return filtResult;
        }

        virtual std::list<pt_base<T>> Intersection(LineBase<T> const& line) const {

            std::list<pt_base<T>> result;

            //translate the line based on center of the elipse so that the math can be simpler
            T offsetX = line.P2().x-line.P1().x;
            T offsetY = line.P2().y-line.P1().y;
            LineBase<T> offsetLine(m_center, pt_base<T>(m_center.x+offsetX, m_center.y+offsetY));

            //equation of ellipse
            // (x/a)^2+(y/b)^2=1

            //special case for vertical lines
            if (offsetLine.IsVertical())
            {
                //Y^2/b^2 = 1-(xVal/a)^2
                //Y^2= b^2(1-(xVal/a)^2)
                //Y = sqrt(b^2(1-(xVal/a)^2))
                T xVal = offsetLine.GetP1().x;
                T y = sqrt(pow(m_yRadius,2)*(1-pow((xVal/m_xRadius),2)));
                result.push_back(pt_base<T>(xVal-offsetX, y-offsetY));
                result.push_back(pt_base<T>(xVal,-offsetX -y-offsetY));
            }
            else
            {
                pt_base<T> p1(offsetLine.GetP1());
                pt_base<T> p2(offsetLine.GetP2());
                T slope = (p1.y-p2.y)/(p1.x-p2.x);
                T yInt = p1.y-slope*p1.x;

                //solve solution using the quadratic formula
                T a = 1/pow(m_xRadius,2)+pow(slope,2)/pow(m_yRadius,2);
                T b = 2*slope*yInt/pow(m_yRadius,2);
                T c = pow(yInt/m_yRadius,2)-1.0L;

                std::list<T> xVals = SolveQuadratic<T>(a,b,c);

                for (auto idx = xVals.begin(); idx != xVals.end(); ++idx) {
                    T y = slope*(*idx)+yInt;
                    result.push_back(pt_base<T>(*idx-offsetX, y-offsetY));
                    //std::cout << "Intercept " << *idx << "," << y << std::endl;
                }
            }

            return result;
        }

        virtual void Print(std::ostream& os) const {
            os << *this;
        }

    private:
        T m_xRadius;
        T m_yRadius;
        pt_base<T> m_center;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, EllipseBase<T> const& rhs) {
    os << "Ellipse {" 
       << "a " << rhs.GetXRadius() << ", b " << rhs.GetYRadius() << " @" << rhs.GetCenter() 
       << "}";
    return os;
}

typedef EllipseBase<double> Ellipse;

#endif
