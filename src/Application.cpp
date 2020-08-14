#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// openGL math library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// main application function
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
        
    // create window and context with core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(3);

    // experimental flag
    //glewExperimental == GL_TRUE;

    // checks if GLEW intializes correctly
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        // index buffer -- assigns each vertex to an index to prevent identical vertex information being stored to memory
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // enable blending
        GLCall(glEnable(GL_BLEND));
        // get source alpha, subtract from one to blend alpha at destination  
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // init vertex array class
        VertexArray va; 
        // init vertex buffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        // init vertex buffer layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        // add layout to vertex array object
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);
        
        // sets 4:3 aspect ratio in orthographic matrix
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        // DEBUG MODE: prints source code of shaders
        /*std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;*/

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/cancel_blank_back.png");
        texture.Bind();
        // bind texture to slot 0
        shader.SetUniform1i("u_Texture", 0);

        // unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform1i("u_Texture", 0);
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            // uniform modulation for color shift
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.05f)
                increment = 0.05f;

            r += increment;
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}