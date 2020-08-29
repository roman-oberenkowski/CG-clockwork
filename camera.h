#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>


class Camera{
public:
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  10.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float yaw=270;
    float pitch=0;
    float lastX;
    float lastY;
    bool firstMouse=false;
    float Zoom=45.0;
    
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        bool debug_mouse=false;
        static int mouseIgnoreCounter=0;
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            yaw=270.0;
            pitch=270.0;  
        } 
    
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        if(mouseIgnoreCounter>5){
            yaw   += xoffset;
            pitch += yoffset;
            //if(debug_mouse)printf("[MouseDebug] Added %f, %f\n",xoffset,yoffset);
        }
        else{
            mouseIgnoreCounter++;
            //if(debug_mouse)printf("[MouseDebug] Ignored\n");
        }
        firstMouse = false;
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }  
    
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }
    
    void processCameraInput(GLFWwindow* window,float deltaTime){
        float cameraSpeed = 2.5f * deltaTime;
	    bool enable_fps_camera=true;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            if(enable_fps_camera) 
                cameraPos += cameraSpeed * glm::vec3(cameraFront[0],0.0,cameraFront[2]);
            else 
                cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            if(enable_fps_camera)
                cameraPos -= cameraSpeed * glm::vec3(cameraFront[0],0.0,cameraFront[2]);
            else 
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraUp * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraPos += -cameraUp * cameraSpeed;
    }

};


#endif