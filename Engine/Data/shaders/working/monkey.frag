#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material 
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material; 

uniform vec3 lightPositions[3];
uniform vec3 lightColors[3];
uniform vec3 viewPos;

const float constant = 1.0f;
const float linear = 0.09f;
const float quadratic = 0.032f;

vec3 BlinnPhong(vec3 lightPos, vec3 lightColor)
{
    vec3 normal = normalize(fs_in.Normal);
    //Ambient
    vec3 ambient = 0.05 * lightColor;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = vec3(0.3f) * spec * material.specular;   
    // Simple attenuation
    float distance = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));   
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular + ambient;
}

void main()
{           
    vec3 color = material.diffuse;

    vec3 lighting = vec3(0);

    for(int i = 0; i < 4; i++)
        lighting += BlinnPhong(lightPositions[i], lightColors[i]);

    color *= lighting;
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0f);

}