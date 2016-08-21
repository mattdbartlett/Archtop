#ifndef LINESEGMENT_H_
#define LINESEGMENT_H_

#include <wx/dc.h>
#include <dl_entities.h>
#include <memory>
#include <drawingcomponent.h>
#include <math.h>
#include <types.h>

template <typename T>
class LineSegment : public DrawingComponent<T> {
    public:
        LineSegment() {
        }
        LineSegment(pt_base<T> const& p1, pt_base<T> const& p2) : m_p1(p1), m_p2(p2) {
        }
        LineSegment(T x1, T y1, T x2, T y2): m_p1(x1,y1), m_p2(x2,y2) {}
        LineSegment(DL_LineData const& lineData) 
            : m_p1(lineData.x1, lineData.y1)
            , m_p2(lineData.x2, lineData.y2) {
        }

        virtual ~LineSegment() {}

        pt_base<T> GetP1() const {return m_p1;};
        pt_base<T> GetP2() const {return m_p2;};

        virtual T MinX() const {return fmin(m_p1.x, m_p2.x);}
        virtual T MinY() const {return fmin(m_p1.y, m_p2.y);}
        virtual T MaxX() const {return fmax(m_p1.x, m_p2.x);}
        virtual T MaxY() const {return fmax(m_p1.y, m_p2.y);}

        virtual void Draw(wxDC& output, T scale) const {
            std::cerr << *this << std::endl;
            output.DrawLine(m_p1.x*scale, m_p1.y*scale, m_p2.x*scale, m_p2.y*scale);
        }

        virtual std::shared_ptr<DrawingComponent<T>> clone() const {
            return std::make_shared<LineSegment<T>>(*this);
        }

        virtual std::list<pt_base<T>> Intersection(Ray<T> const& ray) const {
            
            pt_base<T> intersect;
            std::list<pt_base<T>> result;
            
            //calculate a second point on the ray for use when finding the intercept
            T angle = ray.GetAngle()*M_PI/180.0L; //get the angle, and convert to radians
            pt_base<T> temp(1000*cos(angle), 1000*sin(angle));
            if (::LineIntersection(ray.GetStart(), temp, m_p1, m_p2, intersect)) {
                if (intersect.x <= MaxX() && intersect.x >= MinX() &&
                    intersect.y <= MaxY() && intersect.y >= MinY()) {
                    //for a ray, we also have to check that the intersection point lies on
                    // correct portion
                    int quadrant = fmod(ray.GetAngle(),360)/90;
                    if (quadrant == 0 ||
                        quadrant == 3) {
                        if (intersect.x > ray.GetStart().x) {
                            result.push_back(intersect);
                        }
                    } else if (intersect.x < ray.GetStart().x) {
                        result.push_back(intersect);
                    }
                }
            }
            return result;
        }

        virtual std::list<pt_base<T>> Intersection(Line<T> const& line) const {
            pt_base<T> intersect;
            std::list<pt_base<T>> result;
            if (LineIntersection(line.GetP1(), line.GetP2(), m_p1, m_p2,  intersect)) {
                //Check that the intersection point lies on the segment
                bool goodX = InRange(intersect.x, MinX(), MaxX());
                bool goodY = InRange(intersect.y, MinY(), MaxY());
                if (goodX &&
                    goodY) {
                    result.push_back(intersect);
                    std::cout << intersect.x << " is " << (goodX ? "" : "not") << " between " << MinX() << " and " << MaxX() << std::endl;
                    std::cout << intersect.y << " is " << (goodY ? "" : "not") << " between " << MinY() << " and " << MaxY() << std::endl;
                }
            }
            return result;
        }

        virtual void Print(std::ostream& os) const {
            os << *this;
        }

    private: //methods

    private: //members
        pt_base<T> m_p1;
        pt_base<T> m_p2;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, LineSegment<T> const& rhs) {
    os << "LineSegment {" << rhs.GetP1() << " <-> " << rhs.GetP2() << "}";
    return os;
}

#endif
