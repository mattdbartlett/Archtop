#ifndef DRAWING_H_
#define DRAWING_H_

#include <drawingcomponent.h>
#include <line.h>
#include <list>

template <typename T>
class Drawing {
    public: //types
        typedef std::list<std::shared_ptr<DrawingComponent<T> > > component_list_t;

    public:
        Drawing() : m_first(true) {
        }

        Drawing(Drawing const& src) : m_first(true) {
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
            T xCenter = CalculateCenter();
            return pt_base<T>(xCenter, (m_maxY-m_minY)/2+m_minY);
        }

        T CalculateArea(T stepSize=0.1) const {
            T totalArea = 0;
            //std::cout << "Calculating area over interval " << m_minX << " <-> " << m_maxX << std::endl;
            for (T pos=m_minX; pos < m_maxX; pos+=stepSize) {
                std::list<T> collissions;
                Line<T> curLine(pos, -100.0, pos, 100.0);
                //find collission with each item
                GetCollissions(curLine, collissions);

                T tempArea = GetAreaFromIntersections(stepSize, collissions);
                totalArea += tempArea;
                //std::cout << "Area " << totalArea << " pos=" << pos << std::endl;
            }
            return totalArea;
        }


        void GetCollissions(Line<T> const& line, std::list<T>& collissions) const {
            std::cout << "Testing for intersection with " << line << std::endl;
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
                Line<T> curLine(pos, -100.0, pos, 100.0);
                //find collission with each item
                GetCollissions(curLine, collissions);
                // if value is odd??

                T tempArea = GetAreaFromIntersections(stepSize, collissions);
                totalArea += tempArea;
                offsets.push_back(std::pair<T,T>(pos, totalArea));
                //std::cout << "Area " << totalArea << " pos=" << pos << std::endl;
            }
            std::cout << "Total area is " << totalArea << std::endl;

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
};

#endif
