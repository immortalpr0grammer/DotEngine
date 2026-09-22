#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 fragPos;
in vec3 normal;

struct Material {
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
 float shininess;
};

uniform Material material;

struct Light {
 vec3 position;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

uniform Light light;

void main() {
 vec3 ambient = vec3(0.1) * material.ambient;

 vec3 norm = normalize(normal);
 vec3 lightDir = normalize(light.position - fragPos);
 float diff = max(dot(norm, lightDir), 0.0);
 vec3 diffuse = lightColor * (diff * material.diffuse);

 vec3 viewDir = normalize(viewPos - fragPos);
 vec3 reflectDir = reflect(-viewDir, norm);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 512); // Last value is shininess
 vec3 specular = lightColor * (spec * material.specular);

 vec3 phong = (ambient + diffuse + specular) * objectColor;
 FragColor = vec4(phong, 1.0);
};
