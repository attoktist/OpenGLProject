#pragma once
#ifndef LIGHT_H
#define LIGHT_H
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
#include "Camera.h"

class Light
{

public:
	
	int nVertices;	
	GLuint VAO;
	GLuint VBO;
	
	GLfloat vertices[216];		

	GLfloat x, y, z;
	GLfloat lx, ly, lz;

	glm::vec3 direction;

	glm::vec3 lightColor;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	Light()
	{

	}

	Light(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat lxx, GLfloat lyy, GLfloat lzz,
		glm::vec3 LC, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float cons, float lin, float quadr)
	{
		direction = glm::vec3(-x, -y, -z);
		//direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		lightColor = LC;
		ambient = amb;
		diffuse = dif;
		specular = spec;

		constant = cons;
		linear = lin;
		quadratic = quadr;

		x = xx;
		y = yy;
		z = zz;
		lx = lxx;
		ly = lyy;
		lz = lzz;		

		glGenBuffers(1, &VBO);		
		glGenVertexArrays(1, &VAO);
		nVertices = 216;

		GLfloat vert[] = {
			x - lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx, y - ly, z - lz,  0.0f, 0.0f, -1.0f,

			x - lx, y - ly,  z + lz,  0.0f, 0.0f, 1.0f,
			x + lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 0.0f,1.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,

			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly, z - lz,  -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,

			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,

			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,

			x - lx,  y + ly, z - lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly, z - lz, 0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly, z - lz,  0.0f, 1.0f, 0.0f,
		};
		//vertices = new GLfloat[n];
		for (int i = 0; i < nVertices; i++)
		{
			vertices[i] = vert[i];
		}
				

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}

	void SetLight(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat lxx, GLfloat lyy, GLfloat lzz,
		glm::vec3 LC, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float cons, float lin, float quadr)
	{
		direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		lightColor = LC;
		ambient = amb;
		diffuse = dif;
		specular = spec;

		constant = cons;
		linear = lin;
		quadratic = quadr;

		x = xx;
		y = yy;
		z = zz;
		lx = lxx;
		ly = lyy;
		lz = lzz;

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		nVertices = 216;

		GLfloat vert[] = {
			x - lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx, y - ly, z - lz,  0.0f, 0.0f, -1.0f,

			x - lx, y - ly,  z + lz,  0.0f, 0.0f, 1.0f,
			x + lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 0.0f,1.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,

			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly, z - lz,  -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,

			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,

			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,

			x - lx,  y + ly, z - lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly, z - lz, 0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly, z - lz,  0.0f, 1.0f, 0.0f,
		};
		//vertices = new GLfloat[n];
		for (int i = 0; i < nVertices; i++)
		{
			vertices[i] = vert[i];
		}


		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute6
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);

		glBindVertexArray(0); // Unbind VAO
	}

	void UpdatePosition()
	{
		GLfloat vert[] = {
			x - lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx, y - ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x + lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx,  y + ly, z - lz, 0.0f, 0.0f, -1.0f,
			x - lx, y - ly, z - lz,  0.0f, 0.0f, -1.0f,

			x - lx, y - ly,  z + lz,  0.0f, 0.0f, 1.0f,
			x + lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x + lx,  y + ly,  z + lz, 0.0f, 0.0f,1.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 0.0f,1.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,1.0f,

			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly, z - lz,  -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly, z - lz, -1.0f, 0.0f,0.0f,
			x - lx, y - ly,  z + lz,  -1.0f, 0.0f,0.0f,
			x - lx,  y + ly,  z + lz,  -1.0f, 0.0f,0.0f,

			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly, z - lz,  1.0f, 0.0f,0.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,0.0f,

			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x + lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly,  z + lz,  0.0f, -1.0f,0.0f,
			x - lx, y - ly, z - lz,  0.0f, -1.0f,0.0f,

			x - lx,  y + ly, z - lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly, z - lz, 0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x + lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 1.0f,0.0f,
			x - lx,  y + ly, z - lz,  0.0f, 1.0f, 0.0f,
		};
		//vertices = new GLfloat[n];
		for (int i = 0; i < nVertices; i++)
		{
			vertices[i] = vert[i];
		}


		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute6
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);

		glBindVertexArray(0); // Unbind VAO
	}


	void Draw(Shader shader)
	{

		shader.Use();		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	~Light()
	{

	}
};


std::string Value = "0123";

void set_light(Shader ourShader, Camera camera, Light light)
{

	ourShader.setVec3("viewPos", camera.Position);
	ourShader.setFloat("material.shininess", 32.0f);///было 32
	glm::vec3 lightPos(light.x, light.y, light.z);
	ourShader.setVec3("lightPos", lightPos);
	//Light 1
	ourShader.setVec3("light.position", lightPos);
	ourShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	ourShader.setFloat("light.cutOff", glm::cos(glm::radians(13.0f)));//было 13
	ourShader.setFloat("light.range", 5.0f);//было 2
	ourShader.setVec3("light.ambient", light.ambient*0.01f);
	ourShader.setVec3("light.diffuse", light.diffuse);//!!!!!!!!!!!
	ourShader.setVec3("light.specular", light.specular);//!!!!!!!!!!!
	ourShader.setFloat("light.constant", light.constant);
	ourShader.setFloat("light.linear", light.linear);
	ourShader.setFloat("light.quadratic", light.quadratic);
	//ourShader.setFloat("light[0].outerCutOff", glm::cos(glm::radians(25.0f)));
	//Light 2
	//ourShader.setVec3("light[1].position", glm::vec3(27.0f, 9.0f, 65.7f));
	//ourShader.setVec3("light[1].direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	//ourShader.setFloat("light[1].cutOff", glm::cos(glm::radians(5.0f)));
	//ourShader.setFloat("light[1].range", 5.0f);
	//ourShader.setVec3("light[1].ambient", 0.0f, 0.0f, 0.0f);
	//ourShader.setVec3("light[1].diffuse", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!
	//ourShader.setVec3("light[1].specular", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!
	//ourShader.setFloat("light[1].constant", 1.0f);
	//ourShader.setFloat("light[1].linear", 0.09f);
	//ourShader.setFloat("light[1].quadratic", 0.032f);
	////	ourShader.setFloat("light[1].outerCutOff", glm::cos(glm::radians(15.0f)));
	////Light 3
	//ourShader.setVec3("light[2].position", glm::vec3(39.5f, 9.0f, 65.0f));
	//ourShader.setVec3("light[2].direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	//ourShader.setFloat("light[2].cutOff", glm::cos(glm::radians(5.0f)));
	//ourShader.setFloat("light[2].range", 5.0f);//!!!!!!!!!!
	//ourShader.setVec3("light[2].ambient", 0.0f, 0.0f, 0.0f);
	//ourShader.setVec3("light[2].diffuse", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!
	//ourShader.setVec3("light[2].specular", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!!!!!!!
	//ourShader.setFloat("light[2].constant", 1.0f);
	//ourShader.setFloat("light[2].linear", 0.3f);//!!!!!!!!!!!!!!!!!!!!!
	//ourShader.setFloat("light[2].quadratic", 0.032f);
	//// ourShader.setFloat("light[2].outerCutOff", glm::cos(glm::radians(15.0f)));
	////Light 4
	//ourShader.setVec3("light[3].position", glm::vec3(39.0f, 9.0f, 79.0f));
	//ourShader.setVec3("light[3].direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	//ourShader.setFloat("light[3].cutOff", glm::cos(glm::radians(5.0f)));
	//ourShader.setFloat("light[3].range", 5.0f);//////////////
	//ourShader.setVec3("light[3].ambient", 0.0f, 0.0f, 0.0f);
	//ourShader.setVec3("light[3].diffuse", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!
	//ourShader.setVec3("light[3].specular", 1.0f, 0.5f, 0.0f);//!!!!!!!!!!!
	//ourShader.setFloat("light[3].constant", 1.0f);
	//ourShader.setFloat("light[3].linear", 0.09f);
	//ourShader.setFloat("light[3].quadratic", 0.032f);
	//ourShader.setFloat("light[3].outerCutOff", glm::cos(glm::radians(15.0f)));
}



#endif