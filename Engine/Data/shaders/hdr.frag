
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

const float exposure = 1.0f;

void main()
{ 
    vec3 color = texture(screenTexture, TexCoords).rgb;

    //exposure
    vec3 mapped = vec3(1.0f) - exp(-color * exposure);


    //gama corection
    mapped = pow(mapped, vec3(1.0f/2.2f));

    FragColor = vec4(mapped, 1.0f); 
}