#version 420 core

out vec4 vertexColorOut;

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;  
in vec3 vertexColor;
  
uniform vec3 lightPosition; 
uniform vec3 lightColor;

void main()
{
    // ambient
    /* float ambientStrength = 0.1; */
    /* vec3 ambient = ambientStrength * lightColor; */
  	
    /* // diffuse */ 
    /* vec3 norm = normalize(vertexNormal); */
    /* vec3 lightDir = normalize(lightPosition - vertexWorldPosition); */
    /* float diff = max(dot(norm, lightDir), 0.0); */
    /* vec3 diffuse = diff * lightColor; */
            
    /* vec3 result = (ambient + diffuse) * vertexColor; */
    /* vertexColorOut = vec4(result, 1.0); */
	vec4 col = vec4(0.7f, 0.9f, 0.8f, 1.0f);
    vertexColorOut = col; 
} 
