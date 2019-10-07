#include "Viewer.h"
#include <Eigen/Dense>
#include <iostream>
#include <math.h>

void circle(float radius);
// settings

int main()
{
    Viewer view;
    view.Run();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void circle(float radius)
{
    Eigen::Matrix2Xf coords(2, 8);
    for (int i = 0; i < 8; ++i)
    {
        float fraction = i / 8;
        coords(0, i)   = radius * std::cos(M_PI * fraction);
        coords(1, i)   = radius * std::sin(M_PI * fraction);
    }
}
