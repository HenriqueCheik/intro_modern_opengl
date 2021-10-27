#version 330 core
out vec4 FragColor;
  
// uniforms são variáveis alteradas no código OpenGL
uniform vec4 uColor; 

void main()
{
    FragColor = uColor; //RGBA
} 