#ifndef DRAWINGCOMPONENT_H_
#define DRAWINGCOMPONENT_H_

#include <line.h>
#include <ray.h>
#include <list>
#include <types.h>

template <typename T>
class DrawingComponent {
    public:
        virtual ~DrawingComponent() {}
        virtual T MinX() const = 0;
        virtual T MinY() const = 0;
        virtual T MaxX() const = 0;
        virtual T MaxY() const = 0;

        virtual void Draw(wxDC& output, T scale) const = 0;

        virtual std::shared_ptr<DrawingComponent<T>> clone() const = 0;

        /**
         * Determine the intersection point(s) between this object and the specified ray
         */
        virtual std::list<pt_base<T>> Intersection(RayBase<T> const& ray) const = 0;

        /**
         * Determine the intersection point(s) between this object and the specified line
         */
        virtual std::list<pt_base<T>> Intersection(LineBase<T> const& line) const = 0;

        virtual void Print(std::ostream& os) const = 0;
};

/*template <class D, class T>
D& operator<<(D& output, T const& arg) {
    arg.Draw(output);
    return output;
}*/

#endif
