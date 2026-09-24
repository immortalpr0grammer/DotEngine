#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

struct Material {
 sampler2D diffuse;
 sampler2D specular;
 float shininess;
};

uniform Material material;

struct Light {
 vec3 position;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
 float maxRange;
};

uniform Light light;

void main() {
 float lightStrength = 1 - min(length(light.position - fragPos) / light.maxRange, 1.0);
 vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords)) * lightStrength;

 vec3 norm = normalize(normal);
 vec3 lightDir = normalize(light.position - fragPos);
 float diff = max(dot(norm, lightDir), 0.0);
 vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords)) * lightStrength;

 vec3 viewDir = normalize(viewPos - fragPos);
 vec3 reflectDir = reflect(-lightDir, norm);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // Last value is shininess
 vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords)) * lightStrength;

 vec3 phong = (ambient + diffuse + specular) * objectColor;
 FragColor = vec4(phong, 1.0);
};
