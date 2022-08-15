//
// COMP 371 Assignment Framework
// Code skeleton created by Nicolas Bergeron on 20/06/2019.
// 
// Chloe Hei Yu Law - 40173275
// COMP 371 - Quiz 01
//

#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

using namespace glm;
using namespace std;

// allow to show grid/axis
bool showGrid = true;
bool showAxis = true;

// Taken from lab
const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix = mat4(1.0);;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

// Taken from lab
const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

// Taken from lab
int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createLineVertexBufferObject()
{
    vec3 vertices[] =
    {
        // point                      color
        vec3(0.0f, 0.0f, 0.0f),  vec3(1.0f, 0.0f, 0.0f),    // x-axis
        vec3(5.0f, 0.0f, 0.0f),  vec3(1.0f, 0.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f, 1.0f, 0.0f),    // y-axis
        vec3(0.0f, 5.0f, 0.0f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f),    // z-axis
        vec3(0.0f, 0.0f, 5.0f),  vec3(0.0f, 0.0f, 1.0f),

        vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f, 0.0f, 0.0f),    // grid x-axis
        vec3(1.0f, 0.0f, 0.0f),  vec3(0.0f, 0.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f, 0.0f, 0.0f),    // gridz-axis
        vec3(0.0f, 0.0f, 1.0f),  vec3(0.0f, 0.0f, 0.0f)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)(2 * sizeof(vec3)));
    glEnableVertexAttribArray(1);

    return vertexBufferObject;
}

int createCubeVertexBufferObject(vec3 color)
{
    vec3 vertexArray[] = {
        // Position              Color
        vec3(-0.5f,-0.5f,-0.5f), color,
        vec3(-0.5f,-0.5f, 0.5f), color,
        vec3(-0.5f, 0.5f, 0.5f), color,

        vec3(-0.5f,-0.5f,-0.5f), color,
        vec3(-0.5f, 0.5f, 0.5f), color,
        vec3(-0.5f, 0.5f,-0.5f), color,

        vec3(0.5f, 0.5f,-0.5f), color,
        vec3(-0.5f,-0.5f,-0.5f), color,
        vec3(-0.5f, 0.5f,-0.5f), color,

        vec3(0.5f, 0.5f,-0.5f), color,
        vec3(0.5f,-0.5f,-0.5f), color,
        vec3(-0.5f,-0.5f,-0.5f),color,

        vec3(0.5f,-0.5f, 0.5f), color,
        vec3(-0.5f,-0.5f,-0.5f), color,
        vec3(0.5f,-0.5f,-0.5f), color,

        vec3(0.5f,-0.5f, 0.5f), color,
        vec3(-0.5f,-0.5f, 0.5f), color,
        vec3(-0.5f,-0.5f,-0.5f), color,

        vec3(-0.5f, 0.5f, 0.5f), color,
        vec3(-0.5f,-0.5f, 0.5f), color,
        vec3(0.5f,-0.5f, 0.5f), color,

        vec3(0.5f, 0.5f, 0.5f), color,
        vec3(-0.5f, 0.5f, 0.5f), color,
        vec3(0.5f,-0.5f, 0.5f), color,

        vec3(0.5f, 0.5f, 0.5f), color,
        vec3(0.5f,-0.5f,-0.5f), color,
        vec3(0.5f, 0.5f,-0.5f), color,

        vec3(0.5f,-0.5f,-0.5f), color,
        vec3(0.5f, 0.5f, 0.5f), color,
        vec3(0.5f,-0.5f, 0.5f), color,

        vec3(0.5f, 0.5f, 0.5f), color,
        vec3(0.5f, 0.5f,-0.5f), color,
        vec3(-0.5f, 0.5f,-0.5f), color,

        vec3(0.5f, 0.5f, 0.5f), color,
        vec3(-0.5f, 0.5f,-0.5f), color,
        vec3(-0.5f, 0.5f, 0.5f), color
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    glBindVertexArray(vertexBufferObject);

    return vertexArrayObject;
}

// Render letter methods
void drawC(const mat4& c_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_1 = c_base;
    c_1 = translate(c_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_2 = c_base;
    c_2 = translate(c_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_3 = c_base;
    c_3 = translate(c_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_4 = c_base;
    c_4 = translate(c_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_5 = c_base;
    c_5 = translate(c_5, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_6 = c_base;
    c_6 = translate(c_6, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_7 = c_base;
    c_7 = translate(c_7, vec3(1.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 c_8 = c_base;
    c_8 = translate(c_8, vec3(2.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &c_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawH(const mat4& h_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_1 = h_base;
    h_1 = translate(h_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_2 = h_base;
    h_2 = translate(h_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_3 = h_base;
    h_3 = translate(h_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_4 = h_base;
    h_4 = translate(h_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_5 = h_base;
    h_5 = translate(h_5, vec3(1.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_6 = h_base;
    h_6 = translate(h_6, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_7 = h_base;
    h_7 = translate(h_7, vec3(2.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_8 = h_base;
    h_8 = translate(h_8, vec3(2.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_9 = h_base;
    h_9 = translate(h_9, vec3(2.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_9[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 h_10 = h_base;
    h_10 = translate(h_10, vec3(2.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &h_10[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawL(const mat4& l_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_1 = l_base;
    l_1 = translate(l_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_2 = l_base;
    l_2 = translate(l_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_3 = l_base;
    l_3 = translate(l_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_4 = l_base;
    l_4 = translate(l_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_5 = l_base;
    l_5 = translate(l_5, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 l_6 = l_base;
    l_6 = translate(l_6, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &l_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawO(const mat4& o_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_1 = o_base;
    o_1 = translate(o_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_2 = o_base;
    o_2 = translate(o_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_3 = o_base;
    o_3 = translate(o_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_4 = o_base;
    o_4 = translate(o_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_5 = o_base;
    o_5 = translate(o_5, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_6 = o_base;
    o_6 = translate(o_6, vec3(1.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_7 = o_base;
    o_7 = translate(o_7, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_8 = o_base;
    o_8 = translate(o_8, vec3(2.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_9 = o_base;
    o_9 = translate(o_9, vec3(2.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_9[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_10 = o_base;
    o_10 = translate(o_10, vec3(2.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_10[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 o_11 = o_base;
    o_11 = translate(o_11, vec3(2.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &o_11[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawE(const mat4& e_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_1 = e_base;
    e_1 = translate(e_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_2 = e_base;
    e_2 = translate(e_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_3 = e_base;
    e_3 = translate(e_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_4 = e_base;
    e_4 = translate(e_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_5 = e_base;
    e_5 = translate(e_5, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_6 = e_base;
    e_6 = translate(e_6, vec3(1.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_7 = e_base;
    e_7 = translate(e_7, vec3(1.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_8 = e_base;
    e_8 = translate(e_8, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_9 = e_base;
    e_9 = translate(e_9, vec3(2.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_9[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 e_10 = e_base;
    e_10 = translate(e_10, vec3(2.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &e_10[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawA(const mat4& a_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_1 = a_base;
    a_1 = translate(a_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_2 = a_base;
    a_2 = translate(a_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_3 = a_base;
    a_3 = translate(a_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_4 = a_base;
    a_4 = translate(a_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_5 = a_base;
    a_5 = translate(a_5, vec3(1.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_6 = a_base;
    a_6 = translate(a_6, vec3(1.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_7 = a_base;
    a_7 = translate(a_7, vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_8 = a_base;
    a_8 = translate(a_8, vec3(2.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_9 = a_base;
    a_9 = translate(a_9, vec3(2.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_9[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_10 = a_base;
    a_10 = translate(a_10, vec3(2.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_10[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 a_11 = a_base;
    a_11 = translate(a_11, vec3(2.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &a_11[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawW(const mat4& w_base, const GLuint& worldMatrixLocation, char renderMode)
{
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_base[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_1 = w_base;
    w_1 = translate(w_1, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_2 = w_base;
    w_2 = translate(w_2, vec3(0.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_2[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_3 = w_base;
    w_3 = translate(w_3, vec3(0.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_3[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_4 = w_base;
    w_4 = translate(w_4, vec3(0.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_4[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_5 = w_base;
    w_5 = translate(w_5, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_5[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_6 = w_base;
    w_6 = translate(w_6, vec3(2.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_6[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_7 = w_base;
    w_7 = translate(w_7, vec3(2.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_7[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_8 = w_base;
    w_8 = translate(w_8, vec3(3.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_8[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_9 = w_base;
    w_9 = translate(w_9, vec3(4.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_9[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_10 = w_base;
    w_10 = translate(w_10, vec3(4.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_10[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_11 = w_base;
    w_11 = translate(w_11, vec3(4.0f, 2.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_11[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_12 = w_base;
    w_12 = translate(w_12, vec3(4.0f, 3.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_12[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 w_13 = w_base;
    w_13 = translate(w_13, vec3(4.0f, 4.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &w_13[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// help render 6 skateboards for each letter
void drawSkateboard(const mat4& baseSkateboard, const GLuint& worldMatrixLocation, char renderMode)
{
    mat4 base_skateboard = translate(baseSkateboard, vec3(0.0f, -0.5f, 0.0f));
    mat4 board = base_skateboard;
    mat4 wheel1 = base_skateboard;
    mat4 wheel2 = base_skateboard;
    mat4 wheel3 = base_skateboard;
    mat4 wheel4 = base_skateboard;

    board = translate(board, vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(3.0f, 0.2f, 7.5f));
    wheel1 = translate(wheel1, vec3(2.0f, -0.2f, 3.0f)) * scale(mat4(1.0), vec3(0.4f, 0.4f, 0.4f));
    wheel2 = translate(wheel2, vec3(0.1f, -0.2f, 3.0f)) * scale(mat4(1.0), vec3(0.4f, 0.4f, 0.4f));
    wheel3 = translate(wheel3, vec3(2.0f, -0.2f, -3.0f)) * scale(mat4(1.0), vec3(0.4f, 0.4f, 0.4f));
    wheel4 = translate(wheel4, vec3(0.1f, -0.2f, -3.0f)) * scale(mat4(1.0), vec3(0.4f, 0.4f, 0.4f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &board[0][0]);
    glDrawArrays(renderMode, 0, 36);
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &wheel1[0][0]);
    glDrawArrays(renderMode, 0, 36);
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &wheel2[0][0]);
    glDrawArrays(renderMode, 0, 36);
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &wheel3[0][0]);
    glDrawArrays(renderMode, 0, 36);
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &wheel4[0][0]);
    glDrawArrays(renderMode, 0, 36);
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version - lab
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024x768 - lab
    GLFWwindow* window = glfwCreateWindow(1024, 768, "MIDTERM - 01", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Key press callback


    // Initialize GLEW - lab
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background - lab
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   

    // Compile and link shaders - lab 
    int shaderProgram = compileAndLinkShaders();
    
    // Enable shader program - lab
    glUseProgram(shaderProgram);

    // Camera parameters for view transform - lab
    vec3 cameraPosition(16.0f, 1.0f, 40.0f);
    vec3 cameraLookAt(16.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters - lab
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable


    // Set projection matrix for shader, this won't change - lab
    mat4 projectionMatrix = glm::perspective(70.0f,         // field of view in degrees
                            800.0f / 600.0f,                // aspect ratio
                            0.01f, 100.0f);                 // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    //taken from lab
    mat4 worldMatrix = mat4(1.0);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    // Set initial view matrix - lab
    mat4 viewMatrix = lookAt(cameraPosition,                 // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp);                      // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Define and upload geometry to the GPU here ...
    int lineVao = createLineVertexBufferObject();
    
    // For frame time - lab
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Enable backface culling and depth test
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //artifact from assignment
    char renderMode = GL_TRIANGLES;
    float fov = 70.9f;

    // Letter base cubes
    mat4 c_base = translate(mat4(1.0f), vec3(0.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 h_base = translate(mat4(1.0f), vec3(5.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 l_base = translate(mat4(1.0f), vec3(10.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 o_base = translate(mat4(1.0f), vec3(15.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 e_base = translate(mat4(1.0f), vec3(20.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 a_base = translate(mat4(1.0f), vec3(25.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 w_base = translate(mat4(1.0f), vec3(30.0f, 1.4f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

    // Skateboard base cubes
    mat4 c_baseBoard = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 h_baseBoard = translate(mat4(1.0f), vec3(5.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 l_baseBoard = translate(mat4(1.0f), vec3(10.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 o_baseBoard = translate(mat4(1.0f), vec3(15.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 e_baseBoard = translate(mat4(1.0f), vec3(20.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 a_baseBoard = translate(mat4(1.0f), vec3(25.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
    mat4 w_baseBoard = translate(mat4(1.0f), vec3(31.0f, 1.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation - lab
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;
        
        // Each frame, reset color of each pixel to glClearColor and Clear Depth Buffer Bit as well - lab
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(lineVao);

        // Draw axis lines
        glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_LINES, 2, 2);
        glDrawArrays(GL_LINES, 4, 2);

        //Drawing floor grid
        mat4 gridWorldMatrix;
        if (showGrid)
        {
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
            for (int i = 0; i <= 100; ++i)
            {
                gridWorldMatrix = translate(mat4(1.0f), vec3(-50.0f, -0.1f, -50.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(100.0f, 1.0f, 1.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
                glDrawArrays(GL_LINES, 6, 2);

                gridWorldMatrix = translate(mat4(1.0f), vec3(-50.0f + i * 1.0f, -0.1f, -50.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 100.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
                glDrawArrays(GL_LINES, 8, 2);
            }
        }

        // Draw ground
        createCubeVertexBufferObject(vec3(1.0f, 1.0f, 1.0f));
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.3f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.1f, 100.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render each letter
        createCubeVertexBufferObject(vec3(1.0f, 1.0f, 0.0f));    // C
        drawC(c_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(1.0f, 0.5f, 0.0f));    // H
        drawH(h_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(1.0f, 1.0f, 0.0f));    // L
        drawL(l_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(0.0f, 1.0f, 0.0f));    // O
        drawO(o_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(0.0f, 0.0f, 1.0f));    // E
        drawE(e_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(1.0f, 0.0f, 1.0f));    // A
        drawA(a_base, worldMatrixLocation, renderMode);

        createCubeVertexBufferObject(vec3(2.0f, 0.5f, 1.0f));    // W
        drawW(w_base, worldMatrixLocation, renderMode);

       
        // Render skateboards
        createCubeVertexBufferObject(vec3(0.0f, 0.0f, 0.0f));
        drawSkateboard(c_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(h_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(l_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(o_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(e_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(a_baseBoard, worldMatrixLocation, renderMode);
        drawSkateboard(w_baseBoard, worldMatrixLocation, renderMode);

        // End frame
        glfwSwapBuffers(window);
        
        // Detect inputs
        glfwPollEvents();
        
        // Handle inputs
        // ESC - exit window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Q - hide grid
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            showGrid = false;
        }

        // Select letter
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
        {
            cameraPosition = vec3(1.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(1.0f, 2.0f, 15.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
        {
            cameraPosition = vec3(6.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(6.0f, 2.0f, 15.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            cameraPosition = vec3(11.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(11.0f, 2.0f, 15.0f);

        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            cameraPosition = vec3(16.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(16.0f, 2.0f, 15.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            cameraPosition = vec3(21.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(21.0f, 2.0f, 15.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            cameraPosition = vec3(26.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(26.0f, 2.0f, 15.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            cameraPosition = vec3(32.0f, 3.0f, 15.0f);
            cameraLookAt = vec3(32.0f, 2.0f, 15.0f);
        }

        
        // Letter rotation
        if (glfwGetKey(window, GLFW_KEY_1) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            c_base = rotate(c_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_1) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            c_base = rotate(c_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_2) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            h_base = rotate(h_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_2) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            h_base = rotate(h_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_3) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            l_base = rotate(l_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_3) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            l_base = rotate(l_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_4) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            o_base = rotate(o_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_4) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            o_base = rotate(o_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_5) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            e_base = rotate(e_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_5) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            e_base = rotate(e_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_6) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            a_base = rotate(a_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_6) && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            a_base = rotate(a_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_7) && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            w_base = rotate(w_base, 0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_7) && glfwGetKey(window, GLFW_KEY_K)== GLFW_PRESS)
        {
            w_base = rotate(w_base, -0.1f, vec3(0.0f, 1.0f, 0.0f));
        }

        //camera reset - lab
        if (glfwGetKey(window, GLFW_KEY_HOME)|| glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            cameraPosition = vec3(16.0f, 1.0f, 40.0f);
            cameraLookAt = vec3(16.0f, 0.0f, -1.0f);
            cameraUp = vec3(0.0f, 1.0f, 0.0f);
        }

        // SHIFT - increase camera speed - lab 
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        // Mouse motion dx and dy - lab
        // Update camera horizontal and verticle angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates - lab
        /*const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;*/

        // Clamp vertical angle to [-85, 85] degrees - lab
       cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        // ASDW camera controls
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera down
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera up
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }

        // Set the view matrix for first and third person cameras - lab
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
        mat4 viewMatrix = mat4(1.0);
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        
        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    }
    
    // Shutdown GLFW - lab
    glfwTerminate();
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
	return 0;
}
