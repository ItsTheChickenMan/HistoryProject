/* date = January 11th 2021 2:04 pm */

#ifndef WINDOW_CALLBACKS_H
#define WINDOW_CALLBACKS_H

#include <GLFW/glfw3.h>

void w_position_changed(GLFWwindow* window, int xpos, int ypos);

void w_size_changed(GLFWwindow* window, int width, int height);

void w_closed(GLFWwindow* window);

void w_refresh(GLFWwindow* window);

void w_focused(GLFWwindow* window, int focused);

void w_iconified(GLFWwindow* window, int iconified);

void w_maximized(GLFWwindow* window, int maximized);

void w_framebuffer_resized(GLFWwindow* window, int width, int height);

void w_content_scale(GLFWwindow* window, float xscale, float yscale);

void w_mouse(GLFWwindow* window, double xpos, double ypos);

void w_key(GLFWwindow* window, int key, int scancode, int action, int mods);

void set_callbacks();

#endif //WINDOW_CALLBACKS_H
