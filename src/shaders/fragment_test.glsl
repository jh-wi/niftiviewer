#version 460 core

out vec4 FragColor;
//in vec3 albedo;
in vec2 TexCoord;
uniform vec4 albedo;
uniform sampler2D tex;

void main() {
	//FragColor = mix(texture(tex, TexCoord), albedo, 0.5);
	FragColor = albedo;
}