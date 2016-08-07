#ifndef PARSEDXF_H_
#define PARSEDXF_H_

#include <point.h>
#include <line.h>
#include <arc.h>
#include <dxflib/dl_creationadapter.h>
#include <drawing.h>

template <typename T>
class ParseDXF : public DL_CreationAdapter {
    public:
        ParseDXF() {}
        virtual ~ParseDXF() {}

        virtual void addLine(DL_LineData const& lineData) {
            m_drawing.AddComponent(std::make_shared<Line<T>>(lineData));
        }

        virtual void addArc(DL_ArcData const& arcData) {
            m_drawing.AddComponent(std::make_shared<Arc<T>>(arcData));
        }

        virtual void addPoint(DL_PointData const& pointData) {
            m_drawing.AddComponent(std::make_shared<Point<T>>(pointData));
        }

        Drawing<T> const& GetDrawing() const {return m_drawing;}

    private:
        Drawing<T> m_drawing;
        
};

#endif
