// basic graphics manager

#include "poor/graphics.h"

#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <ft2build.h>
//#include FT_FREETYPE_H

#include <poor/types.h>
#include <poor/io_util.h>
#include <poor/shader.h>

// Put geometry in header for now so it doesnt clog up graphics.cpp
#include <poor/shapes.h>

// Phoenix's entity management functions (wow, so small!  I wonder how he was able to write such little code, and yet do so much!  MAYBE IT'S CAUSE I USED TEMPLATES ETHAN
static EntityManager<Render_Data> renderManager;

static int g_maximized; 

// lighting temp
std::vector<Light> lights;
std::vector<PointLight> pointLights;
std::vector<Spotlight> spotlights;

void pushLight(glm::vec3 position){
	Light light;
	
	light.position = position;
	light.ambient = glm::vec3(0.2, 0.2, 0.2);
	light.diffuse = glm::vec3(0.9, 0.9, 0.9);
	light.specular = glm::vec3(0, 0, 0);
	
	lights.push_back(light);
}

void pushPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
	PointLight light;
	
	light.position = position;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;
	
	pointLights.push_back(light);
}

void pushSpotlight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
	Spotlight light;
	
	light.position = position;
	light.direction = direction;
	light.cutOff = cutOff;
	light.outerCutOff = outerCutOff;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;
	
	spotlights.push_back(light);
}

static u32 phong; //temporarily moving this up here so I can access it below

// no static because we need to access this in other places through the extern keyword.
GLFWwindow* window; //temporarily moved for access

// if the window is minimized
static bool windowSleeping = false;

// this is sort of a temp, but it would be a good utility function to have...whether or not to keep this in the long run is up to you
bool getKeyPressed(int key){
	return glfwGetKey(window, key) == GLFW_PRESS;
}
// end of temporaries

// return a reference to the render manager
EntityManager<Render_Data> *getRenderManager(){
	return &renderManager;
};

// create render_data function, use this externally because it automatically initializes render_data, which can't be done outside of graphics.cpp
// for now, it automatically uses the cube model, but once model management is implemented we can change this
Render_Data createRenderData(char* texturePath){
	Render_Data data;
	
	// fetch asset
	Texture_Info *texture = getAssetByPath(texturePath);
	
	// if the asset is invalid, load the texture
	if(texture == getNullAsset()){
		texture = loadAsset(texturePath, texturePath, rand()); // not the best way to manage assets, but ok for now
	}

	
	init_render_data(&data, texture, (Mipmap_Info){GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR}, cube, sizeof(cube));
	
	return data;
}

Render_Data createRenderData(Texture_Info *texture){
	Render_Data data;
	
	// I don't feel like creating a nullRenderData type so for now just print an error if the texture is null
	if(texture == getNullAsset()){
		printf("Warning: texture provided was null, using default\n");
		texture = getAssetByName("default");
		
		// this is dumb, whatever
		if(texture == getNullAsset()){
			printf("Error: no default texture available\n");
		}
	}
	
	init_render_data(&data, texture, (Mipmap_Info){GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR}, cube, sizeof(cube));
	
	return data;
}

// renders all physics entities 
void graphicsRenderEntities(){
	for(std::vector<Render_Data>::iterator i = renderManager.entities.begin(); i != renderManager.entities.end(); i++){		
		draw_render_data( &(*i), phong);
	}
}
// end of phoenix's stuff


void init_render_data(Render_Data* render_data, 
                      Texture_Info *texture_info, 
                      Mipmap_Info mipmap_info, 
                      float* vertex_data, 
                      u32 vertex_data_size)
{
    // First, Do Texture stuff.
    glGenTextures(1, &render_data->texture);
    glBindTexture(GL_TEXTURE_2D, render_data->texture);
    
    // TODO: this is a little bit not smart but i dont carree
    if (mipmap_info.wrap_s == 0)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mipmap_info.wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mipmap_info.wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_info.min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_info.max_filter);
    } else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    if (texture_info->channel_count == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_info->width, texture_info->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_info->data);
    }
    else if (texture_info->channel_count == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_info->width, texture_info->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_info->data);
    }
    else
        printf("Texture Channel count invalid.\n");
    
    if (mipmap_info.wrap_s == 0)
        glGenerateMipmap(GL_TEXTURE_2D);
    
    // Then, do vertex data stuff
    
    glGenVertexArrays(1, &render_data->vao);
    glBindVertexArray(render_data->vao);
    
    glGenBuffers(1, &render_data->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, render_data->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);
    
    // TODO: Custom vertex attributes?
    render_data->vertex_count = (vertex_data_size / sizeof(*vertex_data)) / 8;
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                          8 * sizeof(float), (void*)0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
                          8 * sizeof(float), (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(sizeof(float) * 5));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void update_render_data(Render_Data* data, Transform trans)
{
    
    // Update Model matrix
    data->model = glm::mat4(1.0f);
    
    data->model = glm::translate(data->model, trans.position);
    
    data->model = glm::rotate(data->model, glm::radians(trans.rotation.x), glm::vec3(1.0f, 0, 0));
    data->model = glm::rotate(data->model, glm::radians(trans.rotation.y), glm::vec3(0, 1.0f, 0));
    data->model = glm::rotate(data->model, glm::radians(trans.rotation.z), glm::vec3(0, 0, 1.0f));
    
    data->model = glm::scale(data->model, trans.scale);
    
    
}

void draw_render_data(Render_Data* data, u32 shader)
{
    u32 model_loc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(data->model));
    
    glBindTexture(GL_TEXTURE_2D, data->texture);
    glUseProgram(shader);
    glBindVertexArray(data->vao);
    
    glDrawArrays(GL_TRIANGLES, 0, data->vertex_count);
}

Camera camera;

// TODO: This is temporary, we need to store this data in a way that every module can get this data. (phoenix can decide this)
// Delta Time
float dt = 0.0f;
float last_frame = 0.0f;

struct Character {
    u32 texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    u32 advance;
};

int graphicsInit()
{
    // First, we initialize all of GLFW related stuff and the window.
    if (!glfwInit())
    {
        printf("You are a terrible person\n");
        return -1;
    }
    
    
    window = glfwCreateWindow(800, 600, "Virtual Museum", 0, 0);
    if (!window)
    { 
        glfwTerminate();
        return -1;
    }
    
    // This makes the window's openGL context on the current thread.
    glfwMakeContextCurrent(window);
    
    // This loads openGL functions into the program.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("OpenGL didnt do the thing.\n");
        return -1;
    }
    
    // TODO: Should this be in here?
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    
    // TODO: Asset Managment
    /*createBrick();
    stbi_set_flip_vertically_on_load(true);
    
    s32 width, height, channels;
    u8* brick = stbi_load("./res/texture/brick.png", &width, &height, &channels, 0);
    if (!brick)
        printf("Error, stbi_load didnt do something, you probably are not cool.\n");
    
    
    
    init_render_data(&thing1,
                     brickTexture,
                     (Mipmap_Info){GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST},
                     cube, sizeof(cube));
    
    stbi_set_flip_vertically_on_load(false);
    
    u8* rope = stbi_load("./res/texture/rope.jpg", &width, &height, &channels, 0);
    if (!rope)
        printf("Error, stbi_load didnt do something, you probably are not cool.\n");
    
    
    
    init_render_data(&thing2,
                     (Texture_Info){width, height, channels, rope},
                     (Mipmap_Info){GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR},
                     cube, sizeof(cube));*/
    // NOTE: Make sure this isnt free'd before use.
    //stbi_image_free(brick);
    //stbi_image_free(rope);
    
    {
        const char* vss = read_entire_file("./res/shader/basic.vs");
        const char* fss = read_entire_file("./res/shader/basic.fs");
        phong = generate_shader_program(vss, fss);
    }
    
    // Camera
    init_camera(&camera, {0, 0, 3.0f});
    glClearColor(.3f, 0, 0, 1.0f);
    // Z Buffer sorting (So openGL draws triangles in the right order);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		
		glfwSwapInterval(1);
		
    return 0;
}

// ETHAN: I temporarily added a parameter to this function so that it knows whether or not to use client movement
void graphicsUpdate()
{
		glfwPollEvents();
		
		// don't do anything if the window's minimized
		if(windowSleeping) return;
		
		// TODO: Move this to a better place.
    float current_frame = glfwGetTime();
    dt = current_frame - last_frame;
    last_frame = current_frame;
    
    static glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    static glm::vec3 light_pos = glm::vec3(1.0f, 1.5f, 2.0f);
    
    update_camera(&camera, phong);
    
    
    // RENDER:
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    u32 specular_loc = glGetUniformLocation(phong, "material.specular");
    glUniform3fv(specular_loc, 1, glm::value_ptr(glm::vec3(.0f, .0f, .0f)));
    
    u32 shine_loc = glGetUniformLocation(phong, "material.shine");
    glUniform1f(shine_loc, 32.0f);
    
    /*u32 light_pos_loc = glGetUniformLocation(phong, "light.position");
    glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_pos));
    
    u32 l_ambient_loc = glGetUniformLocation(phong, "light.ambient");
    glUniform3fv(l_ambient_loc, 1, glm::value_ptr(glm::vec3(.2f, .2f, 0.2f)));
    
    u32 l_diffuse_loc = glGetUniformLocation(phong, "light.diffuse");
    glUniform3fv(l_diffuse_loc, 1, glm::value_ptr(glm::vec3(.9f, .9f, .9f)));
    
    u32 l_specular_loc = glGetUniformLocation(phong, "light.specular");
    glUniform3fv(l_specular_loc, 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
    */
		
		
		// render lights TODO: improve
		for(int i = 0; i < lights.size(); i++){
			char buffer[64];
			u32 tempLoc;
			
			sprintf(buffer, "lights[%d].position", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(lights[i].position));
			
			sprintf(buffer, "lights[%d].ambient", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(lights[i].ambient));
			
			sprintf(buffer, "lights[%d].diffuse", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(lights[i].diffuse));

			sprintf(buffer, "lights[%d].specular", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(lights[i].specular));
		}
		
		/*u32 num_point_loc = glGetUniformLocation(phong, "numPointLights");
		glUniform1i(num_point_loc, pointLights.size());
		for(int i = 0; i < pointLights.size(); i++){
			char buffer[64];
			u32 tempLoc;
			
			sprintf(buffer, "pointLights[%i].position", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(pointLights[i].position));

			sprintf(buffer, "pointLights[%i].constant", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, pointLights[i].constant);

			sprintf(buffer, "pointLights[%i].linear", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, pointLights[i].linear);

			sprintf(buffer, "pointLights[%i].quadratic", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, pointLights[i].quadratic);

			sprintf(buffer, "pointLights[%i].diffuse", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(pointLights[i].diffuse));

			sprintf(buffer, "pointLights[%i].specular", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(pointLights[i].specular));
		}*/
		
		/*u32 num_spot_loc = glGetUniformLocation(phong, "numSpotights");
		glUniform1i(spot_loc, spotlight.size());
		for(int i = 0; i < spotlights.size(); i++){
			char buffer[64];
			u32 tempLoc;
			
			sprintf(buffer, "spotlights[%i].position", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, 1, glm::value_ptr(spotlights[i].position));

			sprintf(buffer, "spotlights[%i].constant", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, spotlights[i].constant);

			sprintf(buffer, "spotlights[%i].linear", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, spotlights[i].linear);

			sprintf(buffer, "spotlights[%i].quadratic", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, spotlights[i].quadratic);

			sprintf(buffer, "spotlights[%i].diffuse", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, buffer, glm::value_ptr(spotlights[i].diffuse));

			sprintf(buffer, "spotlights[%i].specular", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform3fv(tempLoc, buffer, glm::value_ptr(spotlights[i].specular));
			
			sprintf(buffer, "spotlights[%i].cutOff", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, spotlights[i].cutOff);
			
			sprintf(buffer, "spotlights[%i].outerCutOff", i);
			tempLoc = glGetUniformLocation(phong, buffer);
			glUniform1f(tempLoc, spotlights[i].outerCutOff);
		}*/
		
    graphicsRenderEntities(); //phoenix's thing
    
    glfwSwapBuffers(window);
}

void graphicsEnd()
{
    glfwTerminate();
		terminateAssets();
}

// TODO(Ethan): Should this function really be in here?
bool shouldCloseWindow()
{
    return glfwWindowShouldClose(window);
}

void graphics_framebuffer_callback(int w, int h)
{
		// if the window is minimized, set sleeping to true
		if(w == 0 || h == 0){
			windowSleeping = true;
		} else {
			windowSleeping = false;
		}
		
		glViewport(0, 0, w, h);
}

void graphics_maximize_callback(int maximized){
	//const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	/*if(maximized){
		g_maximized = true;
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		g_maximized = false;
		glfwSetWindowMonitor(window, NULL, 50, 50, 800, 600, mode->refreshRate);
		glfwRestoreWindow(window);
	}*/
	
	//glfwSwapInterval(1);
}

// TODO: This should be in a camera module
void graphics_mouse_callback(double xpos, double ypos)
{
			
}

void graphics_key_callback(int key, int scancode, int action, int mods)
{
		// exit full screen
    /*if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && g_maximized){
			graphics_maximize_callback(false);
		} else if(key == GLFW_KEY_F11 && !g_maximized){
			graphics_maximize_callback(true);
		}*/
}