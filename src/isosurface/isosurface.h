#pragma once

#include <mesh.h>

class IsosurfaceAlgorithm {
public:
	virtual SimpleMesh BuildSurfaceMesh(float *densities, int densitiesLength, int n, int _chunkSize, float _isolevel) = 0;
};