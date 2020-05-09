#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm
{
public:
    Algorithm(void)          = default;
    virtual ~Algorithm(void) = default;

    [[nodiscard]] bool virtual Compute(void) = 0;
};

#endif
