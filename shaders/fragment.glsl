#version 400

in vec3 textureDir;

out vec3 color;

flat in uint texturess;

uniform samplerCube grass;
uniform samplerCube stone;

void main(){
	if (texturess == 1)
  		color = texture(grass, textureDir).rgb;
  	else if (texturess == 2)
  		color = texture(stone, textureDir).rgb;
}