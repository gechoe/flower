#version 400

in float intensity;

out vec4 FragColor;

void main()
{
	vec4 color;

	color = vec4(0.44f, 0.44f, 0.71f, 1.0); // medium blue color

	FragColor = color;
}