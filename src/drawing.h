#ifndef DRAWING_H_
#define DRAWING_H_

#include <drawingcomponent.h>
#include <line.h>
#include <linesegment.h>
#include <point.h>
#include <list>


template <typename T>
class DrawingBase {
    public: //types
        typedef std::list<std::shared_ptr<DrawingComponent<T> > > component_list_t;

    public:
        DrawingBase() : m_first(true), m_haveCenter(false) {
        }

        DrawingBase(DrawingBase const& src) : m_first(true), m_haveCenter(false) {
            for (typename component_list_t::const_iterator idx=src.m_components.begin();
                   idx != src.m_components.end();
                   ++idx) {
                m_components.push_back(idx->clone());
            }
        }

        void Draw(wxDC& output, T scale) const {
            for (typename component_list_t::const_iterator idx=m_components.begin();
                 idx != m_components.end();
                 ++idx) {
                (*idx)->Draw(output, scale);
            }
        }

        std::shared_ptr<DrawingBase> Reduce(T factor, wxDC& output, T scale) const {
            T grad=2;
            pt_base<T> center = GetCenter();
            std::shared_ptr<DrawingBase> result(new DrawingBase);
            bool firstPass(true);
            pt_base<T> first;
            pt_base<T> prev;
            for (T angle=0; angle < 360.0L; angle+=grad) {
                RayBase<T> ray(center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                //first in the list should be the closest
                T origLen = Len(collissions.front(), center);
                pt_base<T> newPt = ray.GetPoint(origLen*factor);

                if (!firstPass) {
                    result->AddComponent(std::make_shared<LineSegmentBase<T>>(prev, newPt));
                }

                prev = newPt;
                if (firstPass) {
                    first=newPt;
                    firstPass = false;
                }
            }
            //close the drawing
            result->AddComponent(std::make_shared<LineSegmentBase<T>>(first, prev));
            return result->Smoothe(factor, output, scale);;
        }

        std::shared_ptr<DrawingBase<T>> Smoothe(T factor, wxDC& output, T scale) {
            //fit an elipse
            T a = (MaxX()+MinX())/2;
            T b = (MaxY()+MinY())/2;

            T fitnessCur = Fitness(a,b);
            T fitnessPrev = 1e90;
            std::cout << "Fitness is " << fitnessPrev << std::endl;

            pt_base<T> center = GetCenter();

            T stepsize=0.1;

            T aDir =-stepsize;
            T bDir =-stepsize;

            while (fitnessPrev > fitnessCur) {
                a+=aDir;
                fitnessPrev=fitnessCur;
                fitnessCur = Fitness(a,b);
                //output.DrawEllipse((center.x-a)*scale, (center.y-b)*scale,2*a*scale, 2*b*scale);
                if (fitnessPrev< fitnessCur) {
                    aDir=-aDir/2; //reverseDirection
                    a+=aDir;
                }

                b+=bDir;
                fitnessCur = Fitness(a,b);
                //output.DrawEllipse((center.x-a)*scale, (center.y-b)*scale,2*a*scale, 2*b*scale);
                if (fitnessPrev< fitnessCur) {
                    bDir=-bDir/2; //reverseDirection
                    b+=bDir;
                }
            }
            std::cout << "Fitness " << fitnessCur << std::endl;
            //output.DrawEllipse((center.x-a)*scale, (center.y-b)*scale,2*a*scale, 2*b*scale);

            //find intersection with elipse and existing drawing
            std::shared_ptr<DrawingBase<T>> result(new DrawingBase<T>);
            bool firstPass(true);
            pt_base<T> first;
            pt_base<T> prev;
            T grad=2.0;
            for (T angle=0; angle < 360.0L; angle+=grad) {
                RayBase<T> ray(center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                //first in the list should be the closest
                T origLen = Len(collissions.front(), center);

                T elipseX = a*cos(angle*M_PI/180.0);
                T elipseY = b*sin(angle*M_PI/180.0);
                T elipseLen = sqrt(pow(elipseX,2)+pow(elipseY,2));

                T delta = elipseLen-origLen;
                //T adj = delta*(1-pow(factor,3));
                T adj = delta*(1-factor);
                T adjLen = origLen+adj;

                pt_base<T> newPt = ray.GetPoint(adjLen);

                if (!firstPass) {
                    result->AddComponent(std::make_shared<LineSegmentBase<T>>(prev, newPt));
                }

                prev = newPt;
                if (firstPass) {
                    first=newPt;
                    firstPass = false;
                }
            }
            //close the drawing
            output.DrawEllipse((center.x-a)*scale, (center.y-b)*scale,2*a*scale, 2*b*scale);
            result->AddComponent(std::make_shared<LineSegmentBase<T>>(first, prev));

            //result->Draw(output, scale);

            //at intersection points, adjust
            return result;
        }

        T Fitness(T a, T b) const {
            T total=0.0;
            uint32_t num=0;
            pt_base<T> center = GetCenter();
            for (T angle=0; angle < 360.0L; angle+=5.0) {
                RayBase<T> ray(m_center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                //first in the list should be the closest
                T colLen = Len(collissions.front(), center);
                T elipseX = a*cos(angle*M_PI/180.0);
                T elipseY = b*sin(angle*M_PI/180.0);
                T elipseLen = sqrt(pow(elipseX,2)+pow(elipseY,2));
                total += pow((elipseLen-colLen),2);
                ++num;
            }

            return sqrt(total/num);
        }

        void AddComponent(std::shared_ptr<DrawingComponent<T> > component) {
            if (m_first) {
                m_minX = component->MinX();
                m_minY = component->MinY();
                m_maxX = component->MaxX();
                m_maxY = component->MaxY();
                m_first=false;
            } else {
                m_minX = fmin(m_minX, component->MinX());
                m_minY = fmin(m_minY, component->MinY());
                m_maxX = fmax(m_maxX, component->MaxX());
                m_maxY = fmax(m_maxY, component->MaxY());
            }
            m_components.push_back(component);
            m_haveCenter=false;
        }

        T MaxX() const {return m_maxX;};
        T MaxY() const {return m_maxY;};
        T MinX() const {return m_minX;};
        T MinY() const {return m_minY;};

        void Print(std::ostream& os) const {
            os << "Drawing {" << std::endl;

            for (auto idx = m_components.begin(); idx != m_components.end(); ++idx) {
                (*idx)->Print(os);
                os << std::endl;
            }
            os << "}";
        }

        pt_base<T> GetCenter() const {
            if (!m_haveCenter) {
                T xCenter = CalculateCenter();
                m_center = pt_base<T>(xCenter, (m_maxY-m_minY)/2+m_minY);
                m_haveCenter=true;
            }
            return m_center;
        }

        T CalculateArea(T stepSize=0.1) const {
            T totalArea = 0;
            //std::cout << "Calculating area over interval " << m_minX << " <-> " << m_maxX << std::endl;
            for (T pos=m_minX; pos < m_maxX; pos+=stepSize) {
                std::list<T> collissions;
                LineBase<T> curLine(pos, -100.0, pos, 100.0);
                //find collission with each item
                GetCollissions(curLine, collissions);

                T tempArea = GetAreaFromIntersections(stepSize, collissions);
                totalArea += tempArea;
                //std::cout << "Area " << totalArea << " pos=" << pos << std::endl;
            }
            return totalArea;
        }


        void GetCollissions(LineBase<T> const& line, std::list<T>& collissions) const {
            //std::cout << "Testing for intersection with " << line << std::endl;
            for (auto idx=m_components.begin();
                 idx != m_components.end();
                 ++idx) {
                /*std::cout << "Checking ";
                (*idx)->Print(std::cout);
                std::cout << " for collission" << std::endl;*/
                
                std::list<pt_base<T>> intersections = (*idx)->Intersection(line);
                /*if (!intersections.empty()) {
                    std::cout << "Collissions with ";
                    (*idx)->Print(std::cout);
                    std::cout << std::endl;
                }*/

                for (auto idx2=intersections.begin();
                        idx2 != intersections.end();
                        ++idx2) {
                    collissions.push_back(idx2->y);
                }
            }
            collissions.sort();
            /*if (collissions.size() > 1) {
                std::cout << "Detected " << collissions.size() << " with line " << line << std::endl;
                for (auto idx=collissions.begin(); idx != collissions.end(); ++idx) {
                    std::cout << *idx << ",";
                }
                std::cout << std::endl;
            }*/
        }

        void GetCollissions(RayBase<T> const& ray, std::list<pt_base<T>>& collissions, pt_base<T> center) const {
            for (auto idx=m_components.begin();
                 idx != m_components.end();
                 ++idx) {
                
                std::list<pt_base<T>> intersections = (*idx)->Intersection(ray);

                for (auto idx2=intersections.begin();
                        idx2 != intersections.end();
                        ++idx2) {
                    collissions.push_back(*idx2);
                }
            }
            collissions.sort(DistanceSort<double>(center));
        }

        T GetAreaFromIntersections(T width, std::list<T> const& collissions) const {
            T result = 0;
            if (collissions.size()%2 ==1) {
                //std::cout << "collissions are odd, (" << collissions.size() << ") fugedaboutit" << std::endl;
                return 0;
            }

            for (typename std::list<T>::const_iterator idx = collissions.begin();
                    idx != collissions.end();
                    ) {
                typename std::list<T>::const_iterator idx2 =idx;
                ++idx;
                T len = (*idx)-(*idx2);
                result += len*width;
                //std::cout << "Result " << result << " (len=" << len << ") first=" << *idx2 << " second=" << *idx <<  std::endl;
                ++idx;
            }
            return result;
        }

    private:
        T CalculateCenter() const {
            std::list<std::pair<T, T>> offsets;
            T totalArea = 0;
            const T stepSize = 0.01;
            std::cout << "Calculating area over interval " << m_minX << " <-> " << m_maxX << std::endl;
            for (T pos=m_minX; pos < m_maxX; pos+=stepSize) {
                std::list<T> collissions;
                LineBase<T> curLine(pos, -100.0, pos, 100.0);
                //find collission with each item
                GetCollissions(curLine, collissions);
                // if value is odd??

                T tempArea = GetAreaFromIntersections(stepSize, collissions);
                totalArea += tempArea;
                offsets.push_back(std::pair<T,T>(pos, totalArea));
                //std::cout << "Area " << totalArea << " pos=" << pos << std::endl;
            }
            //std::cout << "Total area is " << totalArea << std::endl;

            for (auto idx = offsets.begin(); idx != offsets.end(); ++idx) {
                //std::cout << "Pos " << idx->first << " area " << idx->second << std::endl;
                if (idx->second>=totalArea/2) {
                    return idx->first;
                }
            }
            return (m_maxX-m_minX)/2.0;
        }



    private:
        bool m_first;
        T m_minX;
        T m_minY;
        T m_maxX;
        T m_maxY;
        component_list_t m_components;
        mutable bool m_haveCenter;
        mutable pt_base<T> m_center;
        
};

typedef DrawingBase<double> Drawing;

#endif
