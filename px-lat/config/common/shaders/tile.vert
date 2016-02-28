#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textcoord;

smooth out vec4 theColor;
smooth out vec2 theTexture;

uniform vec2 scale;

void main()
{
	gl_Position = vec4(position.xyz * scale.xyy, 1);
    theColor = color;
	theTexture = textcoord;
}