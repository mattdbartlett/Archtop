#ifndef PROFILE_H_
#define PROFILE_H_

#include <iprofile.h>

template <typename T>
class Profile : public IProfile<T> {
    public:
        Profile(T baseHeight, T totalHeight) : m_baseHeight(baseHeight), m_totalHeight(totalHeight) {}
        virtual ~Profile() {}
        virtual T Smoothing(T height) const {
            return 1.0L-(height-m_baseHeight)/(m_totalHeight-m_baseHeight);
        }

        virtual T Scaling(T height) const {
            return 1.0L-(height-m_baseHeight)/(m_totalHeight-m_baseHeight);
        }

    private:
        Profile();
        T m_baseHeight;
        T m_totalHeight;
};


#endif /*PROFILE_H_*/
