#pragma once
#ifndef WALL_H
#define WALL_H
#include <glad\glad.h>

#include <gl\gl.h>   

#include <gl\glu.h>  
//#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <soil.h>
#include <iostream>

#include "Shader.h"
#include "Material.h"



class Wall
{

public:

	
	GLuint texture;
	GLuint normal_texture;
	int nVertices;
	int nIndices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLfloat vertices[504];
	

	GLfloat x, y, z;
	GLfloat lx, ly, lz;

	Material material;

	Wall()
	{

	}

	Wall(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat lxx, GLfloat lyy, GLfloat lzz, glm::vec3 ambient,
		glm::vec3 diffuse,glm::vec3 specular, float shininess, GLuint texture, GLuint normal_texture)
	{
		x = xx;
		y = yy;
		z = zz;
		lx = lxx;
		ly = lyy;
		lz = lzz;
		
		material.ambient = ambient;
		material.diffuse = diffuse;
		material.specular = specular;
		material.shininess = shininess;

		this->texture = texture;
		this->normal_texture = normal_texture;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);	
		nVertices = 504;

		// координаты вершин
		glm::vec3 pos1(x - lx, y + ly, z - lz);
		glm::vec3 pos2(x - lx, y - ly, z - lz);
		glm::vec3 pos3(x + lx, y - ly, z - lz);
		glm::vec3 pos4(x + lx, y + ly, z - lz);
		// текстурные координаты
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);
		// вектор нормали
		glm::vec3 nm(0.0, 0.0, -1.0);

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);



		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);

		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);

		GLfloat vert[] = {
			x - lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f, tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly, z - lz,0.0f, 0.0f, -1.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx,  y + ly,  z + lz,-1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx,  y + ly, z - lz,-1.0f, 0.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx, y - ly, z - lz,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx, y - ly, z - lz,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,-1.0f, 0.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,-1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x + lx,  y + ly,  z + lz, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,1.0f, 0.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x + lx, y - ly,  z + lz,1.0f, 0.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x + lx,  y + ly,  z + lz,1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  0.0f, 1.0f, tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z
		};
		//vertices = new GLfloat[n];
		for (int i = 0; i < nVertices; i++)
		{
			vertices[i] = vert[i];
		}
						
	//	image = SOIL_load_image(path_texture.c_str(), &widthT, &heightT, 0, SOIL_LOAD_RGB);
	//	glGenTextures(1, &texture);
	//	glBindTexture(GL_TEXTURE_2D, texture);

		////Для повторений если текстура закончилась
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//по x
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//по y

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthT, heightT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);	

		image = SOIL_load_image(path_normal_texture.c_str(), &widthT, &heightT, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &normal_texture);
		glBindTexture(GL_TEXTURE_2D, normal_texture);*/

		////Для повторений если текстура закончилась
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//по x
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//по y

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthT, heightT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);*/

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0); // Unbind VAO
	}

	void SetWall(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat lxx, GLfloat lyy, GLfloat lzz, glm::vec3 ambient,
		glm::vec3 diffuse, glm::vec3 specular, float shininess, GLuint texture, GLuint normal_texture)
	{
		this->texture = texture;
		this->normal_texture = normal_texture;

		x = xx;
		y = yy;
		z = zz;
		lx = lxx;
		ly = lyy;
		lz = lzz;

		material.ambient = ambient;
		material.diffuse = diffuse;
		material.specular = specular;
		material.shininess = shininess;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		nVertices = 504;

		// координаты вершин
		glm::vec3 pos1(x - lx, y + ly, z - lz);
		glm::vec3 pos2(x - lx, y - ly, z - lz);
		glm::vec3 pos3(x + lx, y - ly, z - lz);
		glm::vec3 pos4(x + lx, y + ly, z - lz);
		// текстурные координаты
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);
		// вектор нормали
		glm::vec3 nm(0.0, 0.0, -1.0);

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);



		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);

		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);

		GLfloat vert[] = {
			x - lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f, tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  1.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly, z - lz,0.0f, 0.0f, -1.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly, z - lz,0.0f, 0.0f, -1.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  1.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx,  y + ly,  z + lz,-1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx,  y + ly, z - lz,-1.0f, 0.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx, y - ly, z - lz,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x - lx, y - ly, z - lz,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,-1.0f, 0.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,-1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x + lx,  y + ly,  z + lz, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,1.0f, 0.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,1.0f, 0.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x + lx, y - ly,  z + lz,1.0f, 0.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x + lx,  y + ly,  z + lz,1.0f, 0.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly,  z + lz,0.0f, -1.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx, y - ly, z - lz,0.0f, -1.0f, 0.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,

			x - lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  1.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
			x + lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly,  z + lz,0.0f, 1.0f, 0.0f,  0.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
			x - lx,  y + ly, z - lz,0.0f, 1.0f, 0.0f,  0.0f, 1.0f, tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z
		};
		//vertices = new GLfloat[n];
		for (int i = 0; i < nVertices; i++)
		{
			vertices[i] = vert[i];
		}

		//	image = SOIL_load_image(path_texture.c_str(), &widthT, &heightT, 0, SOIL_LOAD_RGB);
		//	glGenTextures(1, &texture);
		//	glBindTexture(GL_TEXTURE_2D, texture);

		////Для повторений если текстура закончилась
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//по x
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//по y

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/*	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthT, heightT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		image = SOIL_load_image(path_normal_texture.c_str(), &widthT, &heightT, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &normal_texture);
		glBindTexture(GL_TEXTURE_2D, normal_texture);*/

		////Для повторений если текстура закончилась
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//по x
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//по y

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthT, heightT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);*/

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0); // Unbind VAO
	}


	void Draw(Shader shader)
	{
		
		shader.Use();
		shader.setVec3("material.ambient", material.ambient.x, material.ambient.y, material.ambient.z);
		shader.setVec3("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
		shader.setVec3("material.specular", material.specular.x, material.specular.y, material.specular.z);
		shader.setFloat("material.shininess", material.shininess);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glActiveTexture(GL_TEXTURE0); // Активируем текстурный блок перед привязкой текстуры
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_texture);
		glUniform1i(glGetUniformLocation(shader.Program, "normalMap"), 1);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, normal_texture);
	}

	~Wall()
	{

	}
};

#endif