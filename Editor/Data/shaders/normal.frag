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

void main()
{           
    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // Get diffuse color
    vec3 color = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    // Ambient
    vec3 ambient = 0.2 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular

    float spec;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(0.5) * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}