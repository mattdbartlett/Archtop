#ifndef PROFILE_H_
#define PROFILE_H_

#include <iprofile.h>

template <typename T>
class Profile : public IProfile<T> {
    public:
        Profile(T baseHeight, T totalHeight, uint32_t totalSteps) : m_baseHeight(baseHeight), m_totalHeight(totalHeight), m_totalSteps(totalSteps) {}
        virtual ~Profile() {}
        virtual T Smoothing(uint32_t stepNumber) const {
            return (m_totalSteps-stepNumber)/static_cast<T>(m_totalSteps);
        }

        virtual T Scaling(uint32_t stepNumber) const {
            return 1.0L-(m_totalHeight-m_baseHeight)*;(stepNumber/static_cast<T>(m_totalSteps))
        }

    private:
        Profile();
        uint32_t m_totalSteps;
        T m_baseHeight;
        T m_totalHeight;
};


#endif /*PROFILE_H_*/
