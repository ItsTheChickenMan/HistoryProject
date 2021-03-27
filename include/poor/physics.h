// physics header (pretty much identical to server's)

#ifndef PHYSICS_H
#define PHYSICS_H

#include <tgmath.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "poor/entity.h"
#include <glm/vec3.hpp>

#define PI 3.14159265358979323 //yeah, a little cheap, whatever

// axes
#define PHYSICS_X_AXIS 0
#define PHYSICS_Y_AXIS 1
#define PHYSICS_Z_AXIS 2

// entity types
#define PHYSICS_ENTITY_DYNAMIC 0	// can move as a result of collision
#define PHYSICS_ENTITY_STATIC 1		// can not move as a result of collisions

// Vector3
struct Vector3 {
	double x;
	double y;
	double z;
};

// Basic physics entity, has just enough data to exist
struct PhysicsEntity {
	Vector3 position;
	Vector3 rotation;
	
	Vector3 velocity;
	
	// clientside values, only need to be set when useServersideMovement is false
	Vector3 boundingBox; //bounding box used for collisions (for now, assumes no rotation, although this may be changed in the future)
	double boundingCircle; // diameter of bounding circle used to determine if we need to check collision or not
	
	double friction; //percent speed loss per frame
	
	int entityType; // type of entity to use when handling collisions
	
	// except for this
	int id;
};

PhysicsEntity *getNullEntity();
EntityManager<PhysicsEntity> *getManager();
double toRadians(double degrees);
double toDegrees(double radians);

// vector functions
Vector3 createVector3(double x, double y, double z);
Vector3 createBlankVector3();
double *toArrayVector3(Vector3 c, double *arr);
Vector3 fromArrayVector3(double vector_array[3]);
double getMagVector3(Vector3 v);
Vector3 normalizeVector3(Vector3 v);
Vector3 magnifyVector3(Vector3 v, double amount);
Vector3 multiplyVector3(Vector3 a, Vector3 b);
Vector3 addVector3(Vector3 a, Vector3 b);
Vector3 subtractVector3(Vector3 a, Vector3 b);
double dotVector3(Vector3 a, Vector3 b);
Vector3 crossVector3(Vector3 a, Vector3 b);
Vector3 rotateVector3(Vector3 *vec, double theta, int axis);
glm::vec3 toGlmVec3(Vector3 v);
bool compareVector3(Vector3 *a, Vector3 *b);
bool isVector3Empty(Vector3 *a);
void printVector3(Vector3 v, bool newline);

// entity functions
PhysicsEntity createPhysicsEntity();
PhysicsEntity createPhysicsEntity(Vector3 position, Vector3 rotation, Vector3 velocity);
PhysicsEntity createPhysicsEntity(Vector3 position, Vector3 rotation, Vector3 velocity, Vector3 boundingBox, double friction, int entityType);
double calculateBoundingCircle(Vector3 boundingBox);
Vector3 generateCorner(PhysicsEntity *e, int width, int height, int depth);
void forcePhysicsEntity(PhysicsEntity *entity, Vector3 force);
void updatePhysicsEntity(PhysicsEntity *entity, double deltaFactor);
Vector3 getNextFramePhysicsEntity(PhysicsEntity *a);
bool checkPhysicsEntityCollision(PhysicsEntity *a, PhysicsEntity *b);
void handlePhysicsEntityCollisions(std::vector<PhysicsEntity*> *collisionGroup);
void handleAllMapCollisions(std::vector<PhysicsEntity> *mapObjects);
void handleAllCollisions(std::vector<PhysicsEntity> map);
// temp 
bool vectorContainsItem(std::vector<PhysicsEntity> *v, PhysicsEntity *p);
bool vectorContainsItem(std::vector<PhysicsEntity*> *v, PhysicsEntity *p);
// end temp
void printPhysicsEntity(PhysicsEntity *e, bool newline);

void updateEnvironment(double deltaFactor);

#endif