#version 400 core

in vec3 ourColor;
in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform bool isColors;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main() { 
	// ambient
	vec3 ambient = material.ambient * lightColor;  
	
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * lightColor;  

	vec3 aeff = vec3(1.0);
	vec3 deff = vec3(0.5);
	vec3 seff = vec3(1.0);
	vec3 lightEffect = aeff * ambient + deff * diffuse + seff * specular;
	 
	// если объект нарисован только цветами
	if (isColors) {
		FragColor = vec4(ourColor * lightEffect, 1.0);
	}
	else {
		FragColor = vec4(texture(ourTexture, texCoord).rgb * lightEffect, 1.0);
	}
}