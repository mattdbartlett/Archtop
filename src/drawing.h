#ifndef DRAWING_H_
#define DRAWING_H_

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

        std::pair<T, T> GetCenter() const {
            CalculateCenter();
            return std::pair<T,T>((m_maxX-m_minX)/2+m_minX, (m_maxY-m_minY)/2+m_minY);
        }

    private:
        T CalculateCenter() const {
            std::list<std::pair<T, T>> offsets;
            T totalArea;
            const T stepSize = 0.01;
            for (T pos=m_minX; pos < m_maxX; pos+=stepSize) {
                std::list<T> collissions;
                Line<T> curLine(0,pos);
                //find collission with each item
                GetCollissions(curLine, collissions);
                // if value is odd??

                T tempArea = GetAreaFromIntersections(stepSize, collissions);
                offsets.push_back(std::pair<T,T>(pos, tempArea));
                totalArea = tempArea;
            }
            std::cout << "Total area is " << totalArea << std::endl;
            return totalArea;
        }

        void GetCollissions(Line<T> const& line, std::list<T>& collissions) const {
            for (typename component_list_t::const_iterator idx=m_components.begin();
                 idx != m_components.end();
                 ++idx) {
                 std::list<std::pair<T,T>> intersections = (*idx)->Intersection(line);
                 if (!intersections.empty()) {
                     std::cout << "Collissions with ";
                     (*idx)->Print(std::cout);
                     std::cout << std::endl;
                 }

                 for (typename std::list<std::pair<T,T>>::const_iterator idx2=intersections.begin();
                         idx2 != intersections.end();
                         ++idx2) {
                     collissions.push_back(idx2->second);
                 }
            }
            collissions.sort();
        }

        T GetAreaFromIntersections(T width, std::list<T> const& collissions) const {
            T result = 0;
            if (collissions.size()%2 ==1) {
                std::cout << "collissions are odd, (" << collissions.size() << ") fugedaboutit" << std::endl;
                return 0;
            }

            for (typename std::list<T>::const_iterator idx = collissions.begin();
                    idx != collissions.end();
                    ) {
                typename std::list<T>::const_iterator idx2 =idx;
                ++idx;
                result += (*idx2)-(*(idx))*width;
                ++idx;
            }
            return result;
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
