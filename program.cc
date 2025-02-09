#include <iostream>
#include <time.h>
#include <signal.h>
#include <vector>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>


#define VERTEX_SHADER_FILE_PATH "vertex_shader.vert"


struct Vertex {
    float position[3];
    float color[3];
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
        { -0.5f, -0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f }     
    });

    vertices.push_back({
        { -0.5f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f }     
    });

    vertices.push_back({
        { 0.5f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f }     
    });

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    Vertex * rawVertices = vertices.data();
    glBufferData(GL_ARRAY_BUFFER, sizeof(rawVertices), rawVertices, GL_STATIC_DRAW);
};


std::string get_vertex_shader() {
    std::ifstream ifs;
    ifs.open(VERTEX_SHADER_FILE_PATH, std::ifstream::in);
    std::string content = "";

    if(!ifs.is_open()) {
        printf("failed to open vertex shader file :(\n");
        return content;
    }

    for(std::string line; std::getline(ifs, line);) {
        content += line;
        content.push_back('\n');
    }

    ifs.close();

    return content;
}


void render_stuff(GLFWwindow * window, const std::vector<Vertex> & vertices) {

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

    std::string vertexShaderSource = get_vertex_shader();
    const char * vertexShaderSourceRaw = vertexShaderSource.c_str();

    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceRaw, NULL);
    glCompileShader(vertexShader);

    int compiledSuccessfully;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if (!compiledSuccessfully) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Shader compilation failed, info: %s\n", infoLog);
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        render_stuff(window, vertices);
        glfwSwapBuffers(window);                
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}
