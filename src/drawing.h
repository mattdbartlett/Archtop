#ifndef DRAWING_H_
#define DRAWING_H_

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
            return std::pair<T,T>((m_maxX-m_minX)/2+m_minX, (m_maxY-m_minY)/2+m_minY);
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
