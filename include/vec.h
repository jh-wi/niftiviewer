#pragma once

struct vec3 {
	float x;
	float y;
	float z;
	
	static vec3 up() {
		return vec3{0, 1, 0};
	}
};