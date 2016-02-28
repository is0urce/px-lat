#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

smooth out vec4 theColor;

uniform vec2 scale;
uniform vec2 offset;

void main()
{
	gl_Position = vec4((position + offset) * scale - vec2(1, 1), 0, 1);
    theColor = color;
}