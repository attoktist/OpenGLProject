#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H
#include <glad\glad.h>
//#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <soil.h>
#include <iostream>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

#endif