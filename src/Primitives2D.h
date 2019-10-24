#ifndef PRIMITIVES2D_H
#define PRIMITIVES2D_H

#include "Eigen/Dense"

class Primitives2D
{
	public:
            Primitives2D(void)          = default;
            virtual ~Primitives2D(void) = default;

            virtual void            Draw(void) const                                     = 0;
            virtual Eigen::Vector3f GetColor(void) const                                 = 0;
            virtual void            SetColor(const Eigen::Vector3f& color)               = 0;
            virtual bool            CheckCollision(const Eigen::Vector2f& pointToTest) const = 0;
};

#endif
