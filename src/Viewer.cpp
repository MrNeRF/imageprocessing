#include "Viewer.h"
#include "Circle.h"
#include "Image.h"
#include "Line.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

double xpos, ypos;

Viewer::Viewer(void)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return;
    }
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version  = glGetString(GL_VERSION);  // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported \n"
              << version << std::endl;
    // build and compile our shader zprogram
    // ------------------------------------
}

Viewer::~Viewer(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void Viewer::Run(void)
{
    registerCallbacks();

    Shader imageShader("../Shaders/image.vs", "../Shaders/image.fs");
    Shader pointShader("../Shaders/points.vs", "../Shaders/points.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Image image1("../images/fibi.jpg");
    Eigen::Vector2f p1(0.0, 0.0), p2(0.4, 0.4);
    Line            line(p1, p2, 0.005);

    Circle circle(0.01f, 31);

    // -------------------------------------------------------------------------------------------
    imageShader.activate(); // don't forget to activate/use the shader before setting uniforms!
    imageShader.SetInt("texture1", 0);

    while (!glfwWindowShouldClose(m_window))
    {
        // input
        // -----
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        image1.Use();
        // render container
        imageShader.activate();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        pointShader.activate();
        circle.Draw();

        line.Draw();
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Viewer::registerCallbacks(void)
{
    // Create Callbacks
    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    };

    auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            std::cout << "Cursor Position at (" << xpos << " : " << ypos << ")\n";
        }
    };

    // Register Callback
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

