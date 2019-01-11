#version 330 core

in vec3 FragPosition;
in vec3 FragNormal;

uniform vec3 faceColour;
uniform vec3 lightPos;

out vec4 colour;

void main()
{
    vec3 lightColour = vec3(1.f, 1.f, 1.f);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColour;

    // diffuse
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

    vec3 result = (ambient + diffuse) * faceColour;
    colour = vec4(result, 1.0);
}