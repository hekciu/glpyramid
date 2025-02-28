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


struct Point {
    float x;
    float y;
    float z;
};


struct Color {
    float r;
    float g;
    float b;
};


struct Triangle {
    Point p1;
    Color c1;

    Point p2;
    Color c2;

    Point p3;
    Color c3;
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


void initialize_data(std::vector<Triangle> & triangles, uint32_t & VBO) {
    Triangle t1, t2, t3, t4;

    t1.p1 = { 0.0f, 0.5f, 0.0f };
    t1.p2 = { -0.5f, -0.5f, 0.5f };
    t1.p3 = { 0.5f, -0.5f, 0.5f };
    t1.c1 = { 1.0f, 0.0f, 0.0f };
    t1.c2 = { 1.0f, 0.0f, 0.0f };
    t1.c3 = { 1.0f, 0.0f, 0.0f };

    t2.p1 = { 0.0f, 0.5f, 0.0f };
    t2.p2 = { -0.5f, -0.5f, 0.5f };
    t2.p3 = { -0.5f, -0.5f, -0.5f };
    t2.c1 = { 0.0f, 1.0f, 0.0f };
    t2.c2 = { 0.0f, 1.0f, 0.0f };
    t2.c3 = { 0.0f, 1.0f, 0.0f };

    t3.p1 = { 0.0f, 0.5f, 0.0f };
    t3.p2 = { 0.5f, -0.5f, -0.5f };
    t3.p3 = { -0.5f, -0.5f, -0.5f };
    t3.c1 = { 0.0f, 0.0f, 1.0f };
    t3.c2 = { 0.0f, 0.0f, 1.0f };
    t3.c3 = { 0.0f, 0.0f, 1.0f };

    t4.p1 = { 0.0f, 0.5f, 0.0f };
    t4.p2 = { 0.5f, -0.5f, -0.5f };
    t4.p3 = { 0.5f, -0.5f, 0.5f };
    t4.c1 = { 1.0f, 1.0f, 0.0f };
    t4.c2 = { 1.0f, 1.0f, 0.0f };
    t4.c3 = { 1.0f, 1.0f, 0.0f };

    triangles.push_back(t1);
    triangles.push_back(t2);
    triangles.push_back(t3);
    triangles.push_back(t4);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    size_t trianglesSize = triangles.size() * sizeof(Triangle);

    glBufferData(GL_ARRAY_BUFFER, trianglesSize, triangles.data(), GL_STATIC_DRAW);
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


void animate(std::vector<Triangle> & triangles, int & animationIndex) {
    const float PI = 3.14f;

    Point * triangle1_1 = &triangles[0].p2;
    Point * triangle1_2 = &triangles[0].p3;

    Point * triangle2_1 = &triangles[1].p2;
    Point * triangle2_2 = &triangles[1].p3;

    Point * triangle3_1 = &triangles[2].p2;
    Point * triangle3_2 = &triangles[2].p3;

    Point * triangle4_1 = &triangles[3].p2;
    Point * triangle4_2 = &triangles[3].p3;

    animationIndex = getNextIndex(animationIndex);

    triangle1_1->x = std::sin(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle1_1->z = std::cos(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle1_2->x = std::sin(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle1_2->z = std::cos(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;

    triangle2_1->x = std::sin(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle2_1->z = std::cos(2.0f * PI * ((animationIndex + 45)/(float)360)) * 0.5f;
    triangle2_2->x = std::sin(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle2_2->z = std::cos(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;

    triangle3_1->x = std::sin(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle3_1->z = std::cos(2.0f * PI * ((animationIndex + 135)/(float)360)) * 0.5f;
    triangle3_2->x = std::sin(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle3_2->z = std::cos(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;

    triangle4_1->x = std::sin(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle4_1->z = std::cos(2.0f * PI * ((animationIndex - 135)/(float)360)) * 0.5f;
    triangle4_2->x = std::sin(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
    triangle4_2->z = std::cos(2.0f * PI * ((animationIndex - 45)/(float)360)) * 0.5f;
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
        printf("Vertex shader compilation failed, info: %s\n", infoLog);
        return 1;
    }

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceRaw, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if (!compiledSuccessfully) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed, info: %s\n", infoLog);
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


    std::vector<Triangle> triangles = {};

    uint32_t VBO;

    initialize_data(triangles, VBO); 

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point) + sizeof(Color), (void *)0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point) + sizeof(Color), (void *)sizeof(Point));


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    int animationIndex = 0; 

    glUseProgram(shaderProgram);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 5.0f, 1.0f, 1.0f);

        animate(triangles, animationIndex);

        glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 12);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}
