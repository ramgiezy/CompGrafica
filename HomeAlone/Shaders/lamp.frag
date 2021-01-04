#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex, TexCoords); // Set all 4 vector values to 1.0f
	if (color.a<0.1)
		discard;
}