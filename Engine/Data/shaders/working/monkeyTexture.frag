#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Material 
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D shadowMap;

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

float ShadowCalculation(vec4 fragPosLightSpace)
{
    //perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //transform to range [0,1]
    projCoords = projCoords * 0.5f + 0.5f;
    //get closest depth value from light perspecive 
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(lightPositions[0] - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

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
    
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    return (ambient + (1.0f - shadow) * (diffuse + specular));
}

void main()
{           
    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

    vec3 lighting = vec3(0);

    //for(int i = 0; i < 4; i++)
        lighting += BlinnPhong(lightPositions[0], lightColors[0]);

    color *= lighting;
    //color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0f);

    //out
    //FragColor = vec4(ambient + diffuse + specular, 1.0f);
    //float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}