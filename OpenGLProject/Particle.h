#pragma once
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
#include <vector>

#ifndef PARTICLE_H
#define PARTICLE_H


struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;
	

	Particle() : Position(-10.0f,2.0f,10.0f), Velocity(0.0f,1.0f,0.0f), Color(1.0f), Life((float)(rand() % 6 + 1.0) / 10.f) { }
};
GLuint lastUsedParticle = 0;

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(GLuint texture, GLuint amount)
	{
		this->texture = texture;
		this->amount = amount;
		this->init();
	}
	// Update all particles
	void Update(GLfloat dt, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f))
	{
		GLfloat j = 0.0f;
		// Add new particles 
		for (GLuint i = 0; i < newParticles; ++i)
		{
			if (i % 5) j += 0.02f;
				int unusedParticle = this->firstUnusedParticle();
				this->respawnParticle(this->particles[unusedParticle], offset,j);
			
		}
		// Update all particles
		for (GLuint i = 0; i < this->amount; ++i)
		{
			Particle &p = this->particles[i];
			p.Life -= dt; // reduce life
			if (p.Life > 0.0f)
			{	// particle is alive, thus update
				//p.Position -= p.Velocity * dt;
				p.Position.y += dt;
				GLfloat random = (float)((rand() % 5) - 2.5f) / 100.0f;
				
				//p.Position.x += random;
				
				if (p.Position.x > offset.x) p.Position.x += random;
				else if (p.Position.x < offset.x) p.Position.x -= random;
				else if (p.Position.x == offset.x)
				{
					if(i%2==0)	p.Position.x -= random * 5;
					else p.Position.x += random * 5;

				}
				//p.Position.z += random;
				random = (float)((rand() % 5) - 2.5f) / 100.0f;
				if (p.Position.z > offset.z) p.Position.z += random;
				else if (p.Position.z < offset.z) p.Position.z -= random;
				else if (p.Position.z == offset.z)
				{
					if (i % 2 == 0)	p.Position.z -= random * 5;
					else p.Position.z += random * 5;

				}
				//p.Position.x += dt/10;
				//p.Position.z += dt / 10;
				/*p.Color.a -=  dt*1.5f;
				p.Color.r -= dt*1.5f;
				p.Color.g -= dt*1.5f;
				p.Color.b -= dt*1.5f;*/
				//p.Color -= dt*0.8f;
			}
		}
	}
	// Render all particles
	void Draw(Shader shader)
	{
		// Use additive blending to give it a 'glow' effect
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader.Program, "sprite"), 0);
		for (Particle particle : this->particles)
		{
			if (particle.Life > 0.0f)
			{
				shader.setVec3("offset", particle.Position);
				shader.setVec4("color", particle.Color);

				shader.setVec2("direction", glm::vec2(0.5f,0.5f));
				shader.setFloat("samples", 10.0f);
				shader.setFloat("bokeh", 0.5f);
				if(particle.Life <= 0.3) shader.setVec4("cl", glm::vec4(255.0f/255.0f, 69.0f/255.0f, 0.0f/255.0f, 1.0f));
				if ((particle.Life <= 0.7) && (particle.Life > 0.3f))  shader.setVec4("cl", glm::vec4(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f, 1.0f));
				if ((particle.Life <= 1.5f) && (particle.Life > 0.7f))  shader.setVec4("cl", glm::vec4(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f, 1.0f));
				
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				
			}
		}
		glBindVertexArray(0);
		// Don't forget to reset to default blending mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
private:
	// State
	std::vector<Particle> particles;
	GLuint amount;
	// Render state	
	GLuint texture;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init()
	{
		GLuint VBO;
		/*GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};*/
		GLfloat x = 0.0f, y = 0.0f, z = 0.0f, lx = 1.0f, ly = 1.0f, lz = 1.0f;

		GLfloat particle_quad[] = {
			x - lx, y - ly, z - lz,   0.0f, 0.0f, 
			x + lx, y - ly, z - lz,  1.0f, 0.0f,
			x + lx,  y + ly, z - lz,  1.0f, 1.0f,
			x + lx,  y + ly, z - lz,  1.0f, 1.0f,
			x - lx,  y + ly, z - lz,  0.0f, 1.0f,
			x - lx, y - ly, z - lz,  0.0f, 0.0f,

			x - lx, y - ly,  z + lz,  0.0f, 0.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 1.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 1.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 1.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,

			x - lx,  y + ly,  z + lz,  1.0f, 0.0f,
			x - lx,  y + ly, z - lz,  1.0f, 1.0f,
			x - lx, y - ly, z - lz,  0.0f, 1.0f,
			x - lx, y - ly, z - lz,  0.0f, 1.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,
			x - lx,  y + ly,  z + lz,  1.0f, 0.0f,

			x + lx,  y + ly,  z + lz,   1.0f, 0.0f,
			x + lx,  y + ly, z - lz,  1.0f, 1.0f,
			x + lx, y - ly, z - lz,  0.0f, 1.0f,
			x + lx, y - ly, z - lz,  0.0f, 1.0f,
			x + lx, y - ly,  z + lz,  0.0f, 0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,

			x - lx, y - ly, z - lz,  0.0f, 1.0f,
			x + lx, y - ly, z - lz,  1.0f, 1.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,
			x + lx, y - ly,  z + lz,  1.0f, 0.0f,
			x - lx, y - ly,  z + lz,  0.0f, 0.0f,
			x - lx, y - ly, z - lz,  0.0f, 1.0f,

			x - lx,  y + ly, z - lz,  0.0f, 1.0f,
			x + lx,  y + ly, z - lz,  1.0f, 1.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,
			x + lx,  y + ly,  z + lz,  1.0f, 0.0f,
			x - lx,  y + ly,  z + lz,  0.0f, 0.0f,
			x - lx,  y + ly, z - lz,  0.0f, 1.0f, 
		};
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(this->VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		// Create this->amount default particle instances
		for (GLuint i = 0; i < this->amount; ++i)
			this->particles.push_back(Particle());
	}
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle()
	{
		// First search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		lastUsedParticle = 0;
		return 0;
	}
	// Respawns particle
	void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f), GLfloat smesh=0.0f)
	{
		GLfloat random = (float)((rand() % 50) - 25) / 100.0f;
		//GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
		GLfloat rColor = 1.0f;
		particle.Position.x =  random + offset.x;
		particle.Position.y = 0.0f + offset.y;
		random = (float)((rand() % 50) - 25) / 100.0f;
		particle.Position.z = random + offset.z;
		particle.Color = glm::vec4(rColor, rColor, rColor, 0.5f);
		GLfloat rnd = (float)(rand() % 6 + 1.0)/10.f;
		particle.Life = 0.0f + rnd;
		particle.Velocity =  glm::vec3(0.0f,1.0f,0.0f);
	}
};



#endif