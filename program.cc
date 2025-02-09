#include <iostream>
#include <time.h>
#include <signal.h>
#include <vector>
#include <fstream>
#include <string>

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
    vertices.push_back({
        { -0.5f, -0.5f, 0.0f }
    });

    vertices.push_back({
        { 0.5f, -0.5f, 0.0f }   
    });

    vertices.push_back({
        { 0.0f, 0.5f, 0.0f }   
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


void render_stuff() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        render_stuff();

        glfwSwapBuffers(window);                
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}
