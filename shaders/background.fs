#version 400

in float intensity;

out vec4 FragColor;

void main()
{
	vec4 color;

	color = vec4(0.79f, 0.79f, 0.99f, 1.0); // medium blue color

	FragColor = color;
}