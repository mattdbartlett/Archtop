#ifndef LINESEGMENT_H_
#define LINESEGMENT_H_

#include <drawingcomponent.h>
#include <point.h>
#include <math.h>

template <typename T>
class LineSegment : public DrawingComponent<T> {
    public:
        LineSegment() {
            CalculateParameters();
        }
        LineSegment(Point<T> const& p1, Point<T> const& p2) : m_p1(p1), m_p2(p2) {
            CalculateParameters();
        }
        LineSegment(DL_LineData const& lineData) 
            : m_p1(lineData.x1, lineData.y1)
            , m_p2(lineData.x2, lineData.y2) {
            CalculateParameters();
        }

        virtual ~LineSegment() {}

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
            return std::make_shared<LineSegment<T>>(*this);
        }

        virtual std::list<std::pair<T, T>> Intersection(Ray<T> const& ray) const {
            
            std::pair<T, T> intersect(0,0);
            std::list<std::pair<T,T>> result;
            if (Intersection(ray.GetSlope(), ray.GetIntercept(), intersect)) {
                //for a ray, we also have to check that the intersection point lies on
                // correct portion
                int quadrant = fmod(ray.GetAngle(),360)/90;
                if (quadrant == 0 ||
                    quadrant == 3) {
                    if (intersect.first > ray.GetStart().first) {
                        result.push_back(intersect);
                    }
                } else if (intersect.first < ray.GetStart().first) {
                    result.push_back(intersect);
                }
            }
            return result;
        }

        virtual std::list<std::pair<T, T>> Intersection(Line<T> const& line) const {
            std::pair<T, T> intersect(0,0);
            std::list<std::pair<T,T>> result;
            if (Intersection(line.GetSlope(), line.GetIntercept(), intersect)) {
                result.push_back(intersect);
            }
            return result;
        }

    private: //methods
        bool Intersection(T slope, T intercept, std::pair<T,T>& intersection) const {

            //X = (b2-b1)/(m1-m2)
            // if m1-m2 == 0 (identical slopes), lines do not intersect
            T deltaM = m_slope-slope;
            if (deltaM == 0) {
                return false;
            }
            T xIntersect = (intercept-m_intercept)/deltaM;
            T yIntersect = m_slope*xIntersect+m_intercept;
            intersection.first=xIntersect;
            intersection.second=yIntersect;

            if (xIntersect < MaxX() &&
                xIntersect > MinX()) {
                return true;
            }
            return false;
        }

        void CalculateParameters() {
            //calculate slope and intercept for this segment
            m_slope = 0;
            if ((m_p1.GetX()-m_p2.GetX()) != 0) {
              m_slope = (m_p1.GetY()-m_p2.GetY())/(m_p1.GetX()-m_p2.GetX());
            }
            m_intercept = m_p1.GetY()-m_slope*m_p1.GetX();
        }

    private: //members
        Point<T> m_p1;
        Point<T> m_p2;
        T m_slope;
        T m_intercept;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, LineSegment<T> const& rhs) {
    os << "LineSegment {" << rhs.GetP1() << " <-> " << rhs.GetP2() << "}";
    return os;
}

#endif
