#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

struct Material {
 sampler2D diffuse;
 sampler2D specular;
 float shininess;
};

uniform Material material;

struct DirectionalLight {
 vec3 direction;

 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

uniform DirectionalLight directionalLight;

vec3 calcDirLight(DirectionalLight directional, vec3 normal, vec3 viewDir);

struct PointLight {
 vec3 position;

 float constant;
 float linear;
 float quadratic;

 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

#define AMOUNT_POINT_LIGHTS 4
uniform PointLight pointLights[AMOUNT_POINT_LIGHTS];

vec3 calcPointLight(PointLight point, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
 vec3 position;
 vec3 direction;

 float innerCutOff;
 float outerCutOff;

 float constant;
 float linear;
 float quadratic;

 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

uniform bool ignoreSpotLight;
uniform SpotLight spotLight;

vec3 calcSpotLight(SpotLight spot, vec3 normal, vec3 fragPos, vec3 viewDir);


void main() {
 vec3 norm = normalize(normal);
 vec3 viewDir = normalize(viewPos - fragPos);

 vec3 result = calcDirLight(directionalLight, norm, viewDir);

 for (int i = 0; i < AMOUNT_POINT_LIGHTS; i++) {
  result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
 }

 if (ignoreSpotLight == false) {
  result += calcSpotLight(spotLight, norm, fragPos, viewDir);
 }

 FragColor = vec4(result, 1.0);
};


vec3 calcDirLight(DirectionalLight directional, vec3 normal, vec3 viewDir) {
 vec3 lightDir = normalize(-directional.direction);

 float diff = max(dot(normal, lightDir), 0.0);

 vec3 reflectDir = reflect(-lightDir, normal);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

 vec3 ambient = directional.ambient * texture(material.diffuse, texCoords).rgb;
 vec3 diffuse = directional.diffuse * diff * texture(material.diffuse, texCoords).rgb;
 vec3 specular = directional.specular * spec * texture(material.specular, texCoords).rgb;

 return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight point, vec3 normal, vec3 fragPos, vec3 viewDir) {
 vec3 lightDir = normalize(point.position - fragPos);

 float diff = max(dot(normal, lightDir), 0.0);

 vec3 reflectDir = reflect(-lightDir, normal);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

 float distance = length(point.position - fragPos);
 float attenuation = 1.0 / (point.constant + point.linear * distance + point.quadratic * (distance * distance));

 vec3 ambient = point.ambient * texture(material.diffuse, texCoords).rgb;
 vec3 diffuse = point.diffuse * diff * texture(material.diffuse, texCoords).rgb;
 vec3 specular = point.specular * spec * texture(material.specular, texCoords).rgb;
 ambient *= attenuation;
 diffuse *= attenuation;
 specular *= attenuation;

 return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight spot, vec3 normal, vec3 fragPos, vec3 viewDir) {
 vec3 lightDir = normalize(spot.position - fragPos);

 float diff = max(dot(normal, lightDir), 0.0);

 vec3 reflectDir = reflect(-lightDir, normal);
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

 float distance = length(spot.position - fragPos);
 float attentuation = 1.0 / (spot.constant + spot.linear * distance + spot.quadratic * (distance * distance));

 vec3 ambient = spot.ambient * texture(material.diffuse, texCoords).rgb;
 vec3 diffuse = spot.diffuse * diff * texture(material.diffuse, texCoords).rgb;
 vec3 specular = spot.specular * spec * texture(material.specular, texCoords).rgb;

 float theta = dot(lightDir, normalize(-spot.direction));
 float epsilon = spot.innerCutOff - spot.outerCutOff;
 float intensity = clamp((theta - spot.outerCutOff) / epsilon, 0.0, 1.0);

 diffuse *= intensity;
 specular *= intensity;

 ambient *= attentuation;
 diffuse *= attentuation;
 specular *= attentuation;

 return ambient + diffuse + specular;
}
