#version 330 core

void main() {
	vec2 p = vec2(gl_VertexID%2, gl_VertexID/2); // [ 0.0, 1.0]
	p = 2.0*p - 1.0;                             // [-1.0, 1.0]
	p = 0.5*p;                                   // [-0.5, 0.5]

	gl_Position = vec4(p, 0.0, 1.0);
}