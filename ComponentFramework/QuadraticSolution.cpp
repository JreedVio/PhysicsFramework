#include "QuadraticSolution.h"
#include <algorithm>

GEOMETRY::QuadraticSolution GEOMETRY::solveQuadratic(float a, float b, float c)
{
    GEOMETRY::QuadraticSolution soln;
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
        return soln;
    }
    else if (fabs(discriminant) < FLT_MIN) {
        soln.numSolutions = NumSolutions::oneRoot;
        soln.firstRoot = (-b + sqrt(discriminant)) / (2.0f * a);
        return soln;
    }
    else {
        soln.numSolutions = NumSolutions::twoRoots;
        float x1 = (-b + sqrt(discriminant)) / (2.0f * a);
        float x2 = (-b - sqrt(discriminant)) / (2.0f * a);
        // Order them so that first root is the smallest
        soln.firstRoot = std::min(x1, x2);
        soln.secondRoot = std::max(x1, x2);
        return soln;
        
    }
}

void GEOMETRY::QuadraticSolution::print()
{
}
