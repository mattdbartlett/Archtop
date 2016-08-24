#ifndef PARSEDXF_H_
#define PARSEDXF_H_

#include <point.h>
#include <linesegment.h>
#include <arc.h>
#include <dxflib/dl_creationadapter.h>
#include <drawing.h>

template <typename T>
class ParseDXFBase : public DL_CreationAdapter {
    public:
        ParseDXFBase() {}
        virtual ~ParseDXFBase() {}

        virtual void addLine(DL_LineData const& lineData) {
            m_drawing.AddComponent(std::make_shared<LineSegmentBase<T>>(lineData));
        }

        virtual void addArc(DL_ArcData const& arcData) {
            m_drawing.AddComponent(std::make_shared<ArcBase<T>>(arcData));
        }

        virtual void addPoint(DL_PointData const& pointData) {
            m_drawing.AddComponent(std::make_shared<PointBase<T>>(pointData));
        }

        DrawingBase<T> const& GetDrawing() const {return m_drawing;}

    private:
        DrawingBase<T> m_drawing;
        
};

typedef ParseDXFBase<double> ParseDXF;

#endif
