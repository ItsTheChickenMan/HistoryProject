// networking header

#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>

#define SFML_STATIC
#include "SFML/Network.hpp"
#include "poor/types.h"
#include "poor/networking_types.h"

// represents a request to the server to update a value of the client's entity
// While it's entirely possible to manually create a request, it's recommended to use the functions which automatically handle it all for you
struct NetworkingUpdateRequest {
	int update_type; //field being updated, as defined in networking_types.h
	
	int data_length; //data's length, stored because sizeof is a compile time function so you can't get the length of data at runtime
	double data[8]; //data being sent to the server, stored as an array of doubles because I'm too stupid to think of a better way and most requests involve a bunch of doubles
};

int networkingInit();

sf::UdpSocket *networkingGetSocket();

char* networkingGetServerIp();

char* networkingSetServerIp(char* ip);

u16 *networkingGetListenPort();
u16 *networkingSetListenPort(u16 port);

u16 *networkingGetSendPort();
u16 *networkingSetSendPort(u16 port);

u16 *networkingGetTcpListenPort();
u16 *networkingSetTcpListenPort(u16 port);

u16 *networkingGetTcpSendPort();
u16 *networkingSetTcpSendPort(u16 port);

int *getTimeout();
int *setTimeout(int time);

int networkingSend(void* data, int size);
int networkingSendPacket(sf::Packet packet);

NetworkingUpdateRequest networkingCreateUpdateRequest(int update_type, double data[], int data_length);
sf::Packet networkingPackUpdateRequest(NetworkingUpdateRequest *request);
int networkingSendUpdateRequest(NetworkingUpdateRequest *request);
int networkingGetUpdateRequests(std::vector<NetworkingUpdateRequest> *requests);

int networkingConnect();
int networkingDisconnect();

void networkingParseServerNotice(int notice);

int networkingLog(char* log);

int networkingError(char* error, int code);
#endif