#include <iostream>
#include <time.h>
#include <signal.h>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>


#define VERTEX_SHADER_FILE_PATH "vertex_shader.vert"
#define FRAGMENT_SHADER_FILE_PATH "fragment_shader.frag"


struct Vertex {
    float position[3];
};


void error_callback(int error, const char * description) {
    fprintf(stderr, "an error occured: %s\n", description);
};


void terminate(int sig) {
    printf("got signal: %d, terminating...\n");
    glfwTerminate();
    exit(1);
};


void handle_key(GLFWwindow * window, int key, int scancode, int action, int mods) {
    printf("key clicked, terminating...\n");
    glfwSetWindowShouldClose(window, GLFW_TRUE);
};


void initialize_vertices(std::vector<Vertex> & vertices, uint32_t & VBO) {
    // 1
    vertices.push_back({
        { 0.0f, 0.5f, 0.0f }   
    });

    vertices.push_back({
        { -0.5f, -0.5f, 0.5f }
    });

    vertices.push_back({
        { 0.5f, -0.5f, 0.5f }   
    });

    // 2
    vertices.push_back({
        { 0.0f, 0.5f, 0.0f }   
    });

    vertices.push_back({
        { -0.5f, -0.5f, 0.5f }
    });

    vertices.push_back({
        { -0.5f, -0.5f, -0.5f }
    });

    // 3
    vertices.push_back({
        { 0.0f, 0.5f, 0.0f }   
    });

    vertices.push_back({
        { 0.5f, -0.5f, -0.5f }   
    });

    vertices.push_back({
        { -0.5f, -0.5f, -0.5f }
    });

    // 4
    vertices.push_back({
        { 0.0f, 0.5f, 0.0f }   
    });

    vertices.push_back({
        { 0.5f, -0.5f, -0.5f }
    });

    vertices.push_back({
        { 0.5f, -0.5f, 0.5f }
    });


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    Vertex * rawVertices = vertices.data();
    size_t verticesSize = vertices.size() * sizeof(Vertex);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, rawVertices, GL_STATIC_DRAW);
};


std::string get_shader(const char * fileName) {
    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in);
    std::string content = "";

    if(!ifs.is_open()) {
        printf("failed to open shader file :( path: %s\n", fileName);
        return content;
    }

    for(std::string line; std::getline(ifs, line);) {
        content += line;
        content.push_back('\n');
    }

    ifs.close();

    return content;
}


float getNextIndex(int index) {
    const int max = 360;
    const int min = 0;

    int next = index + 1;

    if (next == max) {
        return min;
    } 

    return (float)next;
}


void animate(std::vector<Vertex> & vertices, int & animationIndex) {
    const float PI = 3.14f;

    Vertex * triangle1_1 = &vertices[1];
    Vertex * triangle1_2 = &vertices[2];

    Vertex * triangle2_1 = &vertices[4];
    Vertex * triangle2_2 = &vertices[5];

    Vertex * triangle3_1 = &vertices[7];
    Vertex * triangle3_2 = &vertices[8];

    Vertex * triangle4_1 = &vertices[10];
    Vertex * triangle4_2 = &vertices[11];

    animationIndex = getNextIndex(animationIndex);

    triangle1_1->position[0] = std::sin(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle1_1->position[2] = std::cos(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle1_2->position[0] = std::sin(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle1_2->position[2] = std::cos(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;

    triangle2_1->position[0] = std::sin(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle2_1->position[2] = std::cos(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle2_2->position[0] = std::sin(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle2_2->position[2] = std::cos(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;

    triangle3_1->position[0] = std::sin(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle3_1->position[2] = std::cos(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle3_2->position[0] = std::sin(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle3_2->position[2] = std::cos(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;

    triangle4_1->position[0] = std::sin(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle4_1->position[2] = std::cos(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle4_2->position[0] = std::sin(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle4_2->position[2] = std::cos(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;

    printf("blue triangle z-indeces: %f, %f\n", triangle1_1->position[2], triangle1_2->position[2]);
    printf("red triangle z-indeces: %f, %f\n", triangle3_1->position[2], triangle3_2->position[2]);
}


int main(void) {
    if (glfwInit() == GLFW_FALSE) {
        printf("glfw initialization failed :<\n");
        exit(1);
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow * window = glfwCreateWindow(640, 480, "hello window", NULL, NULL);

    if (!window) {
        printf("window initialization failed :(\n");
        exit(1);
    }

    signal(SIGINT, terminate);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, handle_key);

    std::vector<Vertex> vertices = {};

    uint32_t VBO;

    initialize_vertices(vertices, VBO); 

    std::string vertexShaderSource = get_shader(VERTEX_SHADER_FILE_PATH);
    const char * vertexShaderSourceRaw = vertexShaderSource.c_str();

    std::string fragmentShaderSource = get_shader(FRAGMENT_SHADER_FILE_PATH);
    const char * fragmentShaderSourceRaw = fragmentShaderSource.c_str();

    int compiledSuccessfully;
    char infoLog[512];

    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceRaw, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if (!compiledSuccessfully) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Shader compilation failed, info: %s\n", infoLog);
        return 1;
    }

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceRaw, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if (!compiledSuccessfully) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Shader compilation failed, info: %s\n", infoLog);
        return 1;
    }


    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compiledSuccessfully);
    if (!compiledSuccessfully) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Program linking failed, info: %s\n", infoLog);
        return 1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    int animationIndex = 0; 

    GLint uniform;
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 5.0f, 1.0f, 1.0f);

        animate(vertices, animationIndex);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

        uniform = glGetUniformLocation(shaderProgram, "triangleColor");

        glUniform3f(uniform, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}
