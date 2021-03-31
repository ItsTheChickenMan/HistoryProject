#include "poor/window_callbacks.h"
#include "poor/graphics.h"
#include "poor/camera.h"
// GLFW window callbacks:
// Fill these out with what you want to do.
// Write callbacks in modules like poor/graphics/physics/networking if you need to set a callback for their respective module, then call the function in their w_callback function in here.


// TODO: Actually fill these out and set them via their respective glfwSetCallback functions.

void w_position_changed(GLFWwindow* window, int xpos, int ypos)
{
    // NOTE: When the window gets dragged to a new location.
}

void w_size_changed(GLFWwindow* window, int width, int height)
{
    // NOTE: When the window gets resized.
}

void w_closed(GLFWwindow* window)
{
    // NOTE: When the window X button gets pressed.
}

void w_refresh(GLFWwindow* window)
{
    // NOTE: What does this do???
}

void w_focused(GLFWwindow* window, int focused)
{
    // NOTE: When the window has focus.
}

void w_iconified(GLFWwindow* window, int iconified)
{
    // NOTE: What does this do???
}

void w_maximized(GLFWwindow* window, int maximized)
{
    // NOTE: When the windows maximize window gets pressed
		graphics_maximize_callback(maximized);
}

void w_framebuffer_resized(GLFWwindow* window, int width, int height)
{
    graphics_framebuffer_callback(width, height);
}

void w_content_scale(GLFWwindow* window, float xscale, float yscale)
{
    // NOTE: What does this do???
    // https://www.glfw.org/docs/latest/window_guide.html#window_scale
    
}

void w_mouse(GLFWwindow* window, double xpos, double ypos)
{
    graphics_mouse_callback(xpos, ypos);
    camera_mouse_callback(xpos, ypos);
}

void w_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    graphics_key_callback(key, scancode, action, mods);
    camera_key_callback(key, scancode, action, mods);
}

void w_mouse_pressed(GLFWwindow* window, int button, int action, int mods){
	camera_mouse_button_callback(button, action, mods);
}

void set_callbacks()
{
    extern GLFWwindow* window;
    
    glfwSetFramebufferSizeCallback(window, w_framebuffer_resized);
    
    glfwSetCursorPosCallback(window, w_mouse);
    
    glfwSetKeyCallback(window, w_key);
		
		glfwSetWindowMaximizeCallback(window, w_maximized);
    
		glfwSetMouseButtonCallback(window, w_mouse_pressed);
}