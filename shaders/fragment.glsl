#version 330 core

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

//uniform sampler2D ourTexture;
uniform vec3 cameraPos;
uniform samplerCube skybox;

float air = 1.;
float water = 1.33;
float ice = 1.309;
float glass = 1.52;
float diamond = 2.42;

float calcRatio(float from, float to) {
    return from / to;
}

void main() {
    //FragColor = texture(ourTexture, TexCoord);
    vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    vec3 R = refract(I, normalize(Normal), calcRatio(air, glass));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}