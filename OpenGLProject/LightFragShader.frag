#version 330 core
out vec4 FragColor;
  
in vec3 Normal;  
in vec3 FragPos;  

in vec2 TexCoord;
  
//uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 viewPos;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

struct Light {
    vec3 position;
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	 float constant;
    float linear;
    float quadratic;
};

uniform Light light;  

uniform sampler2D ourTexture;
uniform sampler2D normalMap;  

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;  

void main()
{
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);   
	normal = normalize(fs_in.TBN * normal);  

float distance    = length(light.position - FragPos);
float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));
   
// ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse 
    //vec3 norm = normalize(Normal);
	vec3 norm = normal*1.5f;
    vec3 lightDir = normalize(light.position - FragPos);
	//vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * material.specular;  

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * texture(ourTexture, fs_in.TexCoords).rgb;
    //color = vec4(result.xyz, 1.0f);
	FragColor = vec4(result, 1.0f);
	
} 