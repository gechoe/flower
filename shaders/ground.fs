#version 400

in float intensity;

out vec4 FragColor;

void main()
{
	vec4 color;

	color = vec4(0.25, 0.25, 0.66, 1.0); // Second darkest blue color

	FragColor = color;
}