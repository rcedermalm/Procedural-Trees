#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Classes
#include "ShaderProgram.hpp"
#include "Camera.h"
#include "MeshObject.h"
#include "Tree.h"

/*******************************************
 ****** FUNCTION/VARIABLE DECLARATIONS *****
 *******************************************/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;

// Camera variables
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Time variables
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

/*******************************************
 **************    MAIN     ****************
 *******************************************/

// The MAIN function, from here we start the application and run the rendering loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    if(!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return 1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural Trees", NULL, NULL);

    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    /************** Callback functions *************/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /***************** Shaders ********************/
    // Build and compile the shader program
    std::string vertexFilename = "../shaders/passThrough.vert";
    std::string geometryFilename = "../shaders/passThrough.gs";
    std::string fragmentFilename = "../shaders/passThrough.frag";
    ShaderProgram passThroughShader(vertexFilename, "", "", geometryFilename, fragmentFilename);
    passThroughShader();

    /****************** Models ********************/
    MeshObject branchCylinder;

    /** Matches with Tree 1-2 **/
    branchCylinder.readOBJ("../objects/mediumcylinder06.obj");
    float initialRadius = 0.6f;
    float initialLength = 1.0f;

    /** Matches with Tree 3 **/
    /*branchCylinder.readOBJ("../objects/cylinder_R06_L05.obj");
    float initialRadius = 0.6f;
    float initialLength = 0.5f;*/

    MeshObject branchSphere;
    branchSphere.createSphere(initialRadius * 1.1f, 4);

    /************** Uniform variables *************/
    GLint modelLoc = glGetUniformLocation(passThroughShader, "model");
    GLint viewLoc = glGetUniformLocation(passThroughShader, "view");
    GLint projLoc = glGetUniformLocation(passThroughShader, "projection");

    GLint objColourLoc = glGetUniformLocation(passThroughShader, "faceColour");
    GLint lightPosLoc = glGetUniformLocation(passThroughShader, "lightPos");

    /********** Build up tree structure ***********/

    /*** Tree 1 ***/
    std::string axiom = "X";
    std::vector<std::pair<char, std::string>> constructors = std::vector<std::pair<char, std::string>>();
    constructors.push_back( std::pair<char, std::string>('F', "FF"));
    constructors.push_back( std::pair<char, std::string>('X', "F-[[X]+X]+F[+FX]-X"));

    Tree* proceduralTree = new Tree(initialLength/2,  22.5f, 0.7f, objColourLoc);
    proceduralTree->createTreeFromLindenmayerSystem(axiom, constructors, 5);

    /*** Tree 2 ***/
    /*std::string axiom = "X";
    std::vector<std::pair<char, std::string>> constructors = std::vector<std::pair<char, std::string>>();
    constructors.push_back( std::pair<char, std::string>('X', "F[+X]F[-X]+X"));
    constructors.push_back( std::pair<char, std::string>('F', "FF"));

    Tree* proceduralTree = new Tree(initialLength/2,  20.f, 0.8f, objColourLoc);
    proceduralTree->createTreeFromLindenmayerSystem(axiom, constructors, 6);*/

    /*** Tree 3 ***/
    /*std::string axiom = "X";
    std::vector<std::pair<char, std::string>> constructors = std::vector<std::pair<char, std::string>>();
    constructors.push_back( std::pair<char, std::string>('X', "F[+X][-X]FX"));
    constructors.push_back( std::pair<char, std::string>('F', "FF"));

    Tree* proceduralTree = new Tree(initialLength/2,  25.7f, 0.8f, objColourLoc);
    proceduralTree->createTreeFromLindenmayerSystem(axiom, constructors, 7);*/

    /****************************************************/
    /******************* RENDER LOOP ********************/
    /****************************************************/
    float rotationAngle = 0.f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // OpenGL settings
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create camera, light and model transformations
        glm::vec3 lightPos = glm::vec3(40.f, 40.f, 0.f);
        Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));

        glm::mat4 rotatingModel;
        rotatingModel = glm::rotate(rotatingModel, glm::radians(currentFrame*10), glm::vec3(0.f, 1.f, 0.f));
        rotatingModel = glm::translate(rotatingModel, glm::vec3(0.f, 0.f, 40.f));

        camera.Position = glm::vec3(rotatingModel * glm::vec4(camera.Position, 1.0f));
        lightPos = glm::vec3(rotatingModel * glm::vec4(lightPos, 1.0f));

        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

        // Send uniforms to GPU
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

        /**************** RENDER STUFF ****************/
        proceduralTree->renderTree(branchCylinder, branchSphere, model, modelLoc);

        // Swap front and back buffers
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    delete proceduralTree;

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glfwGetWindowSize( window, &width, &height );
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
