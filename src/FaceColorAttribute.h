#ifndef FACE_COLOR_ATTRIBUTE
#define FACE_COLOR_ATTRIBUTE

#include "FaceAttribute.h"
#include "Logger.h"
#include <vector>

class FaceColorAttribute : public FaceAttribute
{
public:
    Eigen::Vector3f& operator[](uint32_t i)
    {
        ASSERT(i >= 0 && i < m_faceColor.size());
        ASSERT(!m_faceColor.empty());
        return m_faceColor[i];
    }

    void SetFaceColor(const std::vector<Eigen::Vector3f>& faceColor)
    {
        m_faceColor.resize(faceColor.size());
        std::copy(std::begin(faceColor), std::end(faceColor), std::begin(m_faceColor));
    }

private:
    friend class OpenGL3DDataObject;
    std::vector<Eigen::Vector3f> m_faceColor;
};

#endif
