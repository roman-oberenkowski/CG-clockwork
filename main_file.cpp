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
double clockTime=0.0f;
glm::vec3 positioningVar;
int chooseVar=0;
int timeSpeedVar=0;
bool is_box_drawn=true;
bool grabCursor=true;

Model *ourModel;
Shader *mainShader;
Shader *light;
Camera *cam;
bool lightMoving=true;
int positioningMode=1;

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
	int width, height;
	glfwGetWindowSize(window,&width, &height);
	cam->aspectRatio=width/float(height);
    glfwSetCursorPosCallback(window, mouse_callback); 
	glfwSetScrollCallback(window, scroll_callback);  
	//drawing setup
	glClearColor(0.1,0.1,0.25,0.5);
	glEnable(GL_DEPTH_TEST); 
	//glEnable(GL_CULL_FACE);


	//input setup
	glfwSetKeyCallback(window,keyCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	//positioning stuff
	positioningVar=glm::vec3(-8.4f,9.0f,0.85f);
	
	//shader setup
	mainShader = new Shader("shaders/colorTex_v.glsl","shaders/colorTex_f.glsl");
	light = new Shader("shaders/light_v.glsl","shaders/light_f.glsl");

	mainShader->use(); 
	//textures setup
	//stbi_set_flip_vertically_on_load(true);
	//model loading
	ourModel = new Model("clock");
	
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
	//time management
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;  

	//LIGHT-----------
	static float x=1.2f,z=2.0f;
	if(lightMoving){
		x=2*sin(glfwGetTime());
		z=2*cos(glfwGetTime());
	}
	glm::vec3 lightPos(x, 0.5f, z);
	//drawing light
	glm::mat4 light_model_mat(1.0f);
	light->use();
	light_model_mat = glm::mat4(1.0f);
	light_model_mat = glm::translate(light_model_mat, lightPos);
	light_model_mat = glm::scale(light_model_mat, glm::vec3(0.15)); 
	light->setMat4("model",light_model_mat);
	glBindVertexArray(VAO_light);
	glDrawArrays(GL_TRIANGLES,0,36);

	//MODELS--------------
	glm::mat4 model_mat_clock = glm::mat4(1.0f);
    model_mat_clock = glm::scale(model_mat_clock, glm::vec3(2.0f, 2.0f, 2.0f));
	//model_mat_clock = glm::rotate(model_mat_clock,glm::radians(float(glfwGetTime()*100)),glm::vec3(1.0f,0.0f,0.0f));

	mainShader->use();
	mainShader->setVec3("lightPos", lightPos);  
	mainShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	mainShader->setVec3("lightColor",glm::vec3(1.0f,1.0f,1.0f));
	//mainShader->setMat4("model",model_mat_clock); - not needed, becouse drawing each mesh sets model matrix
	mainShader->setVec3("viewPos", cam->cameraPos); 

    ourModel->Draw(*mainShader,model_mat_clock);
	
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

	window = glfwCreateWindow(800, 800, "OpenGL CLOCK RO WG", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

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
		projection = glm::perspective(glm::radians(cam->Zoom), cam->aspectRatio, 0.15f, 100.0f);
		
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
	cam->aspectRatio=float(width)/height;
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_C) 
		lightMoving=!lightMoving;
		if (key==GLFW_KEY_P){
			positioningMode=(positioningMode+1)%4;
			printf("Current Positioning Mode -> %d\n",positioningMode);
		}
		
		if(key==GLFW_KEY_INSERT){chooseVar++;printf("Choice: %d\n",chooseVar);};
		if(key==GLFW_KEY_HOME){chooseVar--;printf("Choice: %d\n",chooseVar);};
		if (key==GLFW_KEY_0)timeSpeedVar=-1;
		if (key==GLFW_KEY_1)timeSpeedVar=0.0;
		if(key==GLFW_KEY_2)timeSpeedVar=1;
		if(key==GLFW_KEY_3)timeSpeedVar=2;
		if(key==GLFW_KEY_4)timeSpeedVar=3;
		if(key==GLFW_KEY_F1)clockTime=0;
		if(key==GLFW_KEY_B)is_box_drawn=!is_box_drawn;
		if(key==GLFW_KEY_END){
			if(grabCursor){
				grabCursor=false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
			}
			else{
				grabCursor=true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				cam->mouseIgnoreCounter=0;  
			}
		}
	
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
	if(grabCursor)cam->mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	if(!positioningMode)
		cam->scroll_callback(window, xoffset,yoffset);
	else{
		
		switch(positioningMode){
			case 1: 
				positioningVar[0]+=yoffset/20.0;
				break;
			case 2:
				positioningVar[1]+=yoffset/2.0;
				break;
			case 3:
				positioningVar[2]+=yoffset/20.0f;
				break;
		}

		printf("PosVar: %f \t%f \t%f\t \n",positioningVar[0],positioningVar[1],positioningVar[2]);
	}
}

void freeOpenGLProgram(GLFWwindow* window) {

}
enum ID {
	id_hour_hand=7,
	id_minute_hand=8,
	id_pendulum_hand=10,
	id_pendulum_sphere=9,
	id_pendulum_grab=0,
	id_gearwheel_minutes=12,
	id_gearwheel_minutes_mini=11,
	id_gearwheel_minutes_support=13,
	id_gearwheel_minutes_support_mini=14,
	id_gearwheel_hours=19,
	id_gearwheel_hours_support=17,
	id_gearwheel_hours_support_mini=18,
	id_gearwheel_seconds=15,
	id_gearwheel_seconds_mini=16,
};

glm::mat4 getModelMatrix(int id){
	glm::mat4 model(1.0f);
	glm::vec3 rotationAxis(1000.0f,0.0f,0.0f);
	if(id==20){
		model=glm::translate(model,glm::vec3(0.0f,0.0f,-0.002f));
		return model;
	}
	switch(id){
		case id_gearwheel_hours:
		rotationAxis=glm::vec3(0.002392f,1.5636f,0.0f);
		break;
		case id_hour_hand:
		case id_minute_hand:
		rotationAxis=glm::vec3(0.005149f,1.5634f,0.0f);
		break;
		case id_gearwheel_minutes_mini:
		case id_gearwheel_minutes:
		rotationAxis=glm::vec3(0.002252f,1.5639f,0.0f);
		break;
		case id_pendulum_hand:
		case id_pendulum_sphere:
		case id_pendulum_grab:
		rotationAxis=glm::vec3(0.000867f,1.7279f,0.0f);
		break;
		case id_gearwheel_hours_support:
		case id_gearwheel_hours_support_mini:
		rotationAxis=glm::vec3(0.093492f,1.4966f,0.0f);
		break;
		case id_gearwheel_seconds:
		case id_gearwheel_seconds_mini:
		rotationAxis=glm::vec3(0.11684f,1.6188f,0.0f);
		break;
		case id_gearwheel_minutes_support:
		case id_gearwheel_minutes_support_mini:
		rotationAxis=glm::vec3(0.13859f,1.4259f,0.0f);
		break;
	}
	if(rotationAxis.x>100)
		return model;
	else{
		const float pendulum_t=12.0f;
		const float pendulum_phase=-7.55;
		const float pendulum_grab_shift=14.0f;
		const float gearwheel_phase=-6.6999f;
		clockTime+=float(deltaTime*2*pow(10,timeSpeedVar));
		
		float degrees=0;
		float preciseTime;
		float coarseTime;
		preciseTime=fmod(clockTime,12);
		coarseTime=clockTime-preciseTime;
		float degrees_gearwheel_seconds=-( coarseTime+2.0*fmin(preciseTime*2.0,6)+gearwheel_phase);

		switch(id){
			case id_gearwheel_seconds:
			case id_gearwheel_seconds_mini:

			degrees=degrees_gearwheel_seconds;
			break;
			//8:64
			case id_gearwheel_minutes_support:
			case id_gearwheel_minutes_support_mini:
			degrees=-degrees_gearwheel_seconds/8;
			break;
			//8:64
			case id_gearwheel_minutes:
			case id_gearwheel_minutes_mini:
			case id_minute_hand:
			degrees=degrees_gearwheel_seconds/8/8;
			if(id_minute_hand)degrees+=90;
			break;
			//10:30
			case id_gearwheel_hours_support:
			case id_gearwheel_hours_support_mini:
			degrees=-degrees_gearwheel_seconds/8/8/3;
			break;
			//8:30
			case id_gearwheel_hours:
			case id_hour_hand:
			degrees=degrees_gearwheel_seconds/8/3/30;
			break;
			case id_pendulum_hand:
			case id_pendulum_sphere:
			degrees=-9.0f*cos(PI*clockTime/pendulum_t+pendulum_phase);
			break;
			case id_pendulum_grab:
			degrees=-9.0f*cos(PI*clockTime/pendulum_t+pendulum_phase)+pendulum_grab_shift;

		}
		model = glm::translate(model, rotationAxis);
		model = glm::rotate(model, glm::radians(degrees), glm::vec3(0.0f,0.0f,1.0f));
		if(id==id_pendulum_grab)model= glm::scale(model,glm::vec3(float(0.9f)));
		model = glm::translate(model, -rotationAxis);
		return model;
	}
}