// poor engine header

#include <stdio.h>

#include <poor/graphics.h>
#include <poor/physics.h>
#include <poor/networking.h>
#include <poor/map.h>
#include <poor/audio.h>

#include <chrono>

#ifndef POOR_H
#define POOR_H

void poorSetServersideMovement(bool state);
void poorSetCurrentMap(char* map, char* collisionMap);

void poorInit(); //init function, run at start of program to get important things done

void poorUpdate(std::chrono::high_resolution_clock::time_point start);

void poorEnd();

void poorHandleRequest(NetworkingUpdateRequest *request);
void poorParseActiveUpdateRequests(int numRequests);

void poorUpdateRenderData();
void poorSetCamera();
PhysicsEntity *poorGetSelfEntity();
void poorRenderMap(Map map);
std::vector<PhysicsEntity> poorMapToPhysics(Map *map);

std::chrono::high_resolution_clock::time_point poorGetTime();
double poorGetDelta();

int poorLog(char* log);
int poorError(char* error, int code);

void poorSendUpdateRequestClientside(NetworkingUpdateRequest *request);
void tempParseInput();

#endif