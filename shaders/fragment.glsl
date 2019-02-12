#version 400

in vec3 textureDir;

out vec3 color;

uniform samplerCube textureSampler;

void main(){
  color = texture(textureSampler, textureDir).rgb;
}