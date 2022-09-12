#pragma once

#include "isosurface.h"
#include <glm/glm.hpp>
#include "mctables.h"
#include <iostream>
#include <cstdint>
#include "mctables.h"
#include <vector>
#include <thread>

class MarchingCubes : public IsosurfaceAlgorithm {
public:
	float isolevel;

	SimpleMesh BuildSurfaceMesh(float* densities, int densitiesLength, int n, int chunkSize, float _isolevel) {
		
		isolevel = _isolevel;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		int q = 0;
		
		for (int i = 0; i < n; i++) {
			glm::ivec3 pos = GetVoxelPos(i, chunkSize - 1);
			//std::cout << "voxelpos (" << i << "): " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			
			glm::vec3 corners[] = {
				pos,
				pos + glm::ivec3(1, 0, 0),
				pos + glm::ivec3(1, 0, 1),
				pos + glm::ivec3(0, 0, 1),
				pos + glm::ivec3(0, 1, 0),
				pos + glm::ivec3(1, 1, 0),
				pos + glm::ivec3(1, 1, 1),
				pos + glm::ivec3(0, 1, 1)
			};
			
			float cornerDensities[] = {
				GetDensity(pos, densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(1, 0, 0), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(1, 0, 1), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(0, 0, 1), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(0, 1, 0), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(1, 1, 0), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(1, 1, 1), densities, densitiesLength, n, chunkSize),
				GetDensity(pos + glm::ivec3(0, 1, 1), densities, densitiesLength, n, chunkSize)
			};
			
			int cubeIndex = CubeIndex(cornerDensities);
			//std::cout << "cubeIndex (" << i << "): " << cubeIndex << std::endl;
			int row = 15 * cubeIndex;
			//std::cout << "row (" << i << "): " << row << ", val: " << TriangleTable[row] << std::endl;
			
			
			
			for (int t = 0; TriangleTable[row + t] != -1; t += 3) {
				// std::cout << "TriangleTable row: " << row << '+' << t << std::endl;
				int a0 = cornerIndexAFromEdge[TriangleTable[row + t]];
				int b0 = cornerIndexBFromEdge[TriangleTable[row + t]];
				
				int a1 = cornerIndexAFromEdge[TriangleTable[row + t + 1]];
				int b1 = cornerIndexBFromEdge[TriangleTable[row + t + 1]];
				
				int a2 = cornerIndexAFromEdge[TriangleTable[row + t + 2]];
				int b2 = cornerIndexBFromEdge[TriangleTable[row + t + 2]];
				
				vertices.push_back(((corners[a0] + corners[b0])*0.5f).x);
				vertices.push_back(((corners[a0] + corners[b0])*0.5f).y);
				vertices.push_back(((corners[a0] + corners[b0])*0.5f).z);
				vertices.push_back(((corners[a1] + corners[b1])*0.5f).x);
				vertices.push_back(((corners[a1] + corners[b1])*0.5f).y);
				vertices.push_back(((corners[a1] + corners[b1])*0.5f).z);
				vertices.push_back(((corners[a2] + corners[b2])*0.5f).x);
				vertices.push_back(((corners[a2] + corners[b2])*0.5f).y);
				vertices.push_back(((corners[a2] + corners[b2])*0.5f).z);
				indices.push_back(q);
				indices.push_back(q+1);
				indices.push_back(q+2);
				indices.push_back(q+3);
				indices.push_back(q+4);
				indices.push_back(q+5);
				indices.push_back(q+6);
				indices.push_back(q+7);
				indices.push_back(q+8);
				q+=9;
			}
		}
		//std::cout << "verts: " << vertices.size() / 3 << std::endl;
		return SimpleMesh(vertices, indices);
		
	}
	
	glm::ivec3 GetVoxelPos(int index, int _size) {
		glm::ivec3 pos;
		pos.z = index / (_size * _size);
		index -= pos.z * _size * _size;
		pos.y = index / _size;
		pos.x = index % _size;
		return pos;
	}
	
	static int GetVoxelIndex(glm::ivec3 pos, int chunkSize) {
		return (pos.z * chunkSize * chunkSize) + (pos.y * chunkSize) + pos.x;
	}
	
	glm::vec3 InterpolateVertex(glm::vec4 v1, glm::vec4 v2) {
		float t = (isolevel - v1.w) / (v2.w - v1.w);
		return v1 + t * (v2 - v1);
	}
	
	int CubeIndex(float* corners) {
		int index = 0;
		index |= (int)(corners[0] < isolevel ? 1 : 0);
		index |= (int)(corners[1] < isolevel ? 2 : 0);
		index |= (int)(corners[2] < isolevel ? 4 : 0);
		index |= (int)(corners[3] < isolevel ? 8 : 0);
		index |= (int)(corners[4] < isolevel ? 16 : 0);
		index |= (int)(corners[5] < isolevel ? 32 : 0);
		index |= (int)(corners[6] < isolevel ? 64 : 0);
		index |= (int)(corners[7] < isolevel ? 128 : 0);
		return index;
	}
	
	float GetDensity(glm::ivec3 pos, float* densities, int densitiesLength, int n, int chunkSize) {
		int index = GetVoxelIndex(pos, chunkSize);
		if (index >= 0 && index < densitiesLength) { //TODO: change 27 to the length of the densities array
			return densities[index];
		}
		//std::cout << "GetDensity index: " << index << std::endl;
		return 1;
	}
};