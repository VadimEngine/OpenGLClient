#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

void main() {
	color = texture(ourTexture1, TexCoord);

	if (color.r == 1.0f &&color.g == 0.0f && color.b == 1.0f) {
		color.a = 0.0;
	}

}