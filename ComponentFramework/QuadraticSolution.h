#ifndef QUADRATICSOLUTION_H
#define QUADRATICSOLUTION_H
namespace GEOMETRY {
	enum class NumSolutions {
		zeroRoots = 0,
		oneRoot,
		twoRoots
	};
	struct QuadraticSolution
	{
		NumSolutions numSolutions{ NumSolutions::zeroRoots };
		// I'll use first root as the smallest of the two.
		// Set them both to zero to begin with
		float firstRoot = 0.0f;
		float secondRoot = 0.0f;

	};

	QuadraticSolution solveQuadratic(float a, float b, float c);

}
#endif

