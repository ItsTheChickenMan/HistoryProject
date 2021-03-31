#include "poor/camera.h"
#include "poor/types.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

bool show_cursor = false;
float last_x = 400, last_y = 300;
bool first_mouse = true;

Vector3 getDirection(){
	extern Camera camera;
	
	return createVector3(camera.forward.x, camera.forward.y, camera.forward.z);
}

void init_camera(Camera* camera, glm::vec3 pos)
{
    *camera = {};
    
    camera->position = pos;
    
    glm::vec3 up = glm::vec3(0, 1.0f, 0);
    
		camera->direction = glm::vec3(0, 0, 0);
		
		camera->yaw = 180.f;
		camera->pitch = 0;
    
		camera->direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
		camera->direction.y = sin(glm::radians(camera->pitch));
		camera->direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

    glm::vec3 cam_right = glm::normalize(glm::cross(up, camera->direction));
    camera->up = glm::cross(camera->direction, cam_right);
    camera->forward = glm::normalize(camera->direction);
}

void update_camera(Camera* camera, u32 shader)
{
    extern GLFWwindow* window;
    
    camera->view = glm::lookAt(camera->position,
                               camera->position + camera->forward,
                               camera->up);
    
    camera->projection = glm::mat4(1.0f);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    camera->projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
    
    u32 view_loc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera->view));
    
    u32 projection_loc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(camera->projection));
    
    u32 camera_pos_loc = glGetUniformLocation(shader, "camera_pos");
    glUniform3fv(camera_pos_loc, 1, glm::value_ptr(camera->position));
}


void camera_mouse_callback(double xpos, double ypos)
{
    // TODO: Boys this right here is a quick hack to get the camera for testing, we will want to somehow figure out how to get any camera* like the other camera functions.
    extern Camera camera;
    if (!show_cursor)
    {
        if (first_mouse)
        {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }
        
        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos;
        
        last_x = xpos;
        last_y = ypos;
        
        
        const float sensitivity = 0.07f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        camera.yaw += xoffset;
        camera.pitch += yoffset;
        
        // Clamp values so camera doesnt flip.
        if (camera.pitch > 89.f)
            camera.pitch = 89.f;
        if (camera.pitch < -89.f)
            camera.pitch = -89.f;
        
        
        camera.direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        camera.direction.y = sin(glm::radians(camera.pitch));
        camera.direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        
        camera.forward = glm::normalize(camera.direction);
    }
}

void camera_key_callback(int key, int scancode, int action, int mods)
{
    extern GLFWwindow* window;
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_GRAVE_ACCENT) && action == GLFW_PRESS)
    {
        show_cursor = !show_cursor;
        if (show_cursor)
        {
            // Non-Hidden and free cursor
            first_mouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            
            // Hidden and locked to center cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void camera_mouse_button_callback(int button, int action, int mods){
	extern GLFWwindow* window;
	
	if(action == GLFW_PRESS){
		show_cursor = false;
		first_mouse = true;
		
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}