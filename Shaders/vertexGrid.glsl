#version 450

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexUV;

out vec3 fragmentColor;
out vec2 fragmentUV;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragmentColor = vertexColor;
	fragmentUV = vec2( vertexUV.x, 1.0 - vertexUV.y );

	gl_Position = projection * view * vec4( vertexPosition, 1.0 );
}
