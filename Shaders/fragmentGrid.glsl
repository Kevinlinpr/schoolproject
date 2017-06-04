#version 450

in vec3 fragmentColor;
in vec2 fragmentUV;

uniform sampler2D sprite;

out vec4 color;

void main()
{
    //color = vec4(0.4, 0.3, 0.5, 1.0);
	//color = vec4( fragmentColor, 1.0f )*texture( sprite, fragmentUV );
	vec4 t = texture( sprite, fragmentUV );
	if( t.a < 0.1 )
	    discard;
	color = t;
    //color = vec4(0.4, 0.3, 0.5, 1.0);
}
