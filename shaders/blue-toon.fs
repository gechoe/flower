#version 400

in float intensity;

out vec4 FragColor;

void main()
{
	vec4 color;

	// if (intensity > 0.95) {
	// 	color = vec4(0.7, 0.7, 1.0, 1.0); // Lightest blue color
	// } else if (intensity > 0.5) {
	// 	color = vec4(0.36, 0.36, 0.76, 1.0); // Second lightest blue color
	// } else if (intensity > 0.25) {
	// 	color = vec4(0.16, 0.16, 0.55, 1.0); // Second darkest blue color
	// } else {
	// 	color = vec4(0.07, 0.07, 0.4, 1.0); // Darkest blue color
	// }

	if (intensity > 0.5) {
		color = vec4(0.38, 0.38, 0.79, 1.0); // Second lightest blue color
	} else {
		color = vec4(0.25f, 0.25f, 0.58f, 1.0); // Second darkest blue color
	}

	FragColor = color;
}