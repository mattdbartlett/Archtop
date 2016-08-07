#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>

#include <point.h>
#include <line.h>
#include <arc.h>


std::ostream& operator<<(std::ostream& os, DL_LineData const& ld) {
    os << "{{" 
       << ld.x1 << "," << ld.y1 <<  "," << ld.z1 
       << "},{"
       << ld.x2 << "," << ld.y2 <<  "," << ld.z2
       <<  "}}";
    return os;
}

class Creation : public DL_CreationAdapter {
    public:
        Creation() {}
        virtual ~Creation() {}
        virtual void addLine(DL_LineData const& lineData) {
            std::cout << Line<double>(lineData) << std::endl;
        }

        virtual void addArc(DL_ArcData const& arcData) {
            std::cout << Arc<double>(arcData) << std::endl; //arcData << std::endl;
        }
        virtual void addPoint(DL_PointData const& pointData) {
            std::cout << Point<double>(pointData) << std::endl;
        }
    private:
};

int main(void) {
    Arc<double> arc;
    Line<double> line;

    Creation creation;

    DL_Dxf dxf;

    dxf.in("test.dxf", &creation);

    return 0;
}
