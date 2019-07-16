#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aText;

const vec3 TOP = vec3(0, 1, 0);
const vec3 BOTTOM =  vec3(0, -1, 0);
const vec3 LEFT = vec3(-1, 0, 0);
const vec3 RIGHT = vec3(1, 0, 0);
const vec3 FRONT = vec3(0, 0, 1);
const vec3 BACK = vec3(0, 0, -1);

out vec2 Text;
out vec3 Norm;
out vec3 FragPos;

out vec2 tileUV;
out vec2 tileOffset;
out float visibility;

uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;
uniform int		blockType;
uniform float	fogDensity;

/*********** Fog ***********/
const float gradient = 15.0f;
/***************************/


float		deg_to_rad(float deg)
{
	return (deg * (3.14159265359f / 180.0f));
}

vec2 rotateUV(vec2 uv, float rotation, float mid)
{
	return vec2(
	cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
	cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid);
}

void main()
{
	Norm = aNorm;

	/************************ FOG *************************/
	vec4 worldpos = model * vec4(aPos, 1.0f);
	vec4 PRTC = view * worldpos;
	float distance = length(PRTC.xyz);
	visibility = exp(-pow((distance * fogDensity), gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
	/*******************************************************/

	/******** Texture UV atlas ***********/
	tileOffset = aText; // Se referer aux nombres de la fonction pour choisir la texture
	if (Norm == LEFT)
		tileUV = rotateUV(vec2(dot(Norm.zxy, aPos), dot(Norm.yzx, aPos)), deg_to_rad(-90.0f), 0.5f);
	else if (Norm == RIGHT)
		tileUV = rotateUV(vec2(dot(Norm.zxy, aPos), dot(Norm.yzx, aPos)), deg_to_rad(90.0f), 0.5f);
	else if (Norm == FRONT)
		tileUV = rotateUV(vec2(dot(Norm.zxy, aPos), dot(Norm.yzx, aPos)), deg_to_rad(180.0f), 0.5f);
	else
		tileUV = vec2(dot(Norm.zxy, aPos), dot(Norm.yzx, aPos));
	/*************************************/

	FragPos = vec3(vec4(aPos, 1.0f) * model);
	gl_Position = projection * PRTC;
}
