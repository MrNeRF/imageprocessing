#version 420 core

out vec4 FragColor; 

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;  
in vec3 vertexColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
  
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(light.position - vertexWorldPosition);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
            
    vec3 viewDir = normalize(cameraPos - vertexWorldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular) * vertexColor; 
    FragColor = vec4(result, 1.f);
} 
