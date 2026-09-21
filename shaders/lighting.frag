#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 fragPos;
in vec3 normal;

void main() {
 float ambientStrength = 0.1;
 vec3 ambient = ambientStrength * lightColor;

 vec3 norm = normalize(normal);
 vec3 lightDir = normalize(lightPos - fragPos);
 float diff = max(dot(norm, lightDir), 0.0);
 vec3 diffuse = diff * lightColor;

 float specularStrength = 0.5;
 vec3 viewDir = normalize(viewPos - fragPos);
 vec3 reflectDir = reflect(-viewDir, norm);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 512); // Last value is shininess
 vec3 specular = specularStrength * spec * lightColor;

 vec3 phong = (ambient + diffuse + specular) * objectColor;
 FragColor = vec4(phong, 1.0);
};
