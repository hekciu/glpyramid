#include <iostream>
#include <time.h>
#include <signal.h>

#include <GLFW/glfw3.h>


void error_callback(int error, const char * description) {
    fprintf(stderr, "an error occured: %s\n", description);
}


void terminate(int sig) {
    printf("got signal: %d, terminating...\n");
    glfwTerminate();
    exit(1);
}


void handle_key(GLFWwindow * window, int key, int scancode, int action, int mods) {
    printf("key clicked, terminating\n");
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void render_triangle_2D(GLFWwindow * window) {

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

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);                
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}
