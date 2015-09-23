#version 330 core

in vec2 TexCoords;
in vec3 Normal;

out vec4 color;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    float     shininess;
};  

uniform Material material;

void main()
{    
	color = vec4(0.0f, 1.0f, 1.0f, 1.0f);
    //color = vec4(texture(texture_diffuse1, TexCoords));
}

// #version 330 core
// in vec2 TexCoord;

// out vec4 color;

// uniform sampler2D ourTexture1;
// uniform sampler2D ourTexture2;

// void main()
// {
//     color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
// }