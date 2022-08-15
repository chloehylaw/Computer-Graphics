//
// COMP 371 Assignment Framework
// Code skeleton created by Nicolas Bergeron on 20/06/2019.
// 
// Chloe Hei Yu Law - 40173275
// COMP 371 - Quiz 02
//

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include <FreeImageIO.h>

#include "camera.h"
#include "shader.h"

using namespace glm;
using namespace std;

// Window settings
const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 768;

// Camera
Camera camera(vec3(1.0f, 1.0f, 30.0f));
Camera front(vec3(0.0f, 1.0f, -10.0f));
Camera back(vec3(0.0f, 1.0f, 10.0f));
Camera rotating(vec3(0.0f, 0.0f, 5.0f));

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

bool fpsOn = true;
bool frontCam = false;
bool backCam = false;
bool rotatingCam = false;

// Lighting
vec3 lightPos(0.0f, 5.0f, 30.0f);
bool lightSwitch = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Texture
GLuint blueTextureID;
GLuint redTextureID;
GLuint orangeTextureID;
GLuint greenTextureID;
GLuint metallicTextureID;
GLuint glossyTextureID;
GLuint floorTextureID;
GLuint skyTextureID;
GLuint xaxisTextureID;
GLuint yaxisTextureID;
GLuint zaxisTextureID;

// Main base cube
mat4 baseModel = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
vec3 baseModelPos = vec3(1.0f);

unsigned int VAO;

// lab
GLuint loadTexture(const char* filename)
{
    // Load image using the Free Image library
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    FIBITMAP* image = FreeImage_Load(format, filename);
    FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

    // Create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Retrieve width and hight
    int width = FreeImage_GetWidth(image32bits);
    int height = FreeImage_GetHeight(image32bits);

    // This will upload the texture to the GPU memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

    // Free images
    FreeImage_Unload(image);
    FreeImage_Unload(image32bits);

    return textureId;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        baseModel = translate(baseModel, vec3(0.0f, 0.0f, -0.1f));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        baseModel = translate(baseModel, vec3(0.0f, 0.0f, 0.1f));

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        baseModel = translate(baseModel, vec3(-0.1f, 0.0f, 0.0f));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        baseModel = translate(baseModel, vec3(0.1f, 0.0f, 0.0f));

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        baseModel = rotate(baseModel, 0.01f, vec3(0.0f, 1.0f, 0.0f));

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        baseModel = rotate(baseModel, -0.01f, vec3(0.0f, 1.0f, 0.0f));


    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera left
    {
        camera.ProcessKeyboard(LEFT, deltaTime * 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera right
    {
        camera.ProcessKeyboard(RIGHT, deltaTime * 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera back
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime * 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera front
    {
        camera.ProcessKeyboard(FORWARD, deltaTime * 2.0f);
    }
}

void keyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Turn lightsource on and off
    if (key == GLFW_KEY_L && action != GLFW_RELEASE)
    {
        lightSwitch = !lightSwitch;
    }

    // Switch to first person view
    if (key == GLFW_KEY_M && action != GLFW_RELEASE)
    {
        if (frontCam)
        {
            frontCam = false;
            fpsOn = !fpsOn;
        }
        else
        {
            if (backCam)
            {
                backCam = false;
                frontCam = true;
            }
            else if (rotatingCam)
            {
                rotatingCam = false;
                frontCam = true;
            }
            else
            {
                fpsOn = !fpsOn;
                frontCam = true;
            }
        }
    }

    // Switch to back person view
    if (key == GLFW_KEY_B && action != GLFW_RELEASE)
    {
        if (backCam)
        {
            backCam = false;
            fpsOn = !fpsOn;
        }
        else
        {
            if (frontCam)
            {
                frontCam = false;
                backCam = true;
            }
            else if (rotatingCam)
            {
                rotatingCam = false;
                backCam = true;
            }
            else
            {
                fpsOn = !fpsOn;
                backCam = true;
            }
        }
    }

    // Circle around center
    if (key == GLFW_KEY_C && action != GLFW_RELEASE)
    {
        if (rotatingCam)
        {
            rotatingCam = false;
            fpsOn = !fpsOn;
        }
        else
        {
            if (frontCam)
            {
                frontCam = false;
                rotatingCam = true;
            }
            else if (backCam)
            {
                backCam = false;
                rotatingCam = true;
            }
            else
            {
                fpsOn = !fpsOn;
                rotatingCam = true;
            }
        }
    }

    if (key == GLFW_KEY_R && action != GLFW_RELEASE)
    {
        frontCam = false;
        backCam = false;
        fpsOn = true;
        lightSwitch = true;
        camera = Camera(vec3(1.0f, 1.0f, 30.0f));
        rotating = Camera(vec3(1.0f, 3.0f, 30.0f));
    }
}

// https://learnopengl.com
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// https://learnopengl.com
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    if (fpsOn)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// https://learnopengl.com
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fpsOn)
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void drawGround(Shader shaderProgram)
{
    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.4f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.1f, 100.0f));
    shaderProgram.setMat4("model", groundWorldMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSkybox(Shader shaderProgram)
{
    mat4 skyboxMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 100.0f, 100.0f));
    shaderProgram.setMat4("model", skyboxMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyTextureID);
    shaderProgram.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
    shaderProgram.setVec3("light2.specular", 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawAxis(Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, xaxisTextureID);
    mat4 xAxis = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
    shaderProgram.setMat4("model", xAxis);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, yaxisTextureID);
    mat4 yAxis = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
    shaderProgram.setMat4("model", yAxis);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, zaxisTextureID);
    mat4 zAxis = translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
    shaderProgram.setMat4("model", zAxis);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawC(const mat4& c_base, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, redTextureID);

    shaderProgram.setMat4("model", c_base);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_1 = c_base;
    c_1 = translate(c_1, vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", c_1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_2 = c_base;
    c_2 = translate(c_2, vec3(0.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", c_2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_3 = c_base;
    c_3 = translate(c_3, vec3(0.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", c_3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_4 = c_base;
    c_4 = translate(c_4, vec3(0.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", c_4);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_5 = c_base;
    c_5 = translate(c_5, vec3(1.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", c_5);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_6 = c_base;
    c_6 = translate(c_6, vec3(2.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", c_6);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_7 = c_base;
    c_7 = translate(c_7, vec3(1.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", c_7);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_8 = c_base;
    c_8 = translate(c_8, vec3(2.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", c_8);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawH(const mat4& h_base, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blueTextureID);

    shaderProgram.setMat4("model", h_base);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_1 = h_base;
    h_1 = translate(h_1, vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", h_1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_2 = h_base;
    h_2 = translate(h_2, vec3(0.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", h_2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_3 = h_base;
    h_3 = translate(h_3, vec3(0.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", h_3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_4 = h_base;
    h_4 = translate(h_4, vec3(0.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", h_4);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_5 = h_base;
    h_5 = translate(h_5, vec3(1.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", h_5);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_6 = h_base;
    h_6 = translate(h_6, vec3(2.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", h_6);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_7 = h_base;
    h_7 = translate(h_7, vec3(2.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", h_7);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_8 = h_base;
    h_8 = translate(h_8, vec3(2.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", h_8);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_9 = h_base;
    h_9 = translate(h_9, vec3(2.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", h_9);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_10 = h_base;
    h_10 = translate(h_10, vec3(2.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", h_10);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void draw4(const mat4& _4_base, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, orangeTextureID);

    shaderProgram.setMat4("model", _4_base);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_1 = _4_base;
    l_1 = translate(l_1, vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", l_1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_2 = _4_base;
    l_2 = translate(l_2, vec3(0.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", l_2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_3 = _4_base;
    l_3 = translate(l_3, vec3(0.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", l_3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_4 = _4_base;
    l_4 = translate(l_4, vec3(0.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", l_4);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_5 = _4_base;
    l_5 = translate(l_5, vec3(-1.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", l_5);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_6 = _4_base;
    l_6 = translate(l_6, vec3(-2.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", l_6);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_7 = _4_base;
    l_7 = translate(l_7, vec3(-2.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", l_7);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_8 = _4_base;
    l_8 = translate(l_8, vec3(-2.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", l_8);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void draw0(const mat4& _0_base, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, greenTextureID);

    shaderProgram.setMat4("model", _0_base);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_1 = _0_base;
    o_1 = translate(o_1, vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", o_1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_2 = _0_base;
    o_2 = translate(o_2, vec3(0.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", o_2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_3 = _0_base;
    o_3 = translate(o_3, vec3(0.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", o_3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_4 = _0_base;
    o_4 = translate(o_4, vec3(0.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", o_4);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_5 = _0_base;
    o_5 = translate(o_5, vec3(1.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", o_5);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_6 = _0_base;
    o_6 = translate(o_6, vec3(1.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", o_6);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_7 = _0_base;
    o_7 = translate(o_7, vec3(2.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", o_7);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_8 = _0_base;
    o_8 = translate(o_8, vec3(2.0f, 1.0f, 0.0f));
    shaderProgram.setMat4("model", o_8);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_9 = _0_base;
    o_9 = translate(o_9, vec3(2.0f, 2.0f, 0.0f));
    shaderProgram.setMat4("model", o_9);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_10 = _0_base;
    o_10 = translate(o_10, vec3(2.0f, 3.0f, 0.0f));
    shaderProgram.setMat4("model", o_10);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_11 = _0_base;
    o_11 = translate(o_11, vec3(2.0f, 4.0f, 0.0f));
    shaderProgram.setMat4("model", o_11);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSkateboard(const mat4& baseSkateboard, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, metallicTextureID);

    mat4 board = baseSkateboard;

    board = translate(board, vec3(0.0f, 0.65f, 0.0f)) * scale(mat4(1.0), vec3(20.0f, 0.5f, 2.5f));
    shaderProgram.setMat4("model", board);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSkateboardWheels(const mat4& baseSkateboard, Shader shaderProgram)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glossyTextureID);

    mat4 wheel1 = baseSkateboard;
    mat4 wheel2 = baseSkateboard;
    mat4 wheel3 = baseSkateboard;
    mat4 wheel4 = baseSkateboard;

    wheel1 = translate(wheel1, vec3(-8.0f, 0.1f, -1.0f)) * scale(mat4(1.0), vec3(0.6f, 0.6f, 0.6f));
    wheel2 = translate(wheel2, vec3(-8.0f, 0.1f, 1.0f)) * scale(mat4(1.0), vec3(0.6f, 0.6f, 0.6f));
    wheel3 = translate(wheel3, vec3(8.0f, 0.1f, -1.0f)) * scale(mat4(1.0), vec3(0.6f, 0.6f, 0.6f));
    wheel4 = translate(wheel4, vec3(8.0f, 0.1f, 1.0f)) * scale(mat4(1.0), vec3(0.6f, 0.6f, 0.6f));

    shaderProgram.setMat4("model", wheel1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shaderProgram.setMat4("model", wheel2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shaderProgram.setMat4("model", wheel3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shaderProgram.setMat4("model", wheel4);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void renderScene(Shader shader, mat4 baseModel)
{
    shader.use();
    shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("light.specular", 0.3f, 0.3f, 0.3f);
    shader.setFloat("light.shininess", 64.0f);
    shader.setVec3("light2.ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("light2.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("light2.specular", 0.3f, 0.3f, 0.3f);
    shader.setFloat("light2.shininess", 64.0f);

    // Draw axis lines
    drawAxis(shader);
    
    glBindVertexArray(VAO);

    // Draw ground
    drawGround(shader);

    // Render each letter
    // Letter base cubes
    mat4 c_base = translate(baseModel, vec3(-8.5f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 _4_base = translate(baseModel, vec3(-1.5f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 h_base = translate(baseModel, vec3(1.5f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 _0_base = translate(baseModel, vec3(6.5f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

    //glCullFace(GL_BACK);
    drawC(c_base, shader);
    draw4(_4_base, shader);
    drawH(h_base, shader);
    draw0(_0_base, shader);

    // Skateboard base cube
    mat4 skateboard = translate(baseModel, vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

    // Render skateboard
    //glEnable(GL_CULL_FACE);
    drawSkateboard(skateboard, shader);

    // Render skateboard wheels
    drawSkateboardWheels(skateboard, shader);

    // Draw skybox
    drawSkybox(shader);
}


int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version - lab
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024x768 - lab
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MIDTERM - 02", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLEW - lab
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Load Textures
    blueTextureID = loadTexture("../Textures/Blue.jpg");
    redTextureID = loadTexture("../Textures/Red.jpg");
    orangeTextureID = loadTexture("../Textures/Orange.jpg");
    greenTextureID = loadTexture("../Textures/Green.jpg");
    metallicTextureID = loadTexture("../Textures/Metallic.jpg");
    glossyTextureID = loadTexture("../Textures/Glossy.jpg");
    floorTextureID = loadTexture("../Textures/Floor.jpg");
    skyTextureID = loadTexture("../Textures/Sky.jpg");
    xaxisTextureID = loadTexture("../Textures/xaxis.jpg");
    yaxisTextureID = loadTexture("../Textures/yaxis.jpg");
    zaxisTextureID = loadTexture("../Textures/zaxis.jpg");

    // Compile and link shaders
    Shader textureShaderProgram("../Shaders/texture.vs", "../Shaders/texture.fs");
    Shader shadowShaderProgram("../Shaders/shadow.vs", "../Shaders/shadow.fs");

    // lab
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO;

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // https://learnopengl.com
    // configure depth map FBO 
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int depthMap2;
    glGenTextures(1, &depthMap2);
    glBindTexture(GL_TEXTURE_2D, depthMap2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap2, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, 1024, 768);

    textureShaderProgram.use();
    textureShaderProgram.setInt("diffuseTexture", 0);
    textureShaderProgram.setInt("shadowMap", 1);

    mat4 trans = mat4(1.0f);
    mat4 model = mat4(1.0f);
    mat4 view = mat4(1.0f);
    mat4 projection = mat4(1.0f);

    view = translate(view, vec3(0.0f, 0.0f, -3.0f));
    model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    projection = perspective(radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetKeyCallback(window, keyCallbacks);


    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        textureShaderProgram.use();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //inputs
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotating.Position = vec3(cos(glfwGetTime()) * 20.0f, 3.0f, sin(glfwGetTime()) * 20.0f);

        // https://learnopengl.com
        // First: render depth of scene to texture - light's perspective
        glCullFace(GL_FRONT);
        mat4 lightProjection, lightView, lightSpaceMatrix;

        float near_plane = 1.0f;
        float far_plane = 7.5f;

        lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        lightView = lookAt(lightPos, vec3(0.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shadowShaderProgram.use();
        shadowShaderProgram.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shadowShaderProgram.setMat4("lightSpaceMatrix2", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(shadowShaderProgram, baseModel);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        // reset viewport
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Second: render scene as normal using depth/shadow map
        glCullFace(GL_FRONT);
        lightProjection = ortho(-10.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        lightView = lookAt(lightPos, vec3(0.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shadowShaderProgram.use();
        shadowShaderProgram.setMat4("lightSpaceMatrix2", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(shadowShaderProgram, baseModel);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        // reset viewport
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the three cameras
        textureShaderProgram.use();
        int modelLoc = glGetUniformLocation(textureShaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        if (fpsOn)
        {
            mat4 projection = perspective(radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            textureShaderProgram.setMat4("projection", projection);
            mat4 view = camera.GetViewMatrix();
            textureShaderProgram.setMat4("view", view);
        }
        else if (backCam)
        {
            mat4 projection = perspective(radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            textureShaderProgram.setMat4("projection", projection);
            mat4 view = camera.GetViewMatrix() * lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
            textureShaderProgram.setMat4("view", view);
        }
        else if (frontCam)
        {
            mat4 projection = perspective(radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            textureShaderProgram.setMat4("projection", projection);
            mat4 view = camera.GetViewMatrix() * lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
            textureShaderProgram.setMat4("view", view);
        }
        else if (rotatingCam)
        {
            mat4 projection = perspective(radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            textureShaderProgram.setMat4("projection", projection);
            mat4 view = camera.GetViewMatrix() * lookAt(rotating.Position, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
            textureShaderProgram.setMat4("view", view);
        }

        // https://learnopengl.com
        // Set light uniforms
        vec3 lightColor = vec3(1.0f);
        vec3 diffuseColor = lightColor * vec3(0.5f);
        vec3 ambientColor = diffuseColor * vec3(0.2f);
        vec3 specularColor = ambientColor * vec3(0.3f);

        textureShaderProgram.setVec3("viewPos", camera.Position);

        textureShaderProgram.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        textureShaderProgram.setVec3("light.position", lightPos);
        textureShaderProgram.setVec3("light.direction", vec3(0.0f, 0.0f, -1.0f));
        textureShaderProgram.setVec3("light.diffuse", diffuseColor);
        textureShaderProgram.setVec3("light.ambiant", ambientColor);
        textureShaderProgram.setVec3("light.specular", specularColor);
        textureShaderProgram.setFloat("light.cutOff", cos(radians(lightSwitch ? 30.0f : 0.0f)));
        textureShaderProgram.setFloat("light.constant", 1.0f);
        textureShaderProgram.setFloat("light.linear", 0.09f);
        textureShaderProgram.setFloat("light.quadratic", 0.0031f);


        textureShaderProgram.setMat4("lightSpaceMatrix2", lightSpaceMatrix);
        textureShaderProgram.setVec3("light2.position", lightPos);
        textureShaderProgram.setVec3("light2.direction", vec3(0.0f, 0.0f, -1.0f));
        textureShaderProgram.setVec3("light2.diffuse", diffuseColor);
        textureShaderProgram.setVec3("light2.ambiant", ambientColor);
        textureShaderProgram.setVec3("light2.specular", specularColor);
        textureShaderProgram.setFloat("light2.cutOff", cos(radians(30.0f)));
        textureShaderProgram.setFloat("light2.constant", 1.0f);
        textureShaderProgram.setFloat("light2.linear", 0.09f);
        textureShaderProgram.setFloat("light2.quadratic", 0.0031f);
        
        float timeValue = glfwGetTime();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(VAO);
        view = lookAt(vec3(0.0f, 0.0f, 3.0f),  vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        renderScene(textureShaderProgram, baseModel);

        // End frame
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}