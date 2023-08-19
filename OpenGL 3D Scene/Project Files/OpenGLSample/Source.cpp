#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.h> // Camera class

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Hunter Richards Final Project"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    //const int WINDOW_WIDTH = 1920;
    //const int WINDOW_HEIGHT = 1080;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handle for the vertex buffer object
        GLuint ebo;         // Handle the element buffer object
        GLuint nVertices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;


    glm::vec2 gUVScale(100.0f, 100.0f);
    GLint gTexWrapMode = GL_MIRRORED_REPEAT;

    // Cube mesh data
    GLMesh gHouseStructureMesh;
    // Pyramid mesh data 
    GLMesh gRoofMesh;
    // Floor mesh data
    GLMesh gFloorMesh;
    // Sphere mesh data
    GLMesh gSphereMesh;
    // Window mesh data
    GLMesh gWindowMesh;
    // Bush mesh data
    GLMesh gBushMesh;

    // Cube texture id
    GLuint gHouseStructureTextureId;
    // Pyramid texture id
    GLuint gRoofTextureId;
    // Floor texture id
    GLuint gFloorTextureId;
    // Sphere texture id
    GLuint gSphereTextureId;
    // Window texture id
    GLuint gWindowTextureId;
    // Window texture id
    GLuint gBushTextureId;


    // Shader program
    GLuint gProgramId;
    GLuint gLampProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;
    float cameraSpeed = 100.5f;
    bool isPerspectiveView = true;

    // Subject position and scale
    glm::vec3 gCubePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 gCubeScale(2.0f);

    // Cube and light color
    //m::vec3 gObjectColor(0.6f, 0.5f, 0.75f);
    glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);

    // Sun information/settings
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 gLightPosition(1.0f, 100.0f, 1.0f);
    glm::vec3 gLightScale(3.0f);

    // Moon information/settings
    glm::vec3 gSecondLightColor(0.5f, 0.5f, 0.8f);
    glm::vec3 gSecondLightPosition(-1.0f, -100.0f, -1.0f);
    glm::vec3 gSecondLightScale(3.0f);

    // Lamp animation
    bool gIsLampOrbiting = false;

}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateHouseStructureMesh(GLMesh& mesh);
void UCreateRoofMesh(GLMesh& mesh);
void UCreateFloorMesh(GLMesh& mesh);
void UCreateSphereMesh(GLMesh& mesh);
void UCreateBushMesh(GLMesh& mesh);
void UCreateWindowMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
void URenderScene();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/* Cube Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    layout(location = 2) in vec2 textureCoordinate;

    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
    out vec2 vertexTextureCoordinate;

    //Uniform / Global variables for the  transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

        vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

        vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
        vertexTextureCoordinate = textureCoordinate;
    }
);


/* Cube Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,

    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor; // For outgoing cube color to the GPU

    // Uniform / Global variables for object color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 secondLightColor;
    uniform vec3 secondLightPos;
    uniform float secondLightStrength = 2.0; // New variable for the second light's strength
    uniform vec3 viewPosition;
    uniform sampler2D uTexture; // Useful when working with multiple textures
    uniform vec2 uvScale;

    void main()
    {
        /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

        // Calculate Ambient lighting for primary light
        float ambientStrength = 0.01f;
        vec3 ambient = ambientStrength * lightColor;

        // Calculate Diffuse lighting for primary light
        vec3 norm = normalize(vertexNormal);
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
        float impact = max(dot(norm, lightDirection), 0.0);
        vec3 diffuse = impact * lightColor;

        // Calculate Specular lighting for primary light
        float specularIntensity = 0.8f;
        float highlightSize = 16.0f;
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos);
        vec3 reflectDir = reflect(-lightDirection, norm);
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // Calculate Ambient lighting for second light
        vec3 secondAmbient = ambientStrength * secondLightColor * secondLightStrength; // Adjusted by secondLightStrength

        // Calculate Diffuse lighting for second light
        vec3 secondLightDirection = normalize(secondLightPos - vertexFragmentPos);
        float secondImpact = max(dot(norm, secondLightDirection), 0.0);
        vec3 secondDiffuse = secondImpact * secondLightColor * secondLightStrength; // Adjusted by secondLightStrength

        // Calculate Specular lighting for second light
        vec3 secondReflectDir = reflect(-secondLightDirection, norm);
        float secondSpecularComponent = pow(max(dot(viewDir, secondReflectDir), 0.0), highlightSize);
        vec3 secondSpecular = specularIntensity * secondSpecularComponent * secondLightColor * secondLightStrength; // Adjusted by secondLightStrength

        // Texture holds the color to be used for all three components
        vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

        // Calculate phong result by combining both lights
        vec3 phong = (ambient + diffuse + specular + secondAmbient + secondDiffuse + secondSpecular) * textureColor.xyz;

        fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
    }
);


/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

        //Uniform / Global variables for the  transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
    }
);

/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

    void main()
    {
        fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
    }
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateHouseStructureMesh(gHouseStructureMesh);
    UCreateRoofMesh(gRoofMesh);
    UCreateFloorMesh(gFloorMesh);
    UCreateSphereMesh(gSphereMesh);
    UCreateWindowMesh(gWindowMesh);
    UCreateBushMesh(gBushMesh);

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
        return EXIT_FAILURE;

    // Load texture
    const char* cubeTexFilename = "../resources/textures/vinyl-siding.jpg";
    if (!UCreateTexture(cubeTexFilename, gHouseStructureTextureId))
    {
        cout << "Failed to load texture " << cubeTexFilename << endl;
        return EXIT_FAILURE;
    }

    const char* pyramidTexFilename = "../resources/textures/asphalt-shingle.jpg";
    if (!UCreateTexture(pyramidTexFilename, gRoofTextureId))
    {
        cout << "Failed to load texture " << pyramidTexFilename << endl;
        return EXIT_FAILURE;
    }

    const char* floorTexFilename = "../resources/textures/grass.jpg";
    if (!UCreateTexture(floorTexFilename, gFloorTextureId))
    {
        cout << "Failed to load texture " << floorTexFilename << endl;
        return EXIT_FAILURE;
    }
    
    const char* sphereTexFilename = "../resources/textures/sun.jpg";
    if (!UCreateTexture(sphereTexFilename, gSphereTextureId))
    {
        cout << "Failed to load texture " << sphereTexFilename << endl;
        return EXIT_FAILURE;
    }

    const char* windowTexFilename = "../resources/textures/window.jpg";
    if (!UCreateTexture(windowTexFilename, gWindowTextureId))
    {
        cout << "Failed to load texture " << windowTexFilename << endl;
        return EXIT_FAILURE;
    }

    const char* bushTexFilename = "../resources/textures/bush.jpg";
    if (!UCreateTexture(bushTexFilename, gBushTextureId))
    {
        cout << "Failed to load texture " << bushTexFilename << endl;
        return EXIT_FAILURE;
    }


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URenderScene();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gHouseStructureMesh);
    UDestroyMesh(gRoofMesh);
    UDestroyMesh(gFloorMesh);
    UDestroyMesh(gSphereMesh);
    UDestroyMesh(gWindowMesh);
    UDestroyMesh(gBushMesh);

    // Release texture
    UDestroyTexture(gHouseStructureTextureId);
    UDestroyTexture(gRoofTextureId);
    UDestroyTexture(gFloorTextureId);
    UDestroyTexture(gSphereTextureId);
    UDestroyTexture(gWindowTextureId);
    UDestroyTexture(gBushTextureId);

    // Release shader program
    UDestroyShaderProgram(gProgramId);
    UDestroyShaderProgram(gLampProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  // Foward Movement
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  // Backward Movement
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  // Left Movement
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  // Right movement
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Upward movement
        gCamera.ProcessKeyboard(UPWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Downward movement
        gCamera.ProcessKeyboard(DOWNWARD, gDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // Switch between orthographic and perspective views
        isPerspectiveView = !isPerspectiveView;

    // Pause and resume the sun/moon orbiting
    static bool isLKeyDown = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !gIsLampOrbiting)
        gIsLampOrbiting = true;
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && gIsLampOrbiting)
        gIsLampOrbiting = false;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //Increases the camera speed if the mouse is scroll wheeled foward
    if (yoffset == 1) {
        gCamera.MovementSpeed = gCamera.MovementSpeed + 1;
    }
    //Decreases the camera speed if the mouse is scroll wheeled backward
    if (yoffset == -1) {
        if (gCamera.MovementSpeed > 1) // check to ensure the speed doesn't go negative or less than one
            gCamera.MovementSpeed = gCamera.MovementSpeed - 1;
    }
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


// Functioned called to render a frame
void URenderScene()
{
    const float angularVelocity = glm::radians(100.0f);
    if (gIsLampOrbiting)
    {
        // Position of the sun
        glm::vec4 newPosition = glm::rotate(angularVelocity * gDeltaTime, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(gLightPosition, 1.0f);
        gLightPosition.x = newPosition.x;
        gLightPosition.y = newPosition.y;
        gLightPosition.z = newPosition.z;

        // Position of the moon
        glm::vec4 newSecondPosition = glm::rotate(angularVelocity * gDeltaTime, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(gSecondLightPosition, 1.0f);
        gSecondLightPosition.x = newSecondPosition.x;
        gSecondLightPosition.y = newSecondPosition.y;
        gSecondLightPosition.z = newSecondPosition.z;
    }

    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Scales the object by 2
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    // 2. Rotates shape by 15 degrees in the x axis
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0, 1.0f, 0.0f));
    // 3. Place object at the origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, -10.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection;
    if (isPerspectiveView) {
        projection = glm::perspective(glm::radians(gCamera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f); //Utilize perspective view
    }
    else {
        projection = glm::ortho(-7.0f, 7.0f, 0.0f, 14.0f, 0.1f, 100.0f); //Utilize orthographic view
    }

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    // Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    // Reference matrix uniforms for the second light (moon)
    GLint secondLightColorLoc = glGetUniformLocation(gProgramId, "secondLightColor");
    GLint secondLightPositionLoc = glGetUniformLocation(gProgramId, "secondLightPos");

    // Assuming global variables gSecondLightColor and gSecondLightPosition exist
    // Pass second light data to the Cube Shader program's corresponding uniforms
    glUniform3f(secondLightColorLoc, gSecondLightColor.r, gSecondLightColor.g, gSecondLightColor.b);
    glUniform3f(secondLightPositionLoc, gSecondLightPosition.x, gSecondLightPosition.y, gSecondLightPosition.z);


    /// Roof Mesh ///

    GLint roofUVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2f(roofUVScaleLoc, 1.0f, 1.0f); // Set the texture for the roof

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gRoofMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gRoofTextureId);
    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gRoofMesh.nVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    /// Floor Mesh ///

    GLint floorUVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2f(floorUVScaleLoc, 100.0f, 100.0f); // Set the texture scale for the floor

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gFloorMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gFloorTextureId);
    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gFloorMesh.nVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    /// Window Mesh ///

    GLint windowUVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2f(windowUVScaleLoc, 1.0f, 1.0f);

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gWindowMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gWindowTextureId);
    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gWindowMesh.nVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    /// Bush Mesh ///

    GLint bushUVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2f(bushUVScaleLoc, 4.0f, 1.0f);

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gBushMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBushTextureId);
    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gBushMesh.nVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    /// House Mesh ///

    GLint houseUVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2f(houseUVScaleLoc, 5.0f, 5.0f); // Set the texture scale for the house

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gHouseStructureMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gHouseStructureTextureId);
    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gHouseStructureMesh.nVertices);
    glBindTexture(GL_TEXTURE_2D, 0);


    /// Sphere Mesh -- used for the sun/moon

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gSphereMesh.vao);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gSphereTextureId);

    /// Sun Mesh/Light 

    glUseProgram(gLampProgramId);
    //Transform the sphere as a visual que for the light source
    model = glm::translate(gLightPosition) * glm::scale(gLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    modelLoc = glGetUniformLocation(gLampProgramId, "model");
    viewLoc = glGetUniformLocation(gLampProgramId, "view");
    projLoc = glGetUniformLocation(gLampProgramId, "projection");

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, gSphereMesh.nVertices);

    /// Moon Mesh/Light 

    glUseProgram(gLampProgramId);
    //Transform the sphere as a visual que for the light source
    model = glm::translate(gSecondLightPosition) * glm::scale(gSecondLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    modelLoc = glGetUniformLocation(gLampProgramId, "model");
    viewLoc = glGetUniformLocation(gLampProgramId, "view");
    projLoc = glGetUniformLocation(gLampProgramId, "projection");

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, gSphereMesh.nVertices);

    // Deactivate the Vertex Array Object
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}


// Implements the Main House Mesh
void UCreateHouseStructureMesh(GLMesh& mesh)
{
    // Vertex data
    GLfloat verts[] = {
        // -- Main House Begin --
        
        // Back of the house
         1.5f,  0.5f,  2.0f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         1.5f,  0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 1.0f,
         1.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.5f, -0.5f,  2.0f,  0.0f,  1.0f, -1.0f,  1.0f, 0.0f,
         1.5f,  0.5f,  2.0f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,

        // Front side of the house
        -0.5f,  0.5f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

        // Left side of the house
        -0.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  1.0f, 0.0f,
         1.5f,  0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         1.5f,  0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,

        // Right side of the house
        -0.5f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
         1.5f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         1.5f,  0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         1.5f,  0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,

        // Floor of the house
        -0.5f, -0.5f, -3.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         1.5f, -0.5f, -3.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         1.5f, -0.5f,  2.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         1.5f, -0.5f,  2.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  2.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -3.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        // -- Main House End --


        // -- Roof Begin --

        // Side of Roof - Left
         1.53f, 0.5f, -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        -0.53f, 0.5f, -3.00f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
         0.56f, 1.2f, -3.00f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         0.56f, 1.2f, -3.00f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         0.55f, 1.2f, -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -0.53f, 0.5f, -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,

        // Side of Roof - Right
         1.53f, 0.5f, 2.00f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        -0.53f, 0.5f, 2.00f,  0.0f,  1.0f,  1.0f, 1.0f, 0.0f,
         0.56f, 1.2f, 2.00f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,
         0.56f, 1.2f, 2.00f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,
         0.55f, 1.2f, 2.00f,  0.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -0.53f, 0.5f, 2.00f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,

        // -- Roof End --


        // -- Garage Begin --

         // Back of the garage
         1.45f,  0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         1.45f,  0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 1.0f,
         1.45f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.45f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.45f, -0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 0.0f,
         1.45f,  0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,

         // Front side of the garage
         -0.3f,  0.5f, -5.5f, -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         -0.3f,  0.5f, -3.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         -0.3f, -0.5f, -3.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.3f, -0.5f, -3.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.3f, -0.5f, -5.5f, -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -0.3f,  0.5f, -5.5f, -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

         // Left side of the garage
         -0.3f, -0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
          1.45f, -0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 0.0f,
          1.45f,  0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
          1.45f,  0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         -0.3f,  0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  0.0f, 1.0f,
         -0.3f, -0.5f, -5.5f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,

         // Side of Garage Roof - Left
         1.48f, 0.5f, -5.5f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        -0.33f, 0.5f, -5.5f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
         0.56f, 1.10f, -5.5f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         0.56f, 1.10f, -5.5f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         0.55f, 1.10f, -5.5f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -0.33f, 0.5f, -5.5f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,

         // -- Garage End --
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the Roof Mesh
void UCreateRoofMesh(GLMesh& mesh)
{
    // Vertex data for the pyramid
    GLfloat pyramidVerts[] = {
        // -- House Roof Begin --

        // Angled Side of Roof - Back
         0.56f, 1.2f,  2.03f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
         0.56f, 1.2f, -3.03f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
         1.53f, 0.5f, -3.03f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.53f, 0.5f, -3.03f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.53f, 0.5f,  2.03f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         0.56f, 1.2f,  2.03f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,

        // Angled Side of Roof - Front
         0.55f, 1.2f,  2.03f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
         0.55f, 1.2f, -3.03f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.53f, 0.5f, -3.03f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        -0.53f, 0.5f, -3.03f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        -0.53f, 0.5f,  2.03f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.55f, 1.2f,  2.03f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,

        // Bottom of the roof
        -0.53f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
         1.53f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         1.53f, 0.5f,  2.00f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         1.53f, 0.5f,  2.00f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        -0.53f, 0.5f,  2.00f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -0.53f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        // Top of the roof - Reference Beam
        0.55f, 1.2f, -3.03f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        0.56f, 1.2f, -3.03f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.56f, 1.2f,  2.03f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.56f, 1.2f,  2.03f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.55f, 1.2f,  2.03f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.55f, 1.2f, -3.03f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,

        // -- House Roof End --
        

        // -- Garage Begin --

         // Angled Side of Roof - Back
         0.56f, 1.10f, -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
         0.56f, 1.10f, -5.53f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
         1.48f, 0.5f,  -5.53f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.48f, 0.5f,  -5.53f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.48f, 0.5f,  -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         0.56f, 1.10f, -3.00f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,

          // Angled Side of Roof - Front
          0.55f, 1.10f, -3.00f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
          0.55f, 1.10f, -5.53f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         -0.33f, 0.5f,  -5.53f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         -0.33f, 0.5f,  -5.53f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         -0.33f, 0.5f,  -3.00f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
          0.55f, 1.10f, -3.00f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,

          // Bottom of the roof
         -0.33f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
          1.48f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
          1.48f, 0.5f, -5.50f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
          1.48f, 0.5f, -5.50f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         -0.33f, 0.5f, -5.50f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         -0.33f, 0.5f, -3.00f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

          // Top of the roof - Reference Beam
          0.55f, 1.10f, -3.00f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
          0.56f, 1.10f, -3.00f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
          0.56f, 1.10f, -5.53f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
          0.56f, 1.10f, -5.53f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
          0.55f, 1.10f, -5.53f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
          0.55f, 1.10f, -3.00f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,

         // -- Garage End --
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(pyramidVerts) / (sizeof(pyramidVerts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerts), pyramidVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the Scene Floor Mesh
void UCreateFloorMesh(GLMesh& mesh)
{
    // Vertex data for the pyramid
    GLfloat floorVerts[] = {
        // Floor of the scene

        -100.0f, -0.501f, -100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         100.0f, -0.501f, -100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         100.0f, -0.501f,  100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         100.0f, -0.501f,  100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -100.0f, -0.501f,  100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -100.0f, -0.501f, -100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(floorVerts) / (sizeof(floorVerts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVerts), floorVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the Sphere Mesh
void UCreateSphereMesh(GLMesh& mesh)
{
    int subdivisions = 64;

    std::vector<GLfloat> sphereVerts;
    std::vector<GLuint> indices;

    // Function to generate vertices for a unit sphere
    auto generateSphereVertices = [](int subdivisions, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
        const double PI = 3.1415926535897932384626433832795;
        const double TWO_PI = 2.0 * PI;

        for (int i = 0; i <= subdivisions; i++) {
            double lat0 = PI * (-0.5 + static_cast<double>(i - 1) / subdivisions);
            double z0 = std::sin(lat0);
            double zr0 = std::cos(lat0);

            double lat1 = PI * (-0.5 + static_cast<double>(i) / subdivisions);
            double z1 = std::sin(lat1);
            double zr1 = std::cos(lat1);

            for (int j = 0; j <= subdivisions; j++) {
                double lng = TWO_PI * static_cast<double>(j) / subdivisions;
                double x = std::cos(lng);
                double y = std::sin(lng);

                vertices.push_back(static_cast<GLfloat>(x * zr0)); // X
                vertices.push_back(static_cast<GLfloat>(y * zr0)); // Y
                vertices.push_back(static_cast<GLfloat>(z0));      // Z
                vertices.push_back(static_cast<GLfloat>(x * zr0)); // Normal X
                vertices.push_back(static_cast<GLfloat>(y * zr0)); // Normal Y
                vertices.push_back(static_cast<GLfloat>(z0));      // Normal Z
                vertices.push_back(static_cast<GLfloat>(static_cast<double>(j) / subdivisions)); // U
                vertices.push_back(static_cast<GLfloat>(static_cast<double>(i - 1) / subdivisions)); // V

                vertices.push_back(static_cast<GLfloat>(x * zr1)); // X
                vertices.push_back(static_cast<GLfloat>(y * zr1)); // Y
                vertices.push_back(static_cast<GLfloat>(z1));      // Z
                vertices.push_back(static_cast<GLfloat>(x * zr1)); // Normal X
                vertices.push_back(static_cast<GLfloat>(y * zr1)); // Normal Y
                vertices.push_back(static_cast<GLfloat>(z1));      // Normal Z
                vertices.push_back(static_cast<GLfloat>(static_cast<double>(j) / subdivisions)); // U
                vertices.push_back(static_cast<GLfloat>(static_cast<double>(i) / subdivisions)); // V
            }
        }

        for (int i = 0; i < subdivisions; i++) {
            for (int j = 0; j < subdivisions; j++) {
                int first = (i * (subdivisions + 1)) + j;
                int second = first + subdivisions + 1;

                // First triangle
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                // Second triangle
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

    };

    // Generate sphere vertices and indices
    generateSphereVertices(subdivisions, sphereVerts, indices);

    mesh.nVertices = static_cast<GLuint>(sphereVerts.size() / 8);

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sphereVerts.size(), &sphereVerts[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    GLint stride = sizeof(float) * 8;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr); // Positions
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3)); // Normals
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6)); // Texture Coordinates
    glEnableVertexAttribArray(2);
}

// Implements the Window Mesh
void UCreateWindowMesh(GLMesh& mesh)
{
    // Vertex data for the pyramid
    GLfloat windowVerts[] = {

        // Window One - Leftmost

        -0.501f,  0.3f, -2.2f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.501f,  0.3f, -2.7f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.501f, -0.2f, -2.7f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f, -2.7f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f, -2.2f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.501f,  0.3f, -2.2f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

        // Window Two

        -0.501f,  0.3f, -1.8f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.501f,  0.3f, -1.3f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.501f, -0.2f, -1.3f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f, -1.3f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f, -1.8f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.501f,  0.3f, -1.8f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

        // Window Three

        -0.501f,  0.3f,  0.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.501f,  0.3f,  0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.501f, -0.2f,  0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f,  0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f,  0.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.501f,  0.3f,  0.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

        // Window Three

        -0.501f,  0.3f,  1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.501f,  0.3f,  1.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.501f, -0.2f,  1.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f,  1.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.501f, -0.2f,  1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.501f,  0.3f,  1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(windowVerts) / (sizeof(windowVerts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVerts), windowVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the Front Bush Mesh
void UCreateBushMesh(GLMesh& mesh)
{
    // Vertex data for the pyramid
    GLfloat bushVerts[] = {
         //Face          //Negative Z Normal  Texture Coords.

         // -- Right Bush Begin --

         // Front side of the bush
         -0.8f, -0.3f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         -0.8f, -0.3f, -0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         -0.8f, -0.5f, -0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.8f, -0.5f, -0.5f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.8f, -0.5f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -0.8f, -0.3f,  2.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

         //Left side of the bush
         -0.8f, -0.3f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         -0.5f, -0.3f, -0.5f,  0.0f,  1.0f, -1.0f,  0.3f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.3f, 0.3f,
         -0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.3f, 0.3f,
         -0.8f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f, 0.3f,
         -0.8f, -0.3f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,

         // Right side of the bush
         -0.8f, -0.3f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
         -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.0f,
         -0.5f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.3f,
         -0.5f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.3f,
         -0.8f, -0.5f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.3f,
         -0.8f, -0.3f,  2.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,

         // Top of the bush
         -0.8f, -0.3f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.3f, -0.5f,  0.0f,  1.0f,  0.0f, 0.3f, 0.0f,
         -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f, 0.3f, 1.5f,
         -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f, 0.3f, 1.5f,
         -0.8f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.5f,
         -0.8f, -0.3f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,

         // -- Right Bush End --


         // -- Left Bush Begin --

         // Front side of the bush
         -0.8f, -0.3f, -1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         -0.8f, -0.3f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         -0.8f, -0.5f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.8f, -0.5f, -3.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.8f, -0.5f, -1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -0.8f, -0.3f, -1.0f,  -1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

         // Left side of the bush
         -0.8f, -0.3f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         -0.5f, -0.3f, -3.0f,  0.0f,  1.0f, -1.0f,  0.3f, 0.0f,
         -0.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.3f, 0.3f,
         -0.5f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.3f, 0.3f,
         -0.8f, -0.5f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.3f,
         -0.8f, -0.3f, -3.0f,  0.0f,  1.0f, -1.0f,  0.0f, 0.0f,

         // Right side of the bush
         -0.8f, -0.3f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
         -0.5f, -0.3f, -1.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.0f,
         -0.5f, -0.5f, -1.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.3f,
         -0.5f, -0.5f, -1.0f,  0.0f,  1.0f,  1.0f,  0.3f, 0.3f,
         -0.8f, -0.5f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.3f,
         -0.8f, -0.3f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f,

         // Top of the bush
         -0.8f, -0.3f, -3.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.3f, -3.0f,  0.0f,  1.0f,  0.0f, 0.3f, 0.0f,
         -0.5f, -0.3f, -1.0f,  0.0f,  1.0f,  0.0f, 0.3f, 1.5f,
         -0.5f, -0.3f, -1.0f,  0.0f,  1.0f,  0.0f, 0.3f, 1.5f,
         -0.8f, -0.3f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.5f,
         -0.8f, -0.3f, -3.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,

         // -- Left Bush End --
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(bushVerts) / (sizeof(bushVerts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(bushVerts), bushVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}


/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}


// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}
