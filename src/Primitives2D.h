#ifndef PRIMITIVES2D_H
#define PRIMITIVES2D_H

class Primitives2D
{
	public:
            Primitives2D(void)          = default;
            virtual ~Primitives2D(void) = default;

            virtual void Draw(void) const = 0;
};
#endif
