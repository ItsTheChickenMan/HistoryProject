#version 330 core

in vec2 f_tex_coord;
in vec3 f_normals;

in vec3 frag_pos;

out vec4 FragColor;

uniform sampler2D tex;


uniform vec3 camera_pos;

struct Material {
    vec3 specular;
    float shine;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight {
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
};

uniform Material material;

//uniform int numPointLights;
//uniform int numSpotlights;

#define num_lights 11
uniform Light lights[num_lights];
//uniform PointLight pointLights[20];
//uniform Spotlight spotlights[20];

//vec3 calculateDirectional(DirectionalLight light, vec3 normal, vec3 viewDir);
//vec3 calculatePoint(PointLight light, vec3 normal, vec3 viewDir);
//vec3 calculateSpot(Spotlight light, vec3 normal, vec3 viewDir);
vec3 calculateLighting(Light light, vec3 normal);

void main()
{
	vec3 normal = normalize(f_normals);
	vec3 result = vec3(0, 0, 0);
	//vec3 minimumAmbience = vec3(texture(tex, f_tex_coord)) * vec3(0.05, 0.05, 0.05);
	
	for(int i = 0; i < num_lights; i++){
		result += calculateLighting(lights[i], normal);
	}
	
	/*for(int i = 0; i < numSpotlights; i++){
		result += calculateSpot(spotLights[i], normal, viewDir);
	}*/
	
	FragColor = vec4(result, 1);
	/*// Ambient
	vec3 ambient = vec3(texture(tex, f_tex_coord)) * light.ambient;

	// Diffuse

	vec3 normal = normalize(f_normals);

	vec3 light_dir = normalize(light.position - frag_pos);

	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(tex, f_tex_coord));

	// Specular
	
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);

	
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shine);
	
	vec3 specular = light.specular * (spec * material.specular);


	// Final Calculations
	

	vec4 result = vec4(ambient + diffuse, 1.0);

	FragColor = result;*/
}

vec3 calculateLighting(Light light, vec3 normal){
	// Ambient
	vec3 ambient = vec3(texture(tex, f_tex_coord)) * light.ambient;

	// Diffuse
	vec3 light_dir = normalize(light.position - frag_pos);

	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(tex, f_tex_coord));
	
	float distance = length(light.position - frag_pos);
	float linear = 0.072;
	float quadratic = 0.0175;
	float attenuation = 1.0 / (1 + linear * distance + quadratic * (distance * distance));    
						
	// Specular
	
	//vec3 view_dir = normalize(camera_pos - frag_pos);
	//vec3 reflect_dir = reflect(-light_dir, normal);

	/*
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shine);
	
	vec3 specular = light.specular * (spec * material.specular);
*/
	
	// Final Calculations
	return vec3(ambient*attenuation + diffuse*attenuation);
}

/*vec3 calculateDirectional(DirectionalLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	// combine results
	vec3 ambient  = light.ambient  * vec3(texture(tex, f_tex_coord));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(tex, f_tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(tex, f_tex_coord));
	return (ambient + diffuse + specular);
}

vec3 calculatePoint(PointLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(light.position - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    // attenuation
    float distance    = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(tex, f_tex_coord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(tex, f_tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(tex, f_tex_coord));
		
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculateSpot(Spotlight light, vec3 normal, vec3 viewDir){
	// ambient
	vec3 ambient = light.ambient * texture(tex, f_tex_coord).rgb;
	
	// diffuse 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(tex, f_tex_coord).rgb;  
	
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	vec3 specular = light.specular * spec * material.specular;  
	
	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;
	
	// attenuation
	float distance    = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   
			
	return (ambient + diffuse + specular);
}*/