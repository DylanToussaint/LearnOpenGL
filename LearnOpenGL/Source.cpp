#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit(); //initalize GLFW
    //Window handling, choose an option and set its value using glfwWindowHint();
    //list of options: https://www.glfw.org/docs/latest/window.html#window_hints
    //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //set version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //set subversion to 3 therefore 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //tell openGl we want to use exclusively the core profile
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Only for MacOS

    //Create Window object
    //
    //
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //initialise window, set width height name
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //set the window as the main context of the current thread
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //tell glfw to run the resize function everytime the window is resized

    //Initialise GLAD
    //
    //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Start Rendering
    //
    //There are two buffers for output, one that is shown on screen and one that is still being rendered to, to prevent artifacts we swap buffer
   
    //Build and compile our Shader Program
    // 
    // 
    Shader ourShader("shader.vs", "shader.fs");

    //Set up vertex data and buffers
    //
    //

    float vertices[]{
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[]{
        0, 1, 3, //first triangle
        1, 2, 3 //second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //Generates VAO
    glGenBuffers(1, &VBO); //Generates VBO
    glGenBuffers(1, &EBO); //Generates EBO
    //Bind VAO first, bind and set VBOs, configure attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //assign data to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //same as above
    //GL_STREAM_DRAW -> data set once and used by GPU at most a few times
    //GL_STATIC_DRAW -> data set once and used by GPU many times
    //GL_DYNAMIC_DRAW -> data set many times and used by GPU many times

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //Telling OpenGL how to interpret buffer data
    glEnableVertexAttribArray(0);
    //0 specifies the vertex attribute we want to configure
    //Size of the vertex attribute -> vec3 so size = 3
    //Specifies type of data
    //Specifies if we want data to be normalized -> already is so no
    //Stride->> how much space between consecutive vertex attributes therefore 3 * 4 bytes = 12
    //How much offset -> 0
    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo
    glBindVertexArray(0); //unbind vao

    //Load and create a texture
    unsigned int texture1, texture2;

    //texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Load image create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//flips it on y-axis
    
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);//manually set texture as a uniform
    //Uncoment for wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //Render loop
    //
    while (!glfwWindowShouldClose(window)) {
        //Input processing
        processInput(window);

        //Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        //Create transformations
        //glm::mat4 trans = glm::mat4(1.0f);
        //Implement quaternions

        glm::vec3 rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        float rotationAngle = (float)glfwGetTime();
        float w, x, y, z;
        x = rotationAxis.x * sin(rotationAngle / 2);
        y = rotationAxis.y * sin(rotationAngle / 2);
        z = rotationAxis.z * sin(rotationAngle / 2);
        w = cos(rotationAngle / 2);

        glm::quat quaternion;
        quaternion = glm::quat(w, x, y, z);

        //trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        glm::mat4 rotationMatrix = glm::toMat4(quaternion);
        //draw the triangles
        ourShader.use();
        //Pass on our transformaiton matrix as a uniform
        unsigned int transFormLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transFormLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix)); //GL_FALSE -> do we want to transpose our matrix GLM::value_ptr -> converts it to OpenGl type matrix
        //Do some cool stuff with the system time
        float timeValue = glfwGetTime();
        //input time Value
        ourShader.setFloat("timeValue", timeValue);
        //bind VAO and draw triangles
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Check and call events and swap buffers
        glfwSwapBuffers(window); //Swaps the color buffer and show it as output to the screen
        glfwPollEvents(); //Detects if any events occur, like keyboard input or mouse movement
    }

    //De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();//terminates glfw
    return 0;
}

//This function will resize the viewport everytime the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//This function will process inputs
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//If user has pressed the escape key, close the window
        glfwSetWindowShouldClose(window, true);
}