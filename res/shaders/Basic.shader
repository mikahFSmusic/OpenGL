#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in vec4 a_Color;
 
out vec2 v_TexCoord;

uniform mat4 u_MVP;

out float v_TexIndex;
out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_TexIndex = a_TexIndex;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Textures[index], v_TexCoord) * u_Color;
	color = texColor;
};