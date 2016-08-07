#ifndef DRAWINGCOMPONENT_H_
#define DRAWINGCOMPONENT_H_

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
};

/*template <class D, class T>
D& operator<<(D& output, T const& arg) {
    arg.Draw(output);
    return output;
}*/

#endif
