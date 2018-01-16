#ifndef IPROFILE_H_
#define IPROFILE_H_

template <typename T>
class IProfile {
    public:
        virtual ~IProfile() {}
        virtual Factor operator(T height);
        virtual T Smoothing(T height) const = 0;
        virtual T Scaling(T height) const = 0;
};

#endif /*IPROFILE_H_*/
