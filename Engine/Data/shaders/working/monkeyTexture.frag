#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material 
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material; 
uniform vec3 viewPos;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

//spec 0.3
//ambient 0.05
//const float Pi = 3.14159265;

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
    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

    vec3 lighting = vec3(0);

    for(int i = 0; i < 5; i++)
        lighting += BlinnPhong(lightPositions[i], lightColors[i]);

    color *= lighting;
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0f);

    //out
    //FragColor = vec4(ambient + diffuse + specular, 1.0f);
    //float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}