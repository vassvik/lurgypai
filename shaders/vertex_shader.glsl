#version 330 core

out vec2 uv;

uniform vec2 res;

uniform vec2 tex_size;
uniform vec2 sprite_size;

uniform int Nx;
uniform int N;

uniform float time;


void main() {
	// unit square
	vec2 p0 = vec2(gl_VertexID%2, gl_VertexID/2);
	
	// define vertex position to fill window
	vec2 p = 2.0*p0 - 1.0; 

	// correct for aspect ratio (by stretching x)
	p.x /= res.x/res.y;

	gl_Position = vec4(p, 0.0, 1.0);

	// get index of sprite, animate by looping through all sprites in 1 second
	int id = int(gl_InstanceID + N*time) % N;

	// convert index to local x and y index coordinates
	int i = id % Nx;
	int j = id / Nx;

	// expand to uv coordinates
	uv = vec2(i + p0.x, j + 1.0 - p0.y)*sprite_size/tex_size;
}