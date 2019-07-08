#version 330

uniform mat4 MVP;

in vec3 a_vertex;
in vec4 a_color;
in vec2 a_tex0;

out vec4 a_color_out;
out vec2 a_tex0_out;

void main(void){
	gl_Position = MVP * vec4(a_vertex, 1);
	a_color_out = a_color;
	a_tex0_out = a_tex0;
}