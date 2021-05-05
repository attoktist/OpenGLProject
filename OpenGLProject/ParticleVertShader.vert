#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 0.03f;;
    TexCoords = aTexCoords;
    ParticleColor = color;
    gl_Position = projection * view * vec4((aPos * scale) + offset, 1.0f);
}