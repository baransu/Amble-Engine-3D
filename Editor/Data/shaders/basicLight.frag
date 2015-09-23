#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
};  

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform Material material;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{
    vec3 norm = normalize(Normal);


    // norm = texture(material.texture_normal1, TexCoords).rgb;
    // norm = normalize(norm * 2.0 - 1.0);   


    vec3 viewDir = normalize(viewPos - FragPos);

    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Phase 2: Point lights
    //result += CalcPointLight(pointLight, norm, FragPos, viewDir);    
    // Phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    //transparent
    // if(result.a < 0.1)
    //     discard;

    color = vec4(result, 1.0);
} 

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}