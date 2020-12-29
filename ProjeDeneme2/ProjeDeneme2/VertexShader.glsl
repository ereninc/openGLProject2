#version 460 core
layout (location = 0) in vec3 inPos;
uniform float uMoveX;
uniform float uMoveY;
void main()
{
	gl_Position = vec4(inPos+vec3(uMoveX, uMoveY, 0.0f), 5.0);
}