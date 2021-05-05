#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent; 


uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
uniform mat4 MVP;

out vec3 Normal;
out vec3 FragPos;  

out vec2 TexCoord;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;  



void main()
{

	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0f)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0f)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0f)));
	mat3 TBN = mat3(T, B, N);	
    //gl_Position = projection * view * model * vec4(position, 1.0f);
	gl_Position = MVP * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	//Normal = normal;
	Normal = mat3(transpose(inverse(model))) * normal;
	
	vs_out.TBN = mat3(T, B, N);
	vs_out.FragPos = FragPos;
	vs_out.TexCoords = TexCoord;
}