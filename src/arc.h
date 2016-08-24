#ifndef ARC_H_
#define ARC_H_

#include <drawingcomponent.h>
#include <memory>
#include <types.h>

class DLArcData;


template <typename T>
class ArcBase : public DrawingComponent<T> {
    public:
        ArcBase() : m_radius(0), m_startAngle(0), m_endAngle(0) {
        }

        ArcBase(T radius, 
            T startAngle,
            T endAngle,
            pt_base<T> const& center) 
            : m_radius(radius)
            , m_startAngle(startAngle)
            , m_endAngle(endAngle)
            , m_center(center) {
        }

        ArcBase(DL_ArcData const& arcData) 
         : m_radius(arcData.radius)
         , m_startAngle(arcData.angle1)
         , m_endAngle(arcData.angle2)
         , m_center(arcData.cx, arcData.cy) {
        }

        virtual ~ArcBase() {};

        T GetRadius() const {return m_radius;}
        T GetStartAngle() const {return m_startAngle;}
        T GetEndAngle() const {return m_endAngle;}
        pt_base<T> GetCenter() const {return m_center;}

        pt_base<T> GetStart() const {
            T startX = m_center.x+m_radius*cos(ToRadians(m_startAngle));
            T startY = m_center.y+m_radius*sin(ToRadians(m_startAngle));

            return pt_base<T>(startX, startY);
        }

        pt_base<T> GetEnd() const {
            T endX = m_center.x+m_radius*cos(ToRadians(m_endAngle));
            T endY = m_center.y+m_radius*sin(ToRadians(m_endAngle));

            return pt_base<T>(endX, endY);
        }

        //implement drawingcomponent
        //0 is positive X origin, increasing counter-clockwise
        virtual T MinX() const {
            if (m_startAngle < 180 &&
                m_endAngle > 180) {
                return m_center.x-m_radius;
            } else {
                return fmin(GetStart().x, GetEnd().x);
            }
        }

        virtual T MinY() const {
            if (m_startAngle < 270 &&
                m_endAngle > 270) {
                return m_center.y-m_radius;
            } else {
                return fmin(GetStart().y, GetEnd().y);
            }
        }
        virtual T MaxX() const {
            if (m_startAngle > m_endAngle) {
                return m_center.x+m_radius;
            } else {
                return fmax(GetStart().x, GetEnd().x);
            }
        }
        virtual T MaxY() const {
            if (m_startAngle < 90 &&
                m_endAngle > 90) {
                return m_center.y+m_radius;
            } else {
                return fmax(GetStart().y, GetEnd().y);
            }
        }

        virtual void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;
            output.DrawArc(GetStart().x*scale, GetStart().y*scale,
                           GetEnd().x*scale, GetEnd().y*scale,
                           GetCenter().x*scale, GetCenter().y*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const override {
            return std::make_shared<ArcBase<T>>(*this);
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
            //special case for vertical lines
            if (line.IsVertical())
            {
                T xVal = line.GetP1().x;
                T a = 1;
                T b = -2*m_center.y;
                T c = pow(m_center.y,2)-pow(m_radius,2)+pow(xVal,2)-2*m_center.x*xVal+pow(m_center.x,2);
                std::list<T> yVals = SolveQuadratic(a,b,c);
                for (auto idx = yVals.begin(); idx != yVals.end(); ++idx) {
                    result.push_back(pt_base<T>(xVal, *idx));
                }
            }
            else
            {
                pt_base<T> p1(line.GetP1());
                pt_base<T> p2(line.GetP2());
                T slope = (p1.y-p2.y)/(p1.x-p2.x);
                T yInt = p1.y-slope*p1.x;

                //solve solution using the quadratic formula
                T a = 1+pow(slope,2);
                T b = 2*(slope*yInt-slope*m_center.y-m_center.x);
                T c = pow(m_center.y,2)+pow(m_center.x,2)+pow(yInt,2)-pow(m_radius,2)-2*yInt*m_center.y;

                std::list<T> xVals = SolveQuadratic(a,b,c);

                for (auto idx = xVals.begin(); idx != xVals.end(); ++idx) {
                    T y = slope*(*idx)+yInt;
                    result.push_back(pt_base<T>(*idx, y));
                    //std::cout << "Intercept " << *idx << "," << y << std::endl;
                }
            }

            //now check if these are on the arc
            result = GetPointsOnArc(result);
            
            return result;
        }

        virtual void Print(std::ostream& os) const {
            os << *this;
        }

        std::list<T> SolveQuadratic(T a, T b, T c) const {
            std::list<T> results;
            T descriminant = pow(b,2)-4*a*c;
            //descriminiant < 0 is no intersection
            // descriminant == 0 is a tangent
            // we don't care about either
            if (descriminant > 0) {
                T descriminantSqrt = sqrt(descriminant);
                results.push_back((-b+descriminantSqrt)/(2*a));
                results.push_back((-b-descriminantSqrt)/(2*a));
            }
            return results;
        }

        std::list<pt_base<T>> GetPointsOnArc(std::list<pt_base<T>> const& vals) const {
            std::list<pt_base<T>> result;
            for (auto idx = vals.begin(); idx != vals.end(); ++idx) {
                T angle = FromRadians(atan2((idx->y-m_center.y), (idx->x-m_center.x)));
                //atan2 returns an angle in the range pi to -pi (and we then convert to degrees).
                if (angle < 0) {
                    angle+=360.0L;
                }
                //std::cout << "Angle = " << angle << std::endl;
                if (m_startAngle > m_endAngle) {
                    T adj = fmod(360.0L-m_startAngle, 360.0L);
                    T endAdj = fmod(m_endAngle+adj,360.0L);
                    T angleAdj = fmod(angle+adj,360.0L);
                    if (angleAdj <= endAdj && angleAdj >= 0) {
                        result.push_back(*idx);
                    }

                } else if (angle >= m_startAngle && angle <=m_endAngle) {
                    result.push_back(*idx);
                }

            }
            
            return result;
        }


    private:

    private:
        T m_radius;
        T m_startAngle;
        T m_endAngle;
        pt_base<T> m_center;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, ArcBase<T> const& rhs) {
    os << "Arc {" 
       << "r" << rhs.GetRadius() << " @" << rhs.GetCenter() << ", \\" << rhs.GetStartAngle() << " /" << rhs.GetEndAngle()
       << "s " << rhs.GetStart() << ", e " << rhs.GetEnd()
       << "}";
    return os;
}

typedef ArcBase<double> Arc;

#endif
