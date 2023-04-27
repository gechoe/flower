#version 400

in float intensity;

out vec4 FragColor;

void main()
{
	vec4 color;

	color = vec4(0.4, 0.7, 1, 1); // medium blue color

	FragColor = color;
}