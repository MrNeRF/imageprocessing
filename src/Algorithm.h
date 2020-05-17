#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm
{
public:
    virtual ~Algorithm(void) = default;

    [[nodiscard]] virtual bool Compute(void) = 0;
};

#endif
