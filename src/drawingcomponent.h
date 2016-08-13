#ifndef DRAWINGCOMPONENT_H_
#define DRAWINGCOMPONENT_H_

#include <ray.h>
#include <line.h>
#include <list>

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
        virtual std::list<std::pair<T, T>> Intersection(Ray<T> const& ray) const = 0;

        /**
         * Determine the intersection point(s) between this object and the specified line
         */
        virtual std::list<std::pair<T,T>> Intersection(Line<T> const& line) const = 0;

        virtual void Print(std::ostream& os) const = 0;
};

/*template <class D, class T>
D& operator<<(D& output, T const& arg) {
    arg.Draw(output);
    return output;
}*/

#endif
