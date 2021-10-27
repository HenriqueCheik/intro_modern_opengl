#version 330 core

// Location define a posição do atributo no VAO
layout (location = 0) in vec3 aPos; 

void main()
{
    // gl_Position é uma das variaveis presentes no vertex shader
    // definirá a posição final do vértice
    // Deve ser um vec4 (X, Y, Z, W)
    gl_Position = vec4(aPos, 1.0); 
}