#version 330 core

out vec4 color;

uniform sampler2D texture_sampler;

in vec2 uv;

void main() {
	color = texture(texture_sampler, uv);
}