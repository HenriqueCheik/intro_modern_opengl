#version 330 core

// Output do vertex shader = input do fragment shader
// Variaveis devem ter mesmo nome para serem reconhecidas
in vec4 vertexColor;
  
out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
} 