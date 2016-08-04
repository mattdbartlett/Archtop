#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>


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
            std::cout << "Found a line " << lineData << std::endl;
        }

        virtual void addArc(DL_ArcData const& arcData) {
            std::cout << "Found an arc " << std::endl; //arcData << std::endl;
        }
        virtual void addPoint(DL_PointData const& pointData) {
            std::cout << "Found a point " << std::endl; //arcData << std::endl;
        }
    private:
};

int main(void) {
    Creation creation;

    DL_Dxf dxf;

    dxf.in("test.dxf", &creation);

    return 0;
}
