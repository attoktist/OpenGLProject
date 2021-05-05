#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
    float range;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in mat3 TBN;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D texture_normal;



vec3 progectlight(vec3 viewDir,vec3 norm,vec3 FragPos,Light lights)
{
    // ambient
    vec3 ambient = lights.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    
    vec3 lightDir = normalize(lights.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular

    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-lights.direction)); 
    float epsilon = (lights.cutOff - lights.outerCutOff);
    float intensity = clamp((theta - lights.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(lights.position - FragPos);
    float attenuation = lights.range / (lights.constant + lights.linear * distance + lights.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
    return ambient + diffuse + specular;
}



void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
   
    vec3 normal = texture(texture_normal, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(TBN * normal); 

    vec3 result = vec3(0.0);
  // for(int i = 0; i < 4; i ++)
     result = progectlight(viewDir,normal,FragPos,light);
    
       FragColor = vec4(result,1.0);
    //FragColor = vec4(result, 1.0);
} 