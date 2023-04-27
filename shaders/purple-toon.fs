#version 400

in float intensity;

out vec4 FragColor;
uniform vec4 colorStart;

void main()
{
	vec4 color;
	// colorStart = vec4(0, 0, 0, 0);

	// if (intensity > 0.95) {
	// 	color = vec4(colorStart.x + 0.7, colorStart.y + 0.5, colorStart.z + 1.0, colorStart.w + 1.0); // Lightest purple color
	// } else if (intensity > 0.5) {
	// 	color = vec4(colorStart.x + 0.4, colorStart.y + 0.3, colorStart.z + 0.6, colorStart.w + 1.0); // Second lightest purple color
	// } else {//if (intensity > 0.25) {
	// 	color = vec4(colorStart.x + 0.22, colorStart.y + 0.2, colorStart.z + 0.4, colorStart.w + 1.0); // Second darkest purple color
	// }
	
	if (intensity > 0.95) {
		color = vec4(0.7, 0.5, 1.0, 1.0); // Lightest purple color
	} else if (intensity > 0.5) {
		color = vec4(0.4, 0.3, 0.6, 1.0); // Second lightest purple color
	} else {//if (intensity > 0.25) {
		color = vec4(0.22, 0.2, 0.4, 1.0); // Second darkest purple color
	} //else {
		//color = vec4(0.1, 0, 0.2, 1.0); // Darkest purple color
	//}

	FragColor = color;
}