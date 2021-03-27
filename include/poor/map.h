// map header pep compiler

#ifndef MAP_H
#define MAP_H

#include <poor/physics.h> // requires some structs and functions from physics
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define MAP_PEP 0
#define MAP_PEM 1

struct MapObject {
	Vector3 position;
	Vector3 rotation;
	Vector3 boundingBox; //for now, only use bounding box collisions
	
	Vector3 directionalVector;
	
	int texture; // integer representing texture
};

// struct representing a parsed map.  for now, only holds map objects, but in the future we can expand on this
struct Map {
	std::vector<MapObject> objects;
	std::vector<MapObject> lights; // note: only position is needed
	std::vector<MapObject> pointLights; // note: position used normally, rotation is the constant, linear, and quadratic, boundingBox is the diffuse color, texture & directionalVector irrelevant 
	std::vector<MapObject> spotlights; // note: position used normally, rotation is the constant, linear, and quadratic, boundingBox is the direction (idk man just use default), directionalVector.x is innerCutoff and directionalVector.y is outerCutoff, directionalVector.z & texture irrelevant
	
	int mode;
};

Map generateCollisionMap(Map *map);
long calculateMapSize(Map *map);
char *createMapBuffer(Map *map);
std::string doubleToString(double d, int trailing);
MapObject createFace(MapObject *obj, int face);
Vector3 calculateDirectionalVector(MapObject *face);

Map parseMap(char* buffer);
char* read_entire_file(const char* file);
long write_to_file(char *file, char *buffer);
long write_to_file(const char *file, char *buffer);
void writeDataToObj(MapObject *obj, double *data, int mode);
Map loadMap(char* path);
Map loadMap(const char* path);
void printMapObj(MapObject *e);

#endif