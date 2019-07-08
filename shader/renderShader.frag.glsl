#version 330

out vec4 fragColor;

in vec4 a_color_out;
in vec2 a_tex0_out;

uniform sampler2D tex0;

uniform int isTextured = 1;

void main(void){
	vec4 textureData = texture(tex0, a_tex0_out);
	if(isTextured == 0){
		fragColor = a_color_out/255.0;
	}else{
		fragColor = a_color_out/255.0*textureData;
	}
}