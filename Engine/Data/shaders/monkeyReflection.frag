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
uniform vec3 viewPos;

uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];

const float constant = 1.0f;
const float linear = 0.09f;
const float quadratic = 0.032f;

uniform samplerCube skybox;

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


//l - to light
//v to camera (view)
//N normal

void main()
{         
    //I = view direction      
    vec3 I = normalize(fs_in.FragPos - viewPos);
    vec4 reflection = texture(skybox, reflect(I, normalize(fs_in.Normal)));
    // vec3 N = normalize(fs_in.Normal);
    // vec3 L = normalize(lightPositions[0] - fs_in.FragPos.xyz);

    // vec3 eyeRay = normalize(fs_in.FragPos.xyz - viewPos);

    // vec3 reflection = texture(skybox, reflect(eyeRay, fs_in.Normal)).rgb;
    // vec3 refraction = texture(skybox, refract(eyeRay, fs_in.Normal, 1.1)).rgb;


    // vec3 diffuseColor = material.diffuse * max(0.0, dot(N, L));
          
    // // solid color is a mixture between the reflectance and the diffuse term
    // vec3 solidColor = mix(diffuseColor, reflection, material.shininess);    

    // // the final color is a mix between the solid and the transparent color
    // vec3 color = mix(refraction, solidColor, 0.1);


    FragColor = reflection;
}