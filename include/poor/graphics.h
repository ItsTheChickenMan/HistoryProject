// graphics manager header

#ifndef GRAPHICS_H
#define GRAPHICS_H
/*
#define NORMALIZE_RGB(r, g, b) (r / 255.0f, g / 255.0f, b / 255.0f)
*/


#include <glm/glm.hpp>
#include <poor/types.h>
#include <poor/entity.h>
#include <poor/asset.h>
#include <poor/camera.h>

// TODO(Ethan): Move render_data related stuff to its own thing. or do we??

// TODO(Ethan): Do we support Index Buffers?
struct Render_Data {
    u32 vao; // TODO: 1 VAO for each render data? VAO binding is slow. 
    u32 vbo; // TODO: Multiple VBO's?
    s32 vertex_count;
    u32 texture; // TODO: Multiple Textures??
    
    glm::mat4 model;
    
		int id; //NOTE from Phoenix: entities need ids, so...had to add this
};

struct Light {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 position;
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight {
	glm::vec3 position;  
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Mipmap_Info {
    u32 wrap_s;
    u32 wrap_t;
    u32 min_filter;
    u32 max_filter;
};

// TODO: This goes somewhere else.
struct Transform {
    glm::vec3 position;
    glm::vec3 rotation; // NOTE: Probably switch to quats, but my small brain cant handle them.
    glm::vec3 scale;
};

// phoenix's stuff
void pushLight(glm::vec3 position);
void pushPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
void pushSpotlight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

bool getKeyPressed(int key);
EntityManager<Render_Data> *getRenderManager();
Render_Data createRenderData(char* texturePath);
Render_Data createRenderData(Texture_Info *texture);
void graphicsRenderEntities();
// end of phoenix's stuff

void init_render_data(Render_Data* render_data, 
                      Texture_Info texture_info,
                      Mipmap_Info mipmap_info,  
                      float* vertex_data,       
                      u32 vertex_data_size);    

void update_render_data(Render_Data* data, Transform trans);

// TODO(Ethan): Do we make every Render_Data store their own shader? They could have different vertex attributes so it might be a good idea if we have alot of vertex attributes/shaders types. 

void draw_render_data(Render_Data* data, u32 shader);

bool shouldCloseWindow();

int graphicsInit();

void graphicsUpdate();

void graphicsEnd();

void graphics_framebuffer_callback(int w, int h);

void graphics_maximize_callback(int maximized);

void graphics_mouse_callback(double xpos, double ypos);

void graphics_key_callback(int key, int scancode, int action, int mods);

#endif