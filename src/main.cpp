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

