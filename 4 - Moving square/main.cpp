#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "src/Quad.hpp"

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Tamanho da janela
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Player e inimigos globais para facilitar manipulação no input handling
// iniciando um player no meio da tela com a cor verde com tamanho 50, 50
Quad player(glm::vec2((float)WIDTH/2, (float)HEIGHT/2), glm::vec2(50.0f, 50.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

// iniciando um vetor de inimigos utilizando valores padrão para quad, mas posições diferentes
std::vector<Quad> enemies;

// vetor que guarda se algumas teclas estão pressionadas
// 0-W, 1-S, 2-A, 3-D
bool isKeyPressed[4] = {false, false, false, false};

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
    glfwSetKeyCallback(window, key_callback);

    // GLAD: Carregamento dos ponteiros de funções para utilizar OpenGL moderno
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falha na inicializacao do GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //-------------------------------------------------

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
    // Como só existira um shader nesse exemplo, ativar somente uma vez fora do loop
    shaderProgram.use();

    float vertexData[] = {
        // First triangle of quad
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        // Second triangle of quad
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // glBindVertexArray(0); 

    //-------------------------------------------------

    // matriz de view
    // geralmente exerce a função da camera do OpenGL
    // como a camera será fixa, iniciar como matriz identidade
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    shaderProgram.setMat4("uView", viewMatrix);

    // matriz de projeção
    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 100.0f);
    shaderProgram.setMat4("uProjection", projectionMatrix);

    // inicializando vetor de inimigos
    for (int i = 0; i < 10; i++)
    {
        // iniciar na fileira de cima
        if (i<5)
        {
            Quad aux(glm::vec2(250.0f + 60.0f * i, (float)HEIGHT - 100.0f));
            enemies.push_back(aux);
        }
        else
        {
            Quad aux(glm::vec2(250.0f + 60.0f * (i-5), (float)HEIGHT - 180.0f));
            enemies.push_back(aux);
        }
    }

    //-------------------------------------------------
    // Delta time para ficar independente de frame rate
    double currentFrame;
    double lastFrame = glfwGetTime();
    double deltaTime;

    // Loop da aplicação
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Calculo do Delta time
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update dos objetos do jogo
        player.update(deltaTime);
        for (int i = 0; i < 10; i++)
        {
            enemies[i].update(deltaTime);
        }

        // Render ----------------------------------------------------------------------
        // Define a cor de fundo da janela
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // Limpa algum buffer específico
        glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor

        // Desenhar objetos
        // Como eles compartilham mesma geometria, utilizar mesmo VBO
        // 1- ativar shader (pode ser feito uma vez fora do loop, caso exista apenas 1 shader)
        // 2- vincular VAO (pode ser feito uma vez fora do loop, caso exista apenas 1 VAO)
        // 3- usar glDrawArrays para desenhar conteudo do VAO
        // shaderProgram.use();
        // glBindVertexArray(VAO);

        // desenhando player
        glm::mat4 model = player.getModelMatrix();
        shaderProgram.setMat4("uModel", model);
        shaderProgram.setVec4("uColor", player.color);
        glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)

        // desenhando inimigos
        for (int i = 0; i < 10; i++)
        {
            model = enemies[i].getModelMatrix();
            shaderProgram.setMat4("uModel", model);
            shaderProgram.setVec4("uColor", enemies[i].color);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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

    // Checa se W ou S estão pressionadas
    // Somente quando nenhuma destas teclas está pressionada, faz o deslocamento vertical ser 0;
    if(isKeyPressed[0])
    {
        player.dy = 150.0;
    }
    else if(isKeyPressed[1])
    {
        player.dy = -150.0;
    }
    else if(!isKeyPressed[0] && !isKeyPressed[1])
    {
        player.dy = 0.0;
    }

    // Checa se A ou D estão pressionadas
    // Somente quando nenhuma destas teclas está pressionada, faz o deslocamento horizontal ser 0;
    if(isKeyPressed[2])
    {
        player.dx = -150.0;
    }
    else if(isKeyPressed[3])
    {
        player.dx = 150.0;
    }
    else if(!isKeyPressed[2] && !isKeyPressed[3])
    {
        player.dx = 0.0;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Tecla W pressionada
    if(key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        isKeyPressed[0] = true;
    }
    else if(key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        isKeyPressed[0] = false;
    }
    // Tecla S pressionada
    if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        isKeyPressed[1] = true;
    }
    else if(key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        isKeyPressed[1] = false;
    }
    

    // Tecla A pressionada
    if(key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        isKeyPressed[2] = true;
    }
    else if(key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        isKeyPressed[2] = false;
    }
    // Tecla D pressionada
    else if(key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        isKeyPressed[3] = true;
    }
    else if(key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        isKeyPressed[3] = false;
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}