#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform float timeValue;
uniform sampler2D texture1;

vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

void main(){
	vec3 color = vec3(0.0f);
	float pct = abs(sin(timeValue));
	color = mix(colorA, colorB, pct);
	fragColor = mix(texture(texture1, TexCoord), vec4(color, 1.0f), 0.2f);
}