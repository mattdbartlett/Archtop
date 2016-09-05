#ifndef DRAWING_H_
#define DRAWING_H_

#include <drawingcomponent.h>
#include <line.h>
#include <linesegment.h>
#include <point.h>
#include <list>
#include <map>
#include <ellipse.h>
#include <types.h>

//#define DEBUG 1


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

        std::shared_ptr<DrawingBase> Reduce(FactorBase<T> const& factor, wxDC& output, T scale) const {
            T grad=2;
            pt_base<T> center = GetCenter();
            std::shared_ptr<DrawingBase> result(new DrawingBase);
            bool firstPass(true);
            pt_base<T> first;
            pt_base<T> prev;
            for (T angle=0.5; angle < 359.5L; angle+=grad) {
                RayBase<T> ray(center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                //first in the list should be the closest
                T origLen = Len(collissions.front(), center);
                pt_base<T> newPt = ray.GetPoint(origLen*factor.scaling);

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
            #ifdef DEBUG
            result->Draw(output, scale);
            #endif
            return result->Smoothe(factor, output, scale);;
        }

        std::shared_ptr<DrawingBase<T>> Smoothe(FactorBase<T> const& factor, wxDC& output, T scale) {


            std::shared_ptr<EllipseBase<T>> ellipse = FitEllipse();

            pt_base<T> center = GetCenter();
            
            std::cout << "Smoothing drawing with (" << MinX() << "," << MinY() << ")"
                                             << "(" << MaxX() << "," << MaxY() << ")" 
                                             << std::endl;

            //find intersection with elipse and existing drawing
            std::shared_ptr<DrawingBase<T>> result(new DrawingBase<T>);
            bool firstPass(true);
            pt_base<T> first;
            pt_base<T> prev;
            T grad=2.0;
            for (T angle=1.0; angle < 359.0L; angle+=grad) {
                RayBase<T> ray(center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                #ifdef DEBUG
                output.DrawLine(center.x*scale, center.y*scale, collissions.front().x*scale, collissions.front().y*scale);
                #endif

                //first in the list should be the closest
                T origLen = Len(collissions.front(), center);

                auto ellipseIntersect = ellipse->Intersection(ray);
                pt_base<T> ellipseInt = ellipseIntersect.front();
                
                #ifdef DEBUG
                wxPen tempPen = output.GetPen();
                wxPen redPen(*wxRED, 2);
                output.SetPen(redPen);
                output.DrawLine(collissions.front().x*scale, collissions.front().y*scale, 
                                ellipseInt.x*scale, ellipseInt.y*scale);
                output.SetPen(tempPen);
                #endif

                T elipseLen = Len(center, ellipseInt);

                T delta = elipseLen-origLen;
                //T adj = delta*(1-pow(factor,3));
                T adj = delta*(1-factor.smoothing);
                T adjLen = origLen+adj;

                pt_base<T> newPt = ray.GetPoint(adjLen);
                std::cout << "Delta " << delta << ", ellipseLen " 
                          << elipseLen <<  ", origLen " << origLen 
                          << ", newlen " << Len(newPt, center) << std::endl;

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
            #ifdef DEBUG
            ellipse->Draw(output, scale);
            #endif
            result->AddComponent(std::make_shared<LineSegmentBase<T>>(first, prev));

            //result->Draw(output, scale);

            //at intersection points, adjust
            return result;
        }

        //Find an ellipse that is a best fit for the current drawing
        std::shared_ptr<EllipseBase<T>> FitEllipse() const {
            //fit an elipse
            T a = (MaxX()-MinX())/2;
            T b = (MaxY()-MinY())/2;
            pt_base<T> curCenter = GetCenter();

            std::shared_ptr<EllipseBase<T>> start(new EllipseBase<T>(a, b, curCenter));
            std::shared_ptr<EllipseBase<T>> fittest = start;
            T fitnessCur = Fitness(start);
            T fitnessPrev = 1e90;
            T stepsize=0.1;

            while (fitnessPrev > fitnessCur) {
                fitnessPrev = fitnessCur;
                //adjust center
                auto left =  std::make_shared<EllipseBase<T>>(fittest->GetXRadius(), 
                                                              fittest->GetYRadius(), 
                                                              pt_base<T>(fittest->GetCenter().x-stepsize, fittest->GetCenter().y));
                auto right =  std::make_shared<EllipseBase<T>>(fittest->GetXRadius(), 
                                                              fittest->GetYRadius(), 
                                                              pt_base<T>(fittest->GetCenter().x+stepsize, fittest->GetCenter().y));
                std::list<std::shared_ptr<EllipseBase<T>>> selection{left, right, fittest};
                fittest = Fittest(selection);

                //adjust radius
                selection.clear();

                T fxr = fittest->GetXRadius();
                T fyr = fittest->GetYRadius();
                pt_base<T> center = fittest->GetCenter();

                selection.push_back(std::make_shared<EllipseBase<T>>(fxr+stepsize,fyr+stepsize, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr+stepsize,fyr, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr+stepsize,fyr-stepsize, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr-stepsize,fyr+stepsize, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr-stepsize,fyr, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr-stepsize,fyr-stepsize, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr,fyr+stepsize, center));
                selection.push_back(std::make_shared<EllipseBase<T>>(fxr,fyr-stepsize, center));

                fittest = Fittest(selection);
                fitnessCur = Fitness(fittest);
            }
            return fittest;
        }

        typedef std::multimap<T, std::shared_ptr<EllipseBase<T>>> fitness_map_t;

        std::shared_ptr<EllipseBase<T>> Fittest(std::list<std::shared_ptr<EllipseBase<T>>> selections) const {
            fitness_map_t fitnessMap;
            for (auto idx = selections.begin(); idx != selections.end(); ++idx) {
                T fitness = Fitness(*idx);
                fitnessMap.insert(typename fitness_map_t::value_type(fitness,*idx));
            }
            return fitnessMap.begin()->second;
        }

        T Fitness(std::shared_ptr<EllipseBase<T>> ellipse) const {
            T total=0.0;
            uint32_t num=0;
            pt_base<T> center = GetCenter();
            for (T angle=1.0; angle < 359.0L; angle+=10.0) {
                RayBase<T> ray(center, angle);
                std::list<pt_base<T>> collissions;
                GetCollissions(ray, collissions, center);

                //first in the list should be the closest
                T colLen = Len(collissions.front(), center);
                auto ellipseIntersect = ellipse->Intersection(ray);
                if (ellipseIntersect.empty()) {
                    throw std::runtime_error("No intersection with ellipse");
                }

                pt_base<T> ellipseInt = ellipseIntersect.front();
                T ellipseLen = Len(ellipseInt, center);
                total += pow((ellipseLen-colLen),2);
                ++num;
            }

            T fitness = sqrt(total/num);
            return fitness;
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
