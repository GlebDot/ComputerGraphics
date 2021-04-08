#version 330 core
out vec4 FragColor;

in float Height;
void main()
{
	FragColor = vec4(0.0f, Height, 0.5f, 1.0f);
}