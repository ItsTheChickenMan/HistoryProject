/* date = January 25th 2021 5:31 pm */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "poor/types.h"
#include <poor/physics.h>

struct Camera {
    glm::vec3 position;
    glm::vec3 forward; // Normalized Direction
    glm::vec3 up;
    glm::vec3 direction;
    
    float yaw;
    float pitch;
    float roll;
    
    glm::mat4 view;
    glm::mat4 projection;
};

Vector3 getDirection();

void init_camera(Camera* camera, glm::vec3 pos);

void update_camera(Camera* camera, u32 shader);

// TODO: This is would be a game specific type thing, but for now, just chill
// Game Specific stuff {
void move_camera(Camera* camera, float dt);

void camera_mouse_callback(double xpos, double ypos);

void camera_key_callback(int key, int scancode, int action, int mods);
// }


#endif //CAMERA_H
