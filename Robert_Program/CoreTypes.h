#pragma once
#include <tuple>
#include <string>
#include "glm/glm.hpp"

typedef std::tuple<bool, std::string> PROCEDURE_RETURN;
typedef std::tuple<bool, std::string> FAILABLE_PROCEDURE;

typedef float f32;
typedef double f64;

typedef glm::vec3 Point3D;

typedef struct {
	float x, y;
} Point2D;

typedef glm::vec4 RGBA_COL;

/*typedef struct {
	float r, g, b, a;
} RGBA_COL;*/