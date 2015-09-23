#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
};

uniform Material material; 

//spec 0.3
//ambient 0.05
const float Pi = 3.14159265;
const float shininess = 16;

void main()
{           
    vec3 color = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    //Ambient
    vec3 ambient = 0.05f * color;
    //Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);    
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * color;
    //Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;

    //blinn
    float energyConversation = (8.0 + shininess) / (8.0 * Pi);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0f), shininess);
    vec3 specular = vec3(0.5f) * spec * energyConversation * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    //vec3 specular = vec3(texture(material.texture_specular1, fs_in.TexCoords)) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
    //float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}