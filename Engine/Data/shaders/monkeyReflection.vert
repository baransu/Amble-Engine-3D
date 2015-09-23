#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0f)); 
    //for no normal maps (not tangent space) transpose inverse model nor normals
    vs_out.Normal = mat3(transpose(inverse(model))) * normal;
    vs_out.TexCoords = texCoords.st * vec2(-1.0, -1.0); 
    //vs_out.TexCoords = texCoords;
}

