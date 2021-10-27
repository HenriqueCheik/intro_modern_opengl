#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Tamanho da janela
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // criação da janela e verificação de erros
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Falha na criacao da janela" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD: Carregamento dos ponteiros de funções para utilizar OpenGL moderno
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falha na inicializacao do GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //-------------------------------------------------

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    // Triangulo
    float vertexData[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1- bind the Vertex Array Object
    // 2- bind and set vertex buffer(s)
    // 3- configure vertex attributes(s)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // (location, size, GL_datatype, normalize, stride, pointer_to_start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    //-------------------------------------------------

    // Loop da aplicação
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Render ----------------------------------------------------------------------
        // Define a cor de fundo da janela
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Limpa algum buffer específico
        glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor

        // Desenhar o triangulo
        // 1- ativar shader
        // 2- vincular VAO (pode ser feito uma vez fora do loop, caso exista apenas 1 VAO)
        // 3- usar glDrawArrays para desenhar conteudo do VAO
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // (primitiva, location, numero de vertices)


        // Faz a troca do framebuffer antigo para o novo (double buffer)
        glfwSwapBuffers(window);

        //------------------------------------------------------------------------------

        // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
        glfwPollEvents();
    }

    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}