#version 330 core
out vec4 fragColor;

in vec3 ourColor;

uniform float timeValue;

vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

void main(){
	vec3 color = vec3(0.0f);
	float pct = abs(sin(timeValue));
	color = mix(colorA, colorB, pct);
	fragColor = vec4(color, 1.0f) + vec4(ourColor, 1.0f);
}