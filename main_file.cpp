#define GLM_FORCE_RADIANS

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include <iostream>
#include "shader.h"
#include "model.h"
#include "stb_image.h"
#include <unistd.h>
#include "camera.h"


#include "cube_vertices.h"

using namespace std;
unsigned int VAO_cube;
unsigned int VAO_light;
unsigned int shaderProgram;
unsigned int shaderProgram2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Model *ourModel;
Shader *mainShader;
Shader *light;
Camera *cam;
bool lightMoving=true;

//definitions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods);
void processInput(GLFWwindow *window);
void error_callback(int error, const char* description);
void freeOpenGLProgram(GLFWwindow* window);

//init
void initOpenGLProgram(GLFWwindow* window) {
	cam = new Camera();
    glfwSetCursorPosCallback(window, mouse_callback); 
	glfwSetScrollCallback(window, scroll_callback);  
	//drawing setup
	glClearColor(0.1,0.1,0.25,0.5);
	glEnable(GL_DEPTH_TEST); 
	//input setup
	glfwSetKeyCallback(window,keyCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	
	
	//shader setup
	mainShader = new Shader("shaders/colorTex_v.glsl","shaders/colorTex_f.glsl");
	light = new Shader("shaders/light_v.glsl","shaders/light_f.glsl");

	mainShader->use(); 
	//textures setup
	stbi_set_flip_vertically_on_load(true);
	//model loading
	ourModel = new Model("models/gear2/gear2.obj");
	
	glGenVertexArrays(1,&VAO_cube);
	glBindVertexArray(VAO_cube);

	unsigned int VBO_cube;
	glGenBuffers(1,&VBO_cube);
	glBindBuffer(GL_ARRAY_BUFFER,VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices),cube_vertices,GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,sizeof(float)*6,(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,sizeof(float)*6,(void*)(3*sizeof(float)));

	glGenVertexArrays(1,&VAO_light);
	glBindVertexArray(VAO_light);

	glBindBuffer(GL_ARRAY_BUFFER,VBO_cube);
	
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,sizeof(float)*6,(void*)0);
	glEnableVertexAttribArray(0);
}

//Drawing procedure
void drawScene(GLFWwindow* window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;  

	glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
	
	mainShader->use();
	mainShader->setMat4("model", model);
    ourModel->Draw(*mainShader);

	static float x=1.2f,z=2.0f;
	if(lightMoving){
		x=2*sin(glfwGetTime());
		z=2*cos(glfwGetTime());
	}
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	glm::vec3 lightPos(x, 0.5f, z);

	mainShader->setVec3("lightPos", lightPos);  

	model = glm::mat4(1.0f);
	
	mainShader->use();
	mainShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	mainShader->setVec3("lightColor",glm::vec3(1.0f,1.0f,1.0f));
	mainShader->setMat4("model",model);
	mainShader->setVec3("viewPos", cam->cameraPos); 
	glBindVertexArray(VAO_cube);
	//glDrawArrays(GL_TRIANGLES,0,36);
	
	light->use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); 
	light->setMat4("model",model);
	glBindVertexArray(VAO_light);
	glDrawArrays(GL_TRIANGLES,0,36);
	
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window
	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE); 
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(500, 500, "OpenGL CLOCK RO WG", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	
	initOpenGLProgram(window); //Call initialization procedure

	//Main application loop
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{		
		//prepare - need  to set view and projection matrix for each shader.
		glm::mat4 view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam->Zoom), 800.0f / 800.0f, 0.1f, 100.0f);
		
		mainShader->use();
		mainShader->setMat4("projection",projection);
		mainShader->setMat4("view",view);
		
		light->use();
		light->setMat4("projection",projection);
		light->setMat4("view",view);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(window);
		glfwSwapBuffers(window);
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
		processInput(window);
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_C) 
		lightMoving=!lightMoving;
	}
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	
	//used WASD,Space,LShift
	cam->processCameraInput(window,deltaTime);
}

void error_callback(int error, const char* description) {
	printf("[ErrorCallback]");
	fputs(description, stderr);
}

//redirection to cam class
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	cam->mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	cam->scroll_callback(window, xoffset,yoffset);
}

void freeOpenGLProgram(GLFWwindow* window) {

}